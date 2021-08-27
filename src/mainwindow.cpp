#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    loadQStyleSheet(":/res/styles/index.qss");

    QList<PageButton *> PageButtons = ui->centralwidget->findChildren<PageButton *>();
    for (PageButton *pageButton : qAsConst(PageButtons)) {
        int index = pageButton->objectName().rightRef(1).toInt();
        pageButton->setIndex(index);
        connect(this, &MainWindow::widgetPageChanged, pageButton, &PageButton::setCurrentChosen);

        connect(pageButton, &PageButton::indexChanged, this, [=](int current_index) {
            emit widgetPageChanged(current_index);
            ui->stackedWidget->setCurrentIndex(current_index - 1);
        });
    }
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
