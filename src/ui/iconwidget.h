#ifndef ICONWIDGET_H
#define ICONWIDGET_H

#include <QBoxLayout>
#include <QCryptographicHash>
#include <QFile>
#include <QFileInfo>
#include <QFont>
#include <QGraphicsDropShadowEffect>
#include <QLabel>
#include <QPainter>
#include <QProgressBar>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QRect>
#include <QVariant>
#include <QWidget>

#include <cmath>

class IconWidget : public QWidget {
    Q_OBJECT
   public:
    explicit IconWidget(QWidget *parent = nullptr);

    void addImageFromFile(const QString &fileName);
    //    void addImageFromUrl();
    //    void setImageFromData();

    void setImage(QImage img) {
        pix = img;
        thumb = pix.scaled(width(), height() - INFO_SPACE, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }
    QImage image() const { return pix; }
    void setImageData(QByteArray ba) {
        data = ba;
        hash = QCryptographicHash::hash(ba, QCryptographicHash::Md5).toHex();
    }
    QByteArray &imageData() { return data; }
    void setImageInfo(QFileInfo fi) { info = fi; }
    QFileInfo imageInfo() const { return info; }
    QString Hash() const { return hash; }

    static QString sizeUnit(qint64 size);

    enum UPLOAD_STATUS { PENDING, UPLOADING, UPLOADED, FAILED };
    void setStatus(UPLOAD_STATUS newStatus) { emit statusChanged(newStatus); }
    UPLOAD_STATUS status() { return m_status; }
   signals:
    void onDeleteBtnClicked(IconWidget *);
    void onViewBtnClicked(IconWidget *);
    void onUploadBtnClicked(IconWidget *);
    void statusChanged(UPLOAD_STATUS newStatus);

   public slots:
    void onStatusChanged(UPLOAD_STATUS newStatus);
    void updateUploadProgress(qint64 bytesSent, qint64 bytesTotal);

   protected:
    void paintEvent(QPaintEvent *) override;
    void enterEvent(QEvent *e) override;
    void leaveEvent(QEvent *e) override;

   private:
    void setShadow();
    void setDeleteBtn();
    void setViewBtn();
    void setUploadBtn();
    void setInfo();

    QImage pix;
    QFileInfo info;
    QByteArray data;
    QImage thumb;
    QString hash;

    UPLOAD_STATUS m_status = PENDING;

    bool hover = false;
    QGraphicsDropShadowEffect *shadow;
    QVBoxLayout *infoBox;
    QHBoxLayout *bottomLine;
    QLabel *statusLine;
    QProgressBar *progress;

    const int INFO_SPACE = 164;
};

#endif  // ICONWIDGET_H
