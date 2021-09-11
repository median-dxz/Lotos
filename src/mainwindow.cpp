#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    loadQStyleSheet(":/res/styles/index.qss");
    setWindowTitle(tr("Lotos"));
    componentsLayoutManager();
    test();

    connect(ui->uploadButton, &QPushButton::clicked, this, &MainWindow::onUploadButtonClicked);
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
    QGridLayout *gridlayout = new QGridLayout;
    gridlayout->setMargin(8);
    gridlayout->setHorizontalSpacing(8);
    gridlayout->setVerticalSpacing(8);
    gridlayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    ui->dragBoxContents->setLayout(gridlayout);

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
    QString fileName = QFileDialog::getOpenFileName(this, tr("选择图片"), "%USERDATA%\\Pictures");
    //    QString fileName = "56008092_p0.jpg";
    QPixmap p;

    if (p.load(fileName)) {
        int cols = (ui->dragBoxContents->width() - 18) / iconWidgetSize.width();
        int count = iconWidgets.count();
        IconWidget *widget = new IconWidget(ui->dragBox);

        QGridLayout *l = ((QGridLayout *)ui->dragBoxContents->layout());
        l->addWidget(widget, count / cols, count % cols);
        widget->setFixedSize(iconWidgetSize);
        widget->setImage(QImage(fileName));

        iconWidgets.append(widget);

        connect(widget, &IconWidget::onDeleteBtnClicked, this, [=](IconWidget *obj) {
            QList<IconWidget *>::iterator del_iter;
            for (QList<IconWidget *>::iterator iter = iconWidgets.begin(); iter != iconWidgets.end(); iter++) {
                if (*iter == obj) {
                    del_iter = iter;
                }
            }
            iconWidgets.erase(del_iter);

            while (!((QGridLayout *)ui->dragBoxContents->layout())->isEmpty()) {
                delete ((QGridLayout *)ui->dragBoxContents->layout())->takeAt(0);
            }

            obj->deleteLater();

            int c = 0;
            int cols = (ui->dragBoxContents->width() - 18) / iconWidgetSize.width();
            for (QList<IconWidget *>::iterator iter = iconWidgets.begin(); iter != iconWidgets.end(); iter++) {
                l->addWidget(*iter, c / cols, c % cols);
                c++;
            }
        });
    } else {
        qDebug() << "?错误的图片选择?";
    }
}
