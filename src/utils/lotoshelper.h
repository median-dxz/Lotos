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

class StyleClass {
   public:
    static class _Type {
       public:
        struct buttonType {
            const QString danger = "button.danger";
            const QString primary = "button.primary";
            const QString normal = "button.normal";
        } button;

        struct progressbarType {
            const QString normal = "progressbar.normal";
        } progressbar;

        const char *name{"style_type"};
    } _type;

   private:
    StyleClass();
};

constexpr static StyleClass::_Type &StyleType = StyleClass::_type;
}  // namespace LotosHelper

class QPropertyAnimation;
namespace LotosAnimation {

QPropertyAnimation *fade(QGraphicsEffect *, QWidget *, int duration = 100, bool direction = true);
QPropertyAnimation *alphaGradient(QObject *obj,
                                  const QByteArray &name,
                                  QColor c,
                                  qreal alpha1,
                                  qreal alpha2,
                                  QWidget *parent,
                                  int duration = 200,
                                  bool direction = true);

}  // namespace LotosAnimation

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
