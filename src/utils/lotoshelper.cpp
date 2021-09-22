#include "lotoshelper.h"

#include <cmath>

QString getElidedText(QFont font, QString str, int maxWidth) {
    QStringList list = str.split('\n', QString::SkipEmptyParts);
    QFontMetrics fontWidth(font);
    for (int i = 0; i < list.size(); i++)
        list[i] = fontWidth.elidedText(list[i], Qt::ElideRight, maxWidth);
    str = list.join('\n');
    return str;
}

QString formatFileSize(qint64 size) {
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
