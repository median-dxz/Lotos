#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <QFrame>

class QPushButton;
class QLabel;
class QTimeLine;
class QDialogButtonBox;
class QGridLayout;

class MessageBox : public QFrame {
   public:
    MessageBox(QWidget *parent);

    enum IconType { INFO, WARN, ERROR, WAIT, SUCCESS };
    IconType icontype() const;
    void setIcontype(const IconType &icontype);

    QDialogButtonBox *dialogButtonBox() const;
    void setDialogButtonBox(QDialogButtonBox *dialogButtonBox);

    QGridLayout *mainArea() const;
    void setMainArea(QGridLayout *mainArea);

   private:
    QLabel *m_iconArea;
    QGridLayout *m_mainArea;
    QDialogButtonBox *m_dialogButtonBox;
    IconType m_icontype;

    QTimeLine *animation;

    const static int d = 160;

    static void gradientArc(QPainter *painter,
                            int radius,
                            int startAngle,
                            int angleLength,
                            int arcHeight,
                            QColor color);
    void painting(int k, QPainter *painter);

   private slots:
    void paintWai(QPainter *painter, int frame);
    void paintErr(QPainter *painter, int frame);
    void paintSuc(QPainter *painter, int frame);
    void paintInf(QPainter *painter, int frame);
    void paintWar(QPainter *painter, int frame);

    void paintIcon(int frame);
};

#endif  // MESSAGEBOX_H
