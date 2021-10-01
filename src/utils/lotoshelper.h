#ifndef LOTOSHELPER_H
#define LOTOSHELPER_H

#include <QWidget>

namespace LotosHelper {

#define timestamp2str(time, format) (QDateTime::fromTime_t(time).toString(format))

QString getElidedText(QFont font, QString str, int MaxWidth);
QString formatFileSize(qint64 size);

enum ExternalLinkType { BBCode, HTML, Markdown, URL };
QString formatExternalLink(QString filename, QString url, ExternalLinkType type = URL);

bool loadQStyleSheet(const QString &fileName);

}  // namespace LotosHelper

namespace LotosAnimation {

QPropertyAnimation *fade(QGraphicsEffect *, QWidget *, bool direction = true, int duration = 100);

}  // namespace LotosAnimation

namespace LotosAsync {

// scaledImageAsync;
// readFileAsync;
// loadImageFromDataAsync;

}  // namespace LotosAsync

#include <QGraphicsEffect>

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
    bool m_shadowRepaintNeeded = true;
    QImage m_shadowBuffer;
};

inline qreal OpacityWithShadowEffectsGroup::opacity() const {
    return m_opacity;
}

inline void OpacityWithShadowEffectsGroup::setOpacity(qreal opacity) {
    m_opacity = opacity;
    m_shadowRepaintNeeded = false;
    updateBoundingRect();
}

#endif  // LOTOSHELPER_H
