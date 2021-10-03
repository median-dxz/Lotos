#ifndef PICTURESCONTAINER_H
#define PICTURESCONTAINER_H

#include <QScrollArea>

#include "iconwidget.h"
#include "utils/httpclient.h"

class NotificationManager;
class PictureViewWidget;
class QGridLayout;

class PicturesContainer : public QScrollArea {
    Q_OBJECT
   public:
    explicit PicturesContainer(QWidget *parent = nullptr);

    int count();
    QPointer<HttpClient> &client(IconWidget *i);
    QString hash(IconWidget *i) const;
    QList<IconWidget *> widgets() const;

    void getContentsLayout();
   public slots:
    void delAllIconWidgets();
    void delIconWidget(IconWidget *obj);

    void addIconWidget(QString filename);
    void addIconWidgets(QStringList fileNames);

    void previewImage(IconWidget *obj);

   signals:
    void acceptDragFiles(QStringList fileNames);
    void iconWidgetsChanged(int);
    void uploadImage(IconWidget *);

   protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

   private:
    int setPreviewImage(IconWidget *obj, PictureViewWidget *self, QLabel *imgBox, QLabel *info);

    QList<IconWidget *> m_widgets;
    QMap<IconWidget *, QString> m_hashs;
    QMap<IconWidget *, QPointer<HttpClient>> m_clients;
    QGridLayout *gridLayout;

    NotificationManager *notify;
    PictureViewWidget *view;

    inline int uploadBoxCols() const { return (widget()->width() - 18) / iconWidgetSize.width(); }

    static constexpr int UPLOAD_FILES_LIMIT = 20;
    static constexpr QSize iconWidgetSize = QSize(270, 313);
};

#endif  // PICTURESCONTAINER_H
