#include "lotoshelper.h"

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
