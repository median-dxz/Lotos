#include "mainwindow.h"
#include <QHBoxLayout>
#include <QTimer>
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    loadQStyleSheet(":/res/styles/index.qss");
    setWindowTitle(tr("Lotos"));
    componentsLayoutManager();
    test();

    connect(ui->uploadButton, &QPushButton::clicked, this, &MainWindow::onUploadButtonClicked);

    //    QTimer *time = new QTimer;
    //    time->start(20);
    //    connect(time, &QTimer::timeout, this, [=]() {
    //        if (IconWidget::dele) {
    //            delpix();
    //            IconWidget::dele = 0;
    //        }
    //    });
}

MainWindow::~MainWindow() {
    delete ui;
}

bool MainWindow::loadQStyleSheet(const QString &fileName) {
    QFile qssFile(fileName);
    if (qssFile.open(QFile::ReadOnly)) {
        qApp->setStyleSheet(qssFile.readAll());
        qssFile.close();
        return true;
    } else {
        return false;
    }
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    return QWidget::eventFilter(obj, event);
}

void MainWindow::componentsLayoutManager() {
    // set pagebutton toggle signal&icon
    QList<PageButton *> PageButtons = ui->centralwidget->findChildren<PageButton *>();

    for (PageButton *pageButton : qAsConst(PageButtons)) {
        int index = pageButton->objectName().rightRef(1).toInt();

        pageButton->setIndex(index);
        connect(this, &MainWindow::widgetPageChanged, pageButton, &PageButton::setCurrentChosen);
        connect(pageButton, &PageButton::indexChanged, this, [=](int current_index) {
            emit widgetPageChanged(current_index);
            ui->stackedWidget->setCurrentIndex(current_index - 1);
        });

        pageButton->setIconPath(iconPaths.at(index * 2 - 2), iconPaths.at(index * 2 - 1));
        pageButton->setIconSize(QSize(28, 28));
        pageButton->setIconOffset(18, pageButton->height() / 2 - 10);
        pageButton->drawPix(iconPaths[index * 2 - 2]);
    }
}

void MainWindow::test() {
    qDebug() << QSslSocket::sslLibraryBuildVersionString();
    qDebug() << QSslSocket::supportsSsl();
}

void MainWindow::onUploadButtonClicked() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("选择图片"));
    QPixmap p;

    if (p.load(fileName)) {
        IconWidget *widget = new IconWidget(ui->dragBox);

        widget->setGeometry(8, 8, p.width() / p.height() * (ui->dragBox->height() - 80), ui->dragBox->height() - 16);
        widget->show();

        widget->setPic(fileName);

        iconWidgets.append(widget);
        int left_pos = 0;
        for (QList<IconWidget *>::iterator iter = iconWidgets.begin(); iter != iconWidgets.end(); iter++) {
            (*iter)->move(left_pos + 8, 8);
            left_pos += (*iter)->width();
        }

        connect(widget, &QWidget::destroyed, this, [=](QObject *obj) {
            QList<IconWidget *>::iterator del_iter;
            for (QList<IconWidget *>::iterator iter = iconWidgets.begin(); iter != iconWidgets.end(); iter++) {
                if (*iter == obj) {
                    del_iter = iter;
                }
            }
            iconWidgets.erase(del_iter);
            int left_pos = 0;
            for (QList<IconWidget *>::iterator iter = iconWidgets.begin(); iter != iconWidgets.end(); iter++) {
                (*iter)->move(left_pos + 8, 8);
                left_pos += (*iter)->width();
            }
        });
    }
}
