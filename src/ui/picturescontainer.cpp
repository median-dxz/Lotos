#include "picturescontainer.h"

#include "base.h"

PicturesContainer::PicturesContainer(QWidget *parent) : QScrollArea(parent) {
    setAcceptDrops(true);
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
    for (const QUrl &url : qAsConst(urls)) {
        QString fileName = url.toLocalFile();
        QFile file(fileName);
        if (file.exists()) {
            emit acceptDragFileName(fileName);
        }
    }
}
