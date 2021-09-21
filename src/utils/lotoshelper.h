#ifndef LOTOSHELPER_H
#define LOTOSHELPER_H

#include <QDateTime>
#include <QFont>
#include <QFontMetrics>
#include <QString>

QString getElidedText(QFont font, QString str, int MaxWidth);
QString timestamp2str(qint64 time, QString format);

#endif  // LOTOSHELPER_H
