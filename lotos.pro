QT       += core gui network widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

VPATH += src \

INCLUDEPATH += src \

SOURCES += \
    main.cpp \
    mainwindow.cpp \
<<<<<<< HEAD
<<<<<<< HEAD
    ImgButton.cpp \
    utils/HttpClient.cpp

HEADERS += \
    mainwindow.h \
    ImgButton.h \
    utils/HttpClient.h
=======
    imgbutton.cpp \
=======
>>>>>>> 2920bae (refactor: 删除ImgaeButton实现)
    pagebutton.cpp \
    utils/imgehost.cpp \
    utils/httpclient.cpp

HEADERS += \
    mainwindow.h \
    pagebutton.h \
    utils/imgehost.h \
    utils/httpclient.h
>>>>>>> e2619e0 (sync: 修复与整理阶段性工作，修复之前阶段性工作提交的问题)

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

