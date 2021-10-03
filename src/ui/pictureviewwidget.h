#ifndef PICTUREVIEWWIDGET_H
#define PICTUREVIEWWIDGET_H

#include <QFileInfo>
#include <QWidget>
#include <functional>

class QLabel;

class PictureViewWidget : public QWidget {
    Q_OBJECT
   public:
    static PictureViewWidget &Instance();
    void init();

    typedef std::function<int(PictureViewWidget *, QLabel *p, QLabel *i)> showManager;

   protected:
    void showEvent(QShowEvent *) override;
    void hideEvent(QHideEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

    bool eventFilter(QObject *obj, QEvent *event) override;
   public slots:
    void showInfo(showManager display);

   private:
    explicit PictureViewWidget(QWidget *parent = nullptr);
    PictureViewWidget(const PictureViewWidget &other) Q_DECL_EQ_DELETE;
    PictureViewWidget &operator=(PictureViewWidget &other) Q_DECL_EQ_DELETE;

    QLabel *imgBox;
    QLabel *info;

    int index = 0;
};

#endif  // PICTUREVIEWWIDGET_H
