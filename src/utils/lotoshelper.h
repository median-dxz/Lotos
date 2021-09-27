#ifndef LOTOSHELPER_H
#define LOTOSHELPER_H

#include <QDateTime>
#include <QGraphicsEffect>
#include <QWidget>

namespace LotosHelper {

QString getElidedText(QFont font, QString str, int MaxWidth);

inline QString timestamp2str(qint64 time, QString format) {
    QDateTime time_ = QDateTime::fromTime_t(time);
    return time_.toString(format);
};

QString formatFileSize(qint64 size);

enum ExternalLinkType { BBCode, HTML, Markdown, URL };
QString formatExternalLink(QString filename, QString url, ExternalLinkType type = URL);

}  // namespace LotosHelper

namespace LotosAnimation {

QPropertyAnimation *fade(QGraphicsEffect *, QWidget *, bool direction = true, int duration = 100);

}

Q_DECL_IMPORT void qt_blurImage(QPainter *p,
                                QImage &blurImage,
                                qreal radius,
                                bool quality,
                                bool alphaOnly,
                                int transposed = 0);

class OpacityWithShadowEffectsGroup : public QGraphicsDropShadowEffect {
    Q_OBJECT
   signals:
    void opacityChanged(const qreal &newOpacity);

   public:
    OpacityWithShadowEffectsGroup(QObject *parent = 0);

    Q_PROPERTY(qreal opacity MEMBER m_color READ opacity WRITE setOpacity NOTIFY opacityChanged)
    qreal opacity() const;
    inline void setOpacity(qreal opacity);

   protected:
    void draw(QPainter *painter) override;

   private:
    qreal m_opacity = 1;
};

inline qreal OpacityWithShadowEffectsGroup::opacity() const {
    return m_opacity;
}

inline void OpacityWithShadowEffectsGroup::setOpacity(qreal opacity) {
    m_opacity = opacity;
    updateBoundingRect();
}

#endif  // LOTOSHELPER_H
