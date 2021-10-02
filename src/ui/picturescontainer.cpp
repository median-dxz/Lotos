#include "picturescontainer.h"

#include "base.h"

#include "notification.h"
#include "pictureviewwidget.h"
#include "utils/imagehost.h"
#include "utils/promise.h"

constexpr QSize PicturesContainer::iconWidgetSize;

PicturesContainer::PicturesContainer(QWidget *parent) : QScrollArea(parent) {
    setAcceptDrops(true);
    notify = &NotificationManager::Instance();
    connect(this, &PicturesContainer::acceptDragFiles, this, &PicturesContainer::addIconWidgets);
}

void PicturesContainer::getContentsLayout() {
    gridLayout = static_cast<QGridLayout *>(widget()->layout());
}

void PicturesContainer::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
        return;
    }
}

void PicturesContainer::dropEvent(QDropEvent *event) {
    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty()) {
        return;
    }
    QStringList list;
    for (const QUrl &url : qAsConst(urls)) {
        list << url.toLocalFile();
    }
    emit acceptDragFiles(list);
}

void PicturesContainer::delAllIconWidgets() {
    for (auto i : qAsConst(m_widgets)) {
        if (!m_clients[i].isNull()) {
            m_clients[i]->cancel();
        }
        i->setGraphicsEffect(nullptr);
        i->deleteLater();
        gridLayout->removeWidget(i);
    }
    m_widgets.clear();
    m_hashs.clear();
    m_clients.clear();
    widget()->update();
    emit iconWidgetsChanged(m_widgets.size());
}

void PicturesContainer::delIconWidget(IconWidget *obj) {
    if (!m_clients[obj].isNull()) {
        m_clients[obj]->cancel();
    }

    obj->setGraphicsEffect(nullptr);
    obj->deleteLater();

    m_widgets.removeOne(obj);
    m_hashs.remove(obj);
    m_clients.remove(obj);

    while (!gridLayout->isEmpty()) {
        delete gridLayout->takeAt(0);
    }

    int c = uploadBoxCols();
    for (int i = 0; i < m_widgets.size(); i++) {
        gridLayout->addWidget(m_widgets.at(i), i / c, i % c);
    }

    widget()->update();
    emit iconWidgetsChanged(m_widgets.size());
}

void PicturesContainer::addIconWidgets(QStringList fileNames) {
    int length = fileNames.size();
    if (fileNames.size() > UPLOAD_FILES_LIMIT - m_widgets.size()) {
        length = UPLOAD_FILES_LIMIT - m_widgets.size();
        notify->newNotify(NOTIFYS::ERROR, NOTIFYS::imageWidgetLimit(UPLOAD_FILES_LIMIT));
    }
    for (int i = 0; i < length; i++) {
        addIconWidget(fileNames.at(i));
    }
}

void PicturesContainer::previewImage(IconWidget *obj) {
    PictureViewWidget &view = PictureViewWidget::Instance();
    view.showInfo(obj->imageData(), obj->imageInfo());
    view.show();
}

void PicturesContainer::addIconWidget(QString filename) {
    if (UPLOAD_FILES_LIMIT == m_widgets.size()) {
        notify->newNotify(NOTIFYS::ERROR, NOTIFYS::imageWidgetLimit(UPLOAD_FILES_LIMIT));
        return;
    }

    Promise<QByteArray> *promise = new Promise<QByteArray>(this);
    Promise<void> *promise1 = new Promise<void>(this);
    Promise<QString> *promise2 = new Promise<QString>(this);

    promise->onFinished([=](Promise<QByteArray>::result data) {
        promise1->setPromise([=](Promise<void>::Resolver, Promise<void>::Rejector reject) {
            if (QImage::fromData(data).isNull()) {
                reject();
            }
        });

        promise2->onFinished([=](Promise<QString>::result tmpHash) {
            for (const QString &i : qAsConst(m_hashs)) {
                if (i == tmpHash) {
                    notify->newNotify(NOTIFYS::ERROR, NOTIFYS::imageRepeated(filename));
                    return;
                }
            }
            if (SMMS::supportFormat(data) == "") {
                notify->newNotify(NOTIFYS::ERROR, NOTIFYS::imageNotSupported());
                return;
            }

            if (!m_widgets.count()) {
                widget()->update();
            }

            IconWidget *i = new IconWidget(this);
            i->hide();  //减少闪烁

            int c = uploadBoxCols();
            gridLayout->addWidget(i, m_widgets.count() / c, m_widgets.count() % c);
            i->setFixedSize(iconWidgetSize);
            i->addImageFromFile(filename, data);

            i->show();

            m_widgets.append(i);
            m_hashs.insert(i, tmpHash);
            m_clients.insert(i, nullptr);
            emit iconWidgetsChanged(m_widgets.count());

            connect(i, &IconWidget::onUploadBtnClicked, this, &PicturesContainer::uploadImage);
            connect(i, &IconWidget::onViewBtnClicked, this, &PicturesContainer::previewImage);
            connect(i, &IconWidget::onDeleteBtnClicked, this, &PicturesContainer::delIconWidget);
        });

        promise2->setPromise([=](Promise<QString>::Resolver resolve, Promise<QString>::Rejector) {
            resolve(QCryptographicHash::hash(data.left(1024 * 20), QCryptographicHash::Md5).toHex());
        });
    });

    promise->onFailed([=] { notify->newNotify(NOTIFYS::ERROR, NOTIFYS::imageFileError(filename)); });
    promise1->onFailed([=] { notify->newNotify(NOTIFYS::ERROR, NOTIFYS::imageFileError(filename)); });

    promise->setPromise([=](Promise<QByteArray>::Resolver resolve, Promise<QByteArray>::Rejector reject) {
        QFile file(filename);
        if (!file.open(QFile::ReadOnly)) {
            reject();
        }
        resolve(file.readAll());
    });
}

int PicturesContainer::count() {
    return m_widgets.size();
}

QPointer<HttpClient> &PicturesContainer::client(IconWidget *i) {
    return m_clients[i];
}

QString PicturesContainer::hash(IconWidget *i) const {
    return m_hashs[i];
}

QList<IconWidget *> PicturesContainer::widgets() const {
    return m_widgets;
}
