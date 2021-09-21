#ifndef PICTURESCONTAINER_H
#define PICTURESCONTAINER_H

#include <QScrollArea>

class PicturesContainer : public QScrollArea {
    Q_OBJECT
   public:
    explicit PicturesContainer(QWidget *parent = nullptr);

   signals:
    void acceptDragFileName(QString);

   protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
};

#endif  // PICTURESCONTAINER_H
