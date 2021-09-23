#ifndef LOTOSHELPER_H
#define LOTOSHELPER_H

#include <QDateTime>
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

using namespace LotosHelper;

#endif  // LOTOSHELPER_H
