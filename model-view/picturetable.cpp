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

    connect(header->head_name , &QPushButton::clicked ,this,[=](){
        devListSort(flag,1,Lines);
        if(flag == 1)
        {
            header->fnSort->setPixmap(QPixmap(":/上箭头.png"));
            header->sizeSort->setPixmap(QPixmap(""));
            header->linkSort->setPixmap(QPixmap(""));
            flag =0;}
        else
        {
            header->fnSort->setPixmap(QPixmap(":/下箭头.png"));
            header->sizeSort->setPixmap(QPixmap(""));
            header->linkSort->setPixmap(QPixmap(""));
            flag =1;}
    });
    connect(header->head_size , &QPushButton::clicked ,this,[=](){
        devListSort(flag,2,Lines);
        if(flag == 1)
        {
            header->sizeSort->setPixmap(QPixmap(":/上箭头.png"));
            header->fnSort->setPixmap(QPixmap(""));
            header->linkSort->setPixmap(QPixmap(""));
            flag =0;}
        else
        {
            header->sizeSort->setPixmap(QPixmap(":/下箭头.png"));
            header->fnSort->setPixmap(QPixmap(""));
            header->linkSort->setPixmap(QPixmap(""));
            flag =1;}
    });
    connect(header->head_link , &QPushButton::clicked ,this,[=](){
        devListSort(flag,3,Lines);
        if(flag == 1)
        {
            header->linkSort->setPixmap(QPixmap(":/上箭头.png"));
            header->fnSort->setPixmap(QPixmap(""));
            header->sizeSort->setPixmap(QPixmap(""));
            flag =0;}
        else
        {
            header->linkSort->setPixmap(QPixmap(":/下箭头.png"));
            header->fnSort->setPixmap(QPixmap(""));
            header->sizeSort->setPixmap(QPixmap(""));
            flag =1;}
    });
//        Lines[0]->lab_filename->setText("213214");
//        Lines[1]->lab_filename->setText("213214");


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
    head_name =new QPushButton(this);
    //head_name->setStyleSheet("color: blue; background-color: yellow");
    head_name->setFixedWidth(rowWidth[0]);
    head_name->setFont(font);
    head_name->setText("文件名");
    head_name->setStyleSheet("text-align:left");
    head_name->setProperty("class_type", "head");
//    QLabel *l = new QLabel(head_name);
//    l->setGeometry(55,3,20,16);
//    l->setScaledContents(true);
//    //l->setStyleSheet("color: blue; background-color: yellow");
//    l->setPixmap(QPixmap(":/checkbox.png"));
    fnSort=new QLabel(head_name);
    fnSort->setGeometry(55,3,20,16);
    fnSort->setScaledContents(true);
//    fnSort->setIcon(QIcon("C:/Users/lenovo/Desktop/downarrow.png"));
    //head_name->setTextInteractionFlags(Qt::TextSelectableByMouse);
    layout()->addWidget(head_name);

    QLabel *head_pix =new QLabel(this);
    //head_link->setStyleSheet("color: blue; background-color: yellow");
    head_pix->setFixedWidth(rowWidth[1]);
    head_pix->setFont(font);
    head_pix->setText("预览");
    head_pix->setTextInteractionFlags(Qt::TextSelectableByMouse);
    layout()->addWidget(head_pix);




    head_link =new QPushButton(this);
    //head_link->setStyleSheet("color: blue; background-color: yellow");
    head_link->setFixedWidth(rowWidth[2]);
    head_link->setFont(font);
    head_link->setText("上传时间");
    head_link->setStyleSheet("text-align:left");
    head_link->setProperty("class_type", "head");
   // head_link->setTextInteractionFlags(Qt::TextSelectableByMouse);
    ((QHBoxLayout *)layout())->addSpacing(60);
    layout()->addWidget(head_link);

    linkSort=new QLabel(head_link);
    linkSort->setGeometry(70,3,20,16);
    linkSort->setScaledContents(true);
//    linkSort->setIcon(QIcon("C:/Users/lenovo/Desktop/downarrow.png"));
    //head_link->setTextInteractionFlags(Qt::TextSelectableByMouse);
    layout()->addWidget(head_link);

    head_size =new QPushButton(this);
    //head_size->setStyleSheet("color: blue; background-color: yellow");
    head_size->setFixedWidth(rowWidth[3]);
    head_size->setFont(font);
    head_size->setText("大小");
    head_size->setStyleSheet("text-align:left");
    head_size->setProperty("class_type", "head");
    //head_size->setTextInteractionFlags(Qt::TextSelectableByMouse);
    ((QHBoxLayout *)layout())->addSpacing(40);
    layout()->addWidget(head_size);

    sizeSort=new QLabel(head_size);
    sizeSort->setGeometry(40,3,20,16);
    sizeSort->setScaledContents(true);
//    sizeSort->setIcon(QIcon("C:/Users/lenovo/Desktop/downarrow.png"));
    //head_size->setTextInteractionFlags(Qt::TextSelectableByMouse);
    layout()->addWidget(head_size);

    QLabel *head_rec =new QLabel(this);
    //head_rec->setStyleSheet("color: blue; background-color: yellow");
    head_rec->setFixedWidth(rowWidth[4]);
    head_rec->setFont(font);
    head_rec->setText("上传来源");
    head_rec->setTextInteractionFlags(Qt::TextSelectableByMouse);
    ((QHBoxLayout *)layout())->addSpacing(40);
    layout()->addWidget(head_rec);

    QLabel *head_op =new QLabel(this);
    //head_rec->setStyleSheet("color: blue; background-color: yellow");
    head_op->setFixedWidth(rowWidth[5]);
    head_op->setFont(font);
    head_op->setText("操作");
    head_op->setTextInteractionFlags(Qt::TextSelectableByMouse);
    ((QHBoxLayout *)layout())->addSpacing(50);
    layout()->addWidget(head_op);

}


PictureTableLine::PictureTableLine(QWidget *parent, QVariantMap &data) : QWidget(parent), data(data) {
//    setMouseTracking(true);
    in = s--;
    content.append(data[DataKey.filename].toString());
    content.append(data[DataKey.width].toString());
    content.append(data[DataKey.size].toString());
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
    lab_filename->setText(data[DataKey.filename].toString());
    lab_width = new QLabel(this);
    lab_width->setFixedWidth(rowWidth[1]);
    lab_link = new QLabel(this);
    //lab_link->setStyleSheet("color: blue; background-color: yellow");
    lab_link->setFixedWidth(rowWidth[2]);
    lab_link->setText(data[DataKey.width].toString());
    lab_size = new QLabel(this);
    //lab_size->setStyleSheet("color: blue; background-color: yellow");
    lab_size->setFixedWidth(rowWidth[3]);
    lab_size->setText(data[DataKey.size].toString());
    lab_rec = new QLabel(this);
    //lab_rec->setStyleSheet("color: blue; background-color: yellow");
    lab_rec->setFixedWidth(rowWidth[4]);
    lab_rec->setText(QString::number(in));
    opt_del = new QPushButton(this);
    opt_del->setFixedWidth(90);
    opt_del->setStyleSheet("text-align:left");

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
    connect(op_del , &QPushButton::clicked , this, [=](){emit deleteLine(this);});

    opt_del->setProperty("class_type", "link");
    opt_del->setFont(font);
    opt_del->setText(" 复制链接");

    arrow = new QLabel(opt_del);
    arrow->setGeometry(70,3,20,18);
    arrow->setScaledContents(true);
    arrow->setPixmap(QPixmap("C:/Users/lenovo/Desktop/downarrow.png"));


    bx = new QComboBox(this);
    bx->addItem("预览");
    bx->addItem("删除");
    bx->addItem("下载");
    bx->addItem("复制链接");
    layout()->addWidget(bx);
    ((QHBoxLayout *)layout())->addSpacing(10);
    connect(opt_del, &QPushButton::clicked, bx, &QComboBox::showPopup);
    connect(opt_del, &QPushButton::clicked, arrow ,[&](){arrow->setPixmap(QPixmap("C:/Users/lenovo/Desktop/uparrow.png"));});
    connect(bx, &QComboBox::hidePopup, arrow ,[&](){
        qDebug()<<"fadsgas";
        arrow->setPixmap(QPixmap("C:/Users/lenovo/Desktop/downarrow.png"));});
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
    if(i==0)
    {
        qDebug()<<"执行预览操作";

    }
    if(i==1)
    {
        qDebug()<<"执行删除操作";
    }
    if(i==2)
    {
        qDebug()<<"执行下载操作";
    }
    if(i==3)
    {
        qDebug()<<"执行复制链接操作";

    }


}

Qt::CheckState PictureTableLine::getCheckStatus(){return  cb->checkState();}
void PictureTableLine::setCheckState(Qt::CheckState state){ cb->setCheckState(state);}


void PictureTable::devListSort(int cmp , int sel , QList <PictureTableLine *> list)
{
    QList <QString > fn;//更新列表数据
    QList <QString > link;
    QList <QString > size;
    QList <int > rec;
    QList <int > width;
    QList <Qt::CheckState > check;
    if(cmp)
    {
        switch (sel) {
        case 1:qSort(list.begin(), list.end(),[](const  PictureTableLine*info1, const PictureTableLine *info2){return info1->content[0] < info2->content[0];});break;
        case 2:qSort(list.begin(), list.end(), [](const PictureTableLine *info1, const PictureTableLine *info2){return info1->content[1] < info2->content[1];});break;
        case 3:qSort(list.begin(), list.end(), [](const PictureTableLine *info1, const PictureTableLine *info2){return info1->content[2] < info2->content[2];});break;
        }

    }
    else
    {
        switch (sel) {
        case 1:qSort(list.begin(), list.end(), [](const  PictureTableLine*info1, const PictureTableLine *info2){return info1->content[0] > info2->content[0];});break;
        case 2:qSort(list.begin(), list.end(), [](const PictureTableLine *info1, const PictureTableLine *info2){return info1->content[1] > info2->content[1];});break;
        case 3:qSort(list.begin(), list.end(), [](const PictureTableLine *info1, const PictureTableLine *info2){return info1->content[2] > info2->content[2];});break;

        }

    }


    for(int i = 0; i <list.length(); i++)
    {
        fn.append(list[i]->content[0]);
        link.append(list[i]->content[1]);
        size.append(list[i]->content[2]);
        rec.append(list[i]->in);
        width.append(list[i]->in);
        check.append(list[i]->getCheckStatus());
        qDebug()<< i << list.at(i)->content[0];
        //Lines[i]->resetLine(list[i]->in , list[i]->an , list[i]->an ,list[i]->an );
       // Lines[i]->resetLine(fn[i] , link[i] ,size[i] ,rec[i] );
        //Lines[i] =list[i];//这个不行是因为换了整体
        qDebug()<< i << Lines.at(i)->content[0];//还没换过去
    }

    for(int i = 0; i <list.length(); i++)
    {
        Lines[i]->resetLine(width[i] , fn[i], link[i] ,size[i] ,rec[i] );
        Lines[i]->content[0] =fn[i];
        Lines[i]->content[1] =link[i];
        Lines[i]->content[2] =size[i];
        Lines[i]->in =rec[i];
        Lines[i]->setCheckState(check[i]);
        //qDebug()<< i << Lines.at(i)->in;//换过去
    }

}
int PictureTableLine::s=3;

void PictureTableLine::resetLine(int pix,QString fn , QString link , QString size ,int c)
{
    p.load(":/checkbox_o.png");
    p = p.scaledToHeight(height() - 8, Qt::SmoothTransformation);
    lab_width->setPixmap(p);
    lab_filename->setText(fn);
    lab_link->setText(link);
    lab_size->setText(size);
    lab_rec->setText(QString::number(c));
}


