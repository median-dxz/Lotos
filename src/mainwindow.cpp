#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QPainter>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
<<<<<<< HEAD
<<<<<<< HEAD

    LoadQStyleSheet(":/res/styles/index.qss");
=======
    loadQStyleSheet(":/res/styles/index.qss");
    setWindowTitle(tr("Lotos"));
    componentsLayoutManager();
>>>>>>> 614284c (pref: 优化图标绘制，整理主窗口代码)
=======
    setWindowTitle(tr("Lotos"));
    init();
>>>>>>> 79dc943 (feat: 添加api接口定义)

    test();
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

void MainWindow::init() {
    loadQStyleSheet(":/res/styles/index.qss");

    componentsLayoutManager();

    proxy = HttpClient::getNetworkProxyInstanse();
    smms = &SMMS::getInstance();
}

void MainWindow::componentsLayoutManager() {
    // set pagebutton toggle signal&icon
    QList<PageButton *> PageButtons = ui->centralwidget->findChildren<PageButton *>();
    QList<QString> iconPaths;

    iconPaths.append(":/res/icons/page_1.png");
    iconPaths.append(":/res/icons/page_1_ig.png");
    iconPaths.append(":/res/icons/page_2.png");
    iconPaths.append(":/res/icons/page_2_ig.png");
    iconPaths.append(":/res/icons/page_3.png");
    iconPaths.append(":/res/icons/page_3_ig.png");
    iconPaths.append(":/res/icons/page_4.png");
    iconPaths.append(":/res/icons/page_4_ig.png");

    for (PageButton *pageButton : qAsConst(PageButtons)) {
        int index = pageButton->objectName().rightRef(1).toInt();

        pageButton->setIndex(index);
        connect(this, &MainWindow::widgetPageChanged, pageButton, &PageButton::setCurrentChosen);
        connect(pageButton, &PageButton::indexChanged, this, [=](int current_index) {
            emit widgetPageChanged(current_index);
            ui->stackedWidget->setCurrentIndex(current_index - 1);
        });

        pageButton->setIconPath(iconPaths[index * 2 - 2], iconPaths[index * 2 - 1]);
        pageButton->setIconSize(QSize(28, 28));
        pageButton->setIcon(QIcon(iconPaths[index * 2 - 2]));
        pageButton->installEventFilter(this);
    }
}

void MainWindow::test() {
    qDebug() << QSslSocket::supportsSsl();
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    if (obj->inherits("PageButton") && event->type() == QEvent::HoverEnter) {
        PageButton *page_btn = (PageButton *)obj;
        page_btn->drawIcon(true);
    }
    if (obj->inherits("PageButton") && event->type() == QEvent::HoverLeave) {
        PageButton *page_btn = (PageButton *)obj;
        page_btn->drawIcon(false);
    }
    return QWidget::eventFilter(obj, event);
}
