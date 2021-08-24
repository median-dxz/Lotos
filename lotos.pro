QT       += core gui network widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

VPATH += src \

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    utils/HttpClient.cpp

HEADERS += \
    mainwindow.h \
    utils/HttpClient.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

CONFIG += BUILD_X86

CONFIG(BUILD_X64, BUILD_X86|BUILD_X64) {
    message("X64 Build")
} else {
    message("X86 Build")
}

