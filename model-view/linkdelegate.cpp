#include "linkdelegate.h"

class LinkDelegateData : public QSharedData
{
public:

};

LinkDelegate::LinkDelegate() : data(new LinkDelegateData)
{

}

LinkDelegate::LinkDelegate(const LinkDelegate &rhs) : data(rhs.data)
{

}

LinkDelegate &LinkDelegate::operator=(const LinkDelegate &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

LinkDelegate::~LinkDelegate()
{

}
