#include "lotoshelper.h"

#include <QPainter>

#include <QPropertyAnimation>
#include <cmath>

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

QPropertyAnimation *LotosAnimation::fade(QGraphicsEffect *effect, QWidget *parent, bool direction, int duration) {
    QPropertyAnimation *animation = new QPropertyAnimation(effect, "opacity", parent);
    animation->setEasingCurve(QEasingCurve(QEasingCurve::InOutQuad));
    animation->setDuration(duration);
    animation->setStartValue(0);
    animation->setEndValue(0.99);
    if (direction) {
        animation->setDirection(QAbstractAnimation::Forward);
    } else {
        animation->setDirection(QAbstractAnimation::Backward);
        animation->setCurrentTime(duration);
    }
    animation->start(QAbstractAnimation::DeleteWhenStopped);
    return animation;
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

    const QPixmap &px = pixmap;

    if (px.isNull())
        return;

    QImage tmp(px.size(), QImage::Format_ARGB32_Premultiplied);
    tmp.setDevicePixelRatio(px.devicePixelRatio());
    tmp.fill(0);

    QPainter tmpPainter(&tmp);
    tmpPainter.setRenderHint(QPainter::HighQualityAntialiasing);
    tmpPainter.setCompositionMode(QPainter::CompositionMode_Source);
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
    // draw the blurred drop shadow...
    opaquePainter.drawImage(QPointF(0, 0), tmp);

    // draw the actual pixmap...
    opaquePainter.drawPixmap(QPointF(0, 0), px);
    opaquePainter.end();

    tmp = QImage(opaque.size(), QImage::Format_ARGB32_Premultiplied);
    tmp.fill(Qt::transparent);

    tmpPainter.begin(&tmp);
    tmpPainter.setCompositionMode(QPainter::CompositionMode_Source);
    tmpPainter.drawImage(QPointF(0, 0), opaque);
    tmpPainter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    tmpPainter.fillRect(tmp.rect(), QColor(0, 0, 0, 255 * m_opacity));
    tmpPainter.end();

    painter->drawImage(pos, tmp);

    painter->setWorldTransform(restoreTransform);
}
