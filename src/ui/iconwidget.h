#ifndef ICONWIDGET_H
#define ICONWIDGET_H

#include <QCryptographicHash>
#include <QFileInfo>
#include <QImage>
#include <QWidget>

class QProgressBar;
class QGraphicsDropShadowEffect;
class QLabel;
class QHBoxLayout;
class QVBoxLayout;
class QPushButton;

class IconWidget : public QWidget {
    Q_OBJECT
   public:
    explicit IconWidget(QWidget *parent = nullptr);

    inline void setImage(const QByteArray &img);
    inline const QImage &image() const;

    inline void setImageData(const QByteArray &ba);
    inline QByteArray &imageData();

    inline void setImageInfo(QFileInfo fi);
    inline QFileInfo imageInfo() const;

    inline QString Hash() const;

    enum UPLOAD_STATUS { PENDING, UPLOADING, UPLOADED, FAILED };
    inline void setStatus(UPLOAD_STATUS newStatus);
    inline UPLOAD_STATUS status() const;

   signals:
    void onDeleteBtnClicked(IconWidget *);
    void onViewBtnClicked(IconWidget *);
    void onUploadBtnClicked(IconWidget *);
    void statusChanged(UPLOAD_STATUS newStatus);

   public slots:
    void onStatusChanged(UPLOAD_STATUS newStatus);
    void updateUploadProgress(qint64 bytesSent, qint64 bytesTotal);
    void addImageFromFile(const QString &fileName, const QByteArray &fileData);

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

    UPLOAD_STATUS m_status = PENDING;

    bool hover = false;
    QGraphicsDropShadowEffect *shadow;
    QVBoxLayout *infoBox;
    QHBoxLayout *bottomLine;
    QLabel *statusLine;
    QProgressBar *progress;
    QPushButton *uploadBtn;

    const int INFO_SPACE = 164;
};

inline void IconWidget::setImage(const QByteArray &img) {
    pix = QImage::fromData(img);
    setImageData(img);
    thumb = pix.scaled(width(), height() - INFO_SPACE, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    update();
}

inline const QImage &IconWidget::image() const {
    return pix;
}

inline void IconWidget::setImageData(const QByteArray &ba) {
    data = ba;
}

inline QByteArray &IconWidget::imageData() {
    return data;
}

inline void IconWidget::setImageInfo(QFileInfo fi) {
    info = fi;
}

inline QFileInfo IconWidget::imageInfo() const {
    return info;
}

inline void IconWidget::setStatus(IconWidget::UPLOAD_STATUS newStatus) {
    emit statusChanged(newStatus);
}

inline IconWidget::UPLOAD_STATUS IconWidget::status() const {
    return m_status;
}

#endif  // ICONWIDGET_H
