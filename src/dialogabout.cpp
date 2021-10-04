#include "dialogabout.h"
#include "ui_dialogabout.h"

#include <QFile>

#include "settingshelper.h"

DialogAbout::DialogAbout(QWidget *parent) : QDialog(parent), ui(new Ui::DialogAbout) {
    ui->setupUi(this);
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    ui->label_version->setText(LotosVersion());
    ui->scrollAreaWidgetContents->setLayout(new QGridLayout);
    ui->scrollAreaWidgetContents->layout()->addWidget(ui->description);
    QFile f(":/res/LICENSE.txt");
    f.open(QIODevice::ReadOnly);
    ui->description->setText(f.readAll());
}

DialogAbout::~DialogAbout() {
    delete ui;
}
