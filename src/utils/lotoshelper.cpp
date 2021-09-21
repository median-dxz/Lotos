#include "lotoshelper.h"

QString getElidedText(QFont font, QString str, int maxWidth) {
    QStringList list = str.split('\n', QString::SkipEmptyParts);
    QFontMetrics fontWidth(font);
    for (int i = 0; i < list.size(); i++)
        list[i] = fontWidth.elidedText(list[i], Qt::ElideRight, maxWidth);
    str = list.join('\n');
    return str;
}

QString timestamp2str(qint64 time, QString format) {
    QDateTime time_ = QDateTime::fromTime_t(time);
    return time_.toString(format);
}
