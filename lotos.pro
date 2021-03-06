QT       += core gui widgets network concurrent

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

VPATH += src \

INCLUDEPATH += src \
               src/ui \

FORMS += \
    mainwindow.ui \
    src/dialogabout.ui

RC_ICONS = icon.ico

VERSION = 0.1.0
TARGET = Lotos
win32: QMAKE_TARGET_DESCRIPTION = "a modern Image Upload clinet"
win32: QMAKE_TARGET_PRODUCT = "Lotos"

DEFINES += \
    LOTOS_VERSION=\"\\\"$${VERSION}\\\"\"

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

CONFIG += windows

HEADERS += \
    src/dialogabout.h \
    src/mainwindow.h \
    src/settingshelper.h \
    src/ui/iconwidget.h \
    src/ui/linkcopybox.h \
    src/ui/messagebox.h \
    src/ui/networkresponsebox.h \
    src/ui/notification.h \
    src/ui/pagebutton.h \
    src/ui/picturescontainer.h \
    src/ui/picturetable.h \
    src/ui/pictureviewwidget.h \
    src/ui/titlebar.h \
    src/utils/httpclient.h \
    src/utils/imagehost.h \
    src/utils/lotoshelper.h \
    src/utils/promise.h

SOURCES += \
    src/dialogabout.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/settingshelper.cpp \
    src/ui/iconwidget.cpp \
    src/ui/linkcopybox.cpp \
    src/ui/messagebox.cpp \
    src/ui/networkresponsebox.cpp \
    src/ui/notification.cpp \
    src/ui/pagebutton.cpp \
    src/ui/picturescontainer.cpp \
    src/ui/picturetable.cpp \
    src/ui/pictureviewwidget.cpp \
    src/ui/titlebar.cpp \
    src/utils/httpclient.cpp \
    src/utils/imagehost.cpp \
    src/utils/lotoshelper.cpp
