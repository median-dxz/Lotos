#ifndef LINKDELEGATE_H
#define LINKDELEGATE_H

#include <QStyledItemDelegate>
#include <QObject>
#include <QSharedDataPointer>
#include <QWidget>

class LinkDelegateData;

class LinkDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    LinkDelegate();
    LinkDelegate(const LinkDelegate &);
    LinkDelegate &operator=(const LinkDelegate &);
    ~LinkDelegate();

private:
    QSharedDataPointer<LinkDelegateData> data;
};

#endif // LINKDELEGATE_H
