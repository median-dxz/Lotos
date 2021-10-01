QT       += core gui network widgets concurrent

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

VPATH += src \

INCLUDEPATH += src \
               src/ui \

PRECOMPILED_HEADER += \
    base.h

FORMS += \
    mainwindow.ui

RC_ICONS = icon.ico

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

CONFIG += BUILD_X86 cmdline precompile_header

CONFIG(BUILD_X64, BUILD_X86|BUILD_X64) {
    message("X64 Build")
} else {
    message("X86 Build")
}


VERSION = 0.0.1

DEFINES += \
    APP_VERSION=\\\"0.0.1\\\"

HEADERS += \
    src/mainwindow.h \
    src/settingshelper.h \
    src/ui/iconwidget.h \
    src/ui/linkcopybox.h \
    src/ui/loginbox.h \
    src/ui/messagebox.h \
    src/ui/notification.h \
    src/ui/pagebutton.h \
    src/ui/picturescontainer.h \
    src/ui/pictureviewwidget.h \
    src/ui/titlebar.h \
    src/utils/httpclient.h \
    src/utils/imagehost.h \
    src/utils/lotoshelper.h \
    src/utils/promise.h

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/settingshelper.cpp \
    src/ui/iconwidget.cpp \
    src/ui/linkcopybox.cpp \
    src/ui/loginbox.cpp \
    src/ui/messagebox.cpp \
    src/ui/notification.cpp \
    src/ui/pagebutton.cpp \
    src/ui/picturescontainer.cpp \
    src/ui/pictureviewwidget.cpp \
    src/ui/titlebar.cpp \
    src/utils/httpclient.cpp \
    src/utils/imagehost.cpp \
    src/utils/lotoshelper.cpp

