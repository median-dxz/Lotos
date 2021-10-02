#include "lotoshelper.h"

#include <QApplication>
#include <QFile>
#include <QPainter>
#include <QPropertyAnimation>

#include <cmath>
#include <utility>

QString LotosHelper::getElidedText(QFont font, QString str, int maxWidth) {
    QStringList list = str.split('\n', QString::SkipEmptyParts);
    QFontMetrics fontWidth(font);
    for (int i = 0; i < list.size(); i++)
        list[i] = fontWidth.elidedText(list[i], Qt::ElideRight, maxWidth);
    str = list.join('\n');
    return str;
}

QString LotosHelper::formatFileSize(qint64 size) {
    int type = int(log2(size) / 10);
    QString sizeUnit;
    switch (type) {
        case 0:
            sizeUnit = "B";
            break;
        case 1:
            sizeUnit = "KB";
            break;
        case 2:
            sizeUnit = "MB";
            break;
        default:
            break;
    }

    return QString::number(size / pow(2, int(log2(size)) / 10 * 10), 'f', 2) + sizeUnit;
}

QString LotosHelper::formatExternalLink(QString filename, QString url, ExternalLinkType type) {
    const QString &BBCode = "[URL=%1][IMG]%1[/IMG][/URL]";
    const QString &Html = "<a href=\"%url%\" target=\"_blank\"><img src=\"%url%\" alt=\"%filename%\"></a>";
    const QString &Markdown = "![%2](%1)";
    const QString &Url = "%1";
    switch (type) {
        case ExternalLinkType::BBCode:
            return BBCode.arg(url, filename);
        case ExternalLinkType::HTML:
            return Html.arg(url, filename);
        case ExternalLinkType::Markdown:
            return Markdown.arg(url, filename);
        case ExternalLinkType::URL:
            return Url.arg(url, filename);
    }
    return "";
}

bool LotosHelper::loadQStyleSheet(const QString &fileName) {
    QFile qssFile(fileName);
    QString stylesheet = qApp->styleSheet();
    if (qssFile.open(QFile::ReadOnly)) {
        stylesheet += qssFile.readAll();
        qApp->setStyleSheet(stylesheet);
        qssFile.close();
        return true;
    } else {
        return false;
    }
}

QPropertyAnimation *LotosAnimation::fade(QGraphicsEffect *effect, QWidget *parent, int duration, bool direction) {
    QPropertyAnimation *animation = new QPropertyAnimation(effect, "opacity", parent);
    animation->setEasingCurve(QEasingCurve(QEasingCurve::InOutQuad));
    animation->setDuration(duration);

    if (direction) {
        animation->setStartValue(0);
        animation->setEndValue(0.99);
    } else {
        animation->setStartValue(0.99);
        animation->setEndValue(0);
    }
    return animation;
}

QPropertyAnimation *LotosAnimation::alphaGradient(QObject *obj,
                                                  const QByteArray &name,
                                                  QColor c,
                                                  qreal alpha1,
                                                  qreal alpha2,
                                                  QWidget *parent,
                                                  int duration,
                                                  bool direction) {
    QPropertyAnimation *animation = new QPropertyAnimation(obj, name, parent);
    animation->setEasingCurve(QEasingCurve(QEasingCurve::InOutQuad));
    animation->setDuration(duration);
    if (!direction) {
        std::swap(alpha1, alpha2);
    }
    c.setAlphaF(alpha1);
    animation->setStartValue(c);
    c.setAlphaF(alpha2);
    animation->setEndValue(c);
    return animation;
}
void LotosAnimation::shadowGenerator(QGraphicsDropShadowEffect *s, QColor c, int xOffset, int yOffset, int blurRadius) {
    s->setOffset(xOffset, yOffset);
    s->setBlurRadius(blurRadius);
    s->setColor(c);
}

void LotosAnimation::shadowGenerator(QGraphicsDropShadowEffect *s,
                                     qreal alphaF,
                                     int xOffset,
                                     int yOffset,
                                     int blurRadius) {
    s->setOffset(xOffset, yOffset);
    s->setBlurRadius(blurRadius);
    QColor c(0, 0, 0);
    c.setAlphaF(alphaF);
    s->setColor(c);
}

OpacityWithShadowEffectsGroup::OpacityWithShadowEffectsGroup(QObject *parent) : QGraphicsDropShadowEffect(parent) {}

void OpacityWithShadowEffectsGroup::draw(QPainter *painter) {
    if (blurRadius() <= 0 && offset().isNull()) {
        drawSource(painter);
        return;
    }

    PixmapPadMode mode = PadToEffectiveBoundingRect;

    // Draw pixmap in device coordinates to avoid pixmap scaling.
    QPoint pos;
    const QPixmap pixmap = sourcePixmap(Qt::DeviceCoordinates, &pos, mode);
    if (pixmap.isNull())
        return;

    QTransform restoreTransform = painter->worldTransform();
    painter->setWorldTransform(QTransform());
    painter->setRenderHint(QPainter::HighQualityAntialiasing);

    const QPixmap &px = pixmap;

    if (px.isNull())
        return;

    QImage tmp(px.size(), QImage::Format_ARGB32_Premultiplied);
    tmp.setDevicePixelRatio(px.devicePixelRatio());
    tmp.fill(0);

    QPainter tmpPainter;

    tmpPainter.begin(&tmp);
    tmpPainter.setCompositionMode(QPainter::CompositionMode_Source);
    tmpPainter.setRenderHint(QPainter::HighQualityAntialiasing);
    tmpPainter.drawPixmap(offset(), px);
    tmpPainter.end();

    // blur the alpha channel
    QImage blurred(tmp.size(), QImage::Format_ARGB32_Premultiplied);
    blurred.setDevicePixelRatio(px.devicePixelRatio());
    blurred.fill(0);
    QPainter blurPainter(&blurred);
    qt_blurImage(&blurPainter, tmp, blurRadius(), false, true);
    blurPainter.end();

    tmp = std::move(blurred);

    // blacken the image...
    tmpPainter.begin(&tmp);
    tmpPainter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    tmpPainter.fillRect(tmp.rect(), color());
    tmpPainter.end();

    QImage opaque(px.size(), QImage::Format_ARGB32_Premultiplied);
    opaque.setDevicePixelRatio(px.devicePixelRatio());
    opaque.fill(0);
    QPainter opaquePainter(&opaque);
    opaquePainter.setRenderHint(QPainter::HighQualityAntialiasing);
    // draw the blurred drop shadow...
    if (m_shadowRepaintNeeded || m_shadowBuffer.isNull()) {
        opaquePainter.drawImage(QPointF(0, 0), tmp);
        m_shadowBuffer = std::move(tmp);
    } else {
        opaquePainter.drawImage(QPointF(0, 0), m_shadowBuffer);
    }
    // draw the actual pixmap...
    opaquePainter.drawPixmap(QPointF(0, 0), px);
    opaquePainter.end();

    tmp = QImage(opaque.size(), QImage::Format_ARGB32_Premultiplied);
    tmp.fill(Qt::transparent);

    tmpPainter.begin(&tmp);
    tmpPainter.setCompositionMode(QPainter::CompositionMode_Source);
    tmpPainter.setRenderHint(QPainter::HighQualityAntialiasing);
    tmpPainter.drawImage(QPointF(0, 0), opaque);
    tmpPainter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    tmpPainter.fillRect(tmp.rect(), QColor(0, 0, 0, 255 * m_opacity));
    tmpPainter.end();

    painter->drawImage(pos, tmp);

    painter->setWorldTransform(restoreTransform);
    m_shadowRepaintNeeded = true;
}

using namespace LotosHelper;
StyleClass::_Type StyleClass::_type;
