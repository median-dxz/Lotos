#include "picturetable.h"
#include <QDebug>

PictureTable::PictureTable(QWidget *parent) : QFrame(parent) {
    header = new PictureTableHeader(this);
    header->setFixedHeight(lineHeight);
    setLayout(new QVBoxLayout(this));
    layout()->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    layout()->setMargin(0);
    layout()->setSpacing(0);
    layout()->addWidget(header);
    connect(header->all ,&QCheckBox::stateChanged, this,[=](){
        for(int i=0 ; i<Lines.size(); i++)
            if(header->all->checkState()==Qt::Checked)
                Lines[i]->setCheckState(Qt::Checked);
            else  Lines[i]->setCheckState(Qt::Unchecked);
    });

    connect(header->fnSort , &QPushButton::clicked ,this,[=](){
//        header->fnSort->setIcon(QIcon(":/checkbox_o_hover.png"));
        devListSort(1,1,Lines);});
    connect(header->sizeSort , &QPushButton::clicked ,this,[=](){
//        header->sizeSort->setIcon(QIcon(":/checkbox_o_hover.png"));
        devListSort(0,2,Lines);});
    connect(header->linkSort , &QPushButton::clicked ,this,[=](){
//        header->linkSort->setIcon(QIcon(":/checkbox_o_hover.png"));
        devListSort(1,3,Lines);});

}
void PictureTable::deleteLine1(PictureTableLine * obj)
{
    QList<PictureTableLine *>::iterator del_iter;
    for (QList<PictureTableLine *>::iterator iter = Lines.begin(); iter != Lines.end();iter++) {
        if (*iter == obj) {
            del_iter = iter;
            break;
        }
    }
   this->layout()->removeWidget(obj);
   Lines.erase(del_iter);
   obj->deleteLater();
}

void PictureTable::addData(QVariantMap d) {

    PictureTableLine *line = new PictureTableLine(this, d);
    line->setFixedHeight(lineHeight);
    layout()->addWidget(line);
    datas.append(d);
    Lines.append(line);
    connect( line , &PictureTableLine::onStateChanged, this,[=](){
        list.clear();
        for(int i=0 ; i<Lines.size(); i++)
       {

            list.append(Lines[i]->getCheckStatus());
            qDebug()<<list[i]; }});
    connect(line , SIGNAL(deleteLine(PictureTableLine *)) ,this,SLOT(deleteLine1(PictureTableLine *)));

}

PictureTableHeader::PictureTableHeader(QWidget *parent) : QWidget(parent) {

    setLayout(new QHBoxLayout(this));
    setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed));
    layout()->setMargin(0);
    layout()->setSpacing(0);
    layout()->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    all = new QCheckBox(this);
    all->setFixedWidth(40);
    layout()->addWidget(all);
    QFont font ( "Microsoft YaHei", 8, 10);
    QLabel *head_name =new QLabel(this);
    head_name->setFixedWidth(rowWidth[0]);
    head_name->setFont(font);
    head_name->setText("文件名");
    fnSort=new QPushButton(head_name);
    fnSort->setGeometry(55,3,20,16);
    fnSort->setIcon(QIcon(":/checkbox_o_hover.png"));
    head_name->setTextInteractionFlags(Qt::TextSelectableByMouse);
    layout()->addWidget(head_name);

    QLabel *head_pix =new QLabel(this);
    head_pix->setFixedWidth(100);
    head_pix->setFont(font);
    head_pix->setText("预览");
    head_pix->setTextInteractionFlags(Qt::TextSelectableByMouse);
    layout()->addWidget(head_pix);

    QLabel *head_link =new QLabel(this);
    head_link->setFixedWidth(rowWidth[1]);
    head_link->setFont(font);
    head_link->setText("上传时间");
    head_link->setTextInteractionFlags(Qt::TextSelectableByMouse);
    ((QHBoxLayout *)layout())->addSpacing(60);
    layout()->addWidget(head_link);

    linkSort=new QPushButton(head_link);
    linkSort->setGeometry(70,3,20,16);
    linkSort->setIcon(QIcon(":/checkbox_o_hover.png"));
    head_link->setTextInteractionFlags(Qt::TextSelectableByMouse);
    layout()->addWidget(head_link);

    QLabel *head_size =new QLabel(this);
    head_size->setFixedWidth(rowWidth[2]);
    head_size->setFont(font);
    head_size->setText("大小");
    head_size->setTextInteractionFlags(Qt::TextSelectableByMouse);
    ((QHBoxLayout *)layout())->addSpacing(40);
    layout()->addWidget(head_size);

    sizeSort=new QPushButton(head_size);
    sizeSort->setGeometry(40,3,20,16);
    sizeSort->setIcon(QIcon(":/checkbox_o_hover.png"));
    head_size->setTextInteractionFlags(Qt::TextSelectableByMouse);
    layout()->addWidget(head_size);

    QLabel *head_rec =new QLabel(this);
    head_rec->setFixedWidth(rowWidth[3]);
    head_rec->setFont(font);
    head_rec->setText("上传来源");
    head_rec->setTextInteractionFlags(Qt::TextSelectableByMouse);
    ((QHBoxLayout *)layout())->addSpacing(40);
    layout()->addWidget(head_rec);

    QLabel *head_see =new QLabel(this);
    head_see->setFixedWidth(rowWidth[5]);
    head_see->setFont(font);
    head_see->setText("操作");
    head_see->setTextInteractionFlags(Qt::TextSelectableByMouse);
    ((QHBoxLayout *)layout())->addSpacing(50);
    layout()->addWidget(head_see);

}


PictureTableLine::PictureTableLine(QWidget *parent, QVariantMap &data) : QWidget(parent), data(data) {
//    setMouseTracking(true);
    in = s--;
    an =in;
    setLayout(new QHBoxLayout(this));
    setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed));
    layout()->setMargin(0);
    layout()->setSpacing(0);
    layout()->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    cb = new QCheckBox(this);
    cb->setFixedWidth(40);
    connect(cb, &QCheckBox::stateChanged, this, &PictureTableLine::onStateChanged);
    lab_filename = new QLabel(this);
    //lab_filename->setStyleSheet("color: blue; background-color: yellow");
    lab_filename->setFixedWidth(rowWidth[0]);
    lab_filename->setText(QString::number(in));
    lab_link = new QLabel(this);
    //lab_link->setStyleSheet("color: blue; background-color: yellow");
    lab_link->setFixedWidth(rowWidth[1]);
    lab_link->setText(QString::number(an));
    lab_size = new QLabel(this);
    //lab_size->setStyleSheet("color: blue; background-color: yellow");
    lab_size->setFixedWidth(rowWidth[2]);
    lab_size->setText(QString::number(an));
    lab_rec = new QLabel(this);
    //lab_rec->setStyleSheet("color: blue; background-color: yellow");
    lab_rec->setFixedWidth(rowWidth[3]);
    lab_rec->setText(QString::number(an));
    lab_width = new QLabel(this);
    lab_width->setFixedWidth(100);
    opt_del = new QPushButton(this);

    QFont font ( "Microsoft YaHei", 8, 10);
    op_view = new QPushButton(this);
    op_view->setFont(font);
    op_view->setFixedWidth(rowWidth[5]);
    op_view->setText("预览");
    op_del = new QPushButton(this);
    op_del->setFont(font);
    op_del->setFixedWidth(rowWidth[5]);
    op_del->setText("删除");
    op_load = new QPushButton(this);
    op_load->setFont(font);
    op_load->setFixedWidth(rowWidth[5]);
    op_load->setText("下载");




//    p.loadFromData(data[DataKey.thumb].toByteArray());
    p.load(":/checkbox.png");
    p = p.scaledToHeight(height() - 8, Qt::SmoothTransformation);
    lab_width->setPixmap(p);

    layout()->addWidget(cb);
    layout()->addWidget(lab_filename);
    layout()->addWidget(lab_width);
    ((QHBoxLayout *)layout())->addSpacing(40+20);
    layout()->addWidget(lab_link);
    ((QHBoxLayout *)layout())->addSpacing(40);
    layout()->addWidget(lab_size);
    ((QHBoxLayout *)layout())->addSpacing(40);
    layout()->addWidget(lab_rec);
    //((QHBoxLayout *)layout())->addSpacing(10);

    ((QHBoxLayout *)layout())->addSpacing(40);
    layout()->addWidget(op_view);
    ((QHBoxLayout *)layout())->addSpacing(10);
    layout()->addWidget(op_del);
    ((QHBoxLayout *)layout())->addSpacing(10);
    layout()->addWidget(op_load);
    ((QHBoxLayout *)layout())->addSpacing(10);
    layout()->addWidget(opt_del);
    ((QHBoxLayout *)layout())->addSpacing(10);
    op_view->setProperty("class_type", "link");
    op_load->setProperty("class_type", "link");
    op_del->setProperty("class_type", "link");
    opt_del->setProperty("class_type", "link");
    opt_del->setFont(font);
    opt_del->setText(" 复制链接");

    connect(op_del , &QPushButton::clicked , this, [=](){emit deleteLine(this);});
    bx = new QComboBox(this);
    bx->addItem("预览");
    bx->addItem("删除");
    bx->addItem("下载");
    bx->addItem("复制链接");
    layout()->addWidget(bx);
    ((QHBoxLayout *)layout())->addSpacing(10);
    connect(opt_del, &QPushButton::clicked, bx, &QComboBox::showPopup);
    connect(bx , SIGNAL(activated(int)) , this ,SLOT(x(int)));
}


void PictureTableHeader::paintEvent(QPaintEvent *) {
    QPainter painter(this);

    painter.setRenderHints(QPainter::HighQualityAntialiasing | QPainter::TextAntialiasing);
    painter.fillRect(0, 0, width(), height(), QColor("#fff"));
    painter.setPen(QColor("#ebeef5"));
    painter.drawLine(0, 0, width(), 0);
    update();
}

void PictureTableLine::paintEvent(QPaintEvent *) {
    QPainter painter(this);

    painter.setRenderHints(QPainter::HighQualityAntialiasing | QPainter::TextAntialiasing);

    painter.fillRect(0, 0, width(), height(), lineBackgroundColor);

    painter.setPen(QColor("#ebeef5"));
    painter.drawLine(0, 0, width(), 0);
    update();
}

void PictureTableLine:: enterEvent(QEvent *)
{
    lineBackgroundColor =QColor("#f7f7f7");
}

void PictureTableLine::leaveEvent(QEvent *)
{
    lineBackgroundColor =QColor("#fff");
}

void PictureTableLine::x(int i){
    //复制链接操作

}

int PictureTableLine::getCheckStatus(){return  cb->checkState();}
void PictureTableLine::setCheckState(Qt::CheckState state){ cb->setCheckState(state);}


void PictureTable::devListSort(int cmp , int sel , QList <PictureTableLine *> list)
{
    QList <int > fn;//更新列表数据
    QList <int > link;
    QList <int > size;
    QList <int > rec;
    QList <int > width;
    if(cmp==1)
    {
        switch (sel) {
        case 1:qSort(list.begin(), list.end(), [](const PictureTableLine *info1, const PictureTableLine *info2){return info1->in < info2->in;});break;
        case 2:qSort(list.begin(), list.end(), [](const PictureTableLine *info1, const PictureTableLine *info2){return info1->an < info2->an;});break;
        case 3:qSort(list.begin(), list.end(), [](const PictureTableLine *info1, const PictureTableLine *info2){return info1->an < info2->an;});break;
        }

    }
    else
    {
        switch (sel) {
        case 1:qSort(list.begin(), list.end(), [](const PictureTableLine *info1, const PictureTableLine *info2){return info1->in > info2->in;});break;
        case 2:qSort(list.begin(), list.end(), [](const PictureTableLine *info1, const PictureTableLine *info2){return info1->an > info2->an;});break;
        case 3:qSort(list.begin(), list.end(), [](const PictureTableLine *info1, const PictureTableLine *info2){return info1->an > info2->an;});break;

        }

    }

    for(int i = 0; i <list.length(); i++)
    {
        fn.append(list[i]->in);
        link.append(list[i]->an);
        size.append(list[i]->an);
        rec.append(list[i]->an);
        width.append(list[i]->an);
        qDebug()<< i << list.at(i)->in;
        //Lines[i] =list[i];//这个不行是因为换了整体
        qDebug()<< i << Lines.at(i)->in;//还没换过去
    }

    for(int i = 0; i <list.length(); i++)
    {

        Lines[i]->resetLine(width[i] , fn[i] , link[i] ,size[i] ,rec[i] );
        Lines[i]->in =fn[i];
        Lines[i]->an =link[i];
        Lines[i]->an =size[i];
        Lines[i]->an =rec[i];
        qDebug()<< i << Lines.at(i)->in;//换过去
    }

}
int PictureTableLine::s=3;

void PictureTableLine::resetLine(int k,int i , int a , int b ,int c)
{
    p.load(":/checkbox_o.png");
    p = p.scaledToHeight(height() - 8, Qt::SmoothTransformation);
    lab_width->setPixmap(p);
    lab_filename->setText(QString::number(i));
    lab_link->setText(QString::number(a));
    lab_size->setText(QString::number(b));
    lab_rec->setText(QString::number(c));
}


