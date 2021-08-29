#ifndef IMGEHOST_H
#define IMGEHOST_H

#include <QObject>
#include <QWidget>

#include "httpclient.h"

class ImgeHost : public QObject {
    Q_OBJECT
   public:
    ImgeHost(QObject *parent = nullptr);
};

#endif  // IMGEHOST_H
