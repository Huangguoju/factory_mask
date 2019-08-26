#-------------------------------------------------
#
# Project created by QtCreator 2019-07-15T13:48:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += qml quick
# 网络请求
QT += network

TARGET = factory_mask
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# 软件exe图标
RC_FILE = softwarePicture.rc

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        aboutusdialog.cpp \
    updatedialog.cpp \
    ftpmanager.cpp \
    useuserinfo.cpp

HEADERS += \
        mainwindow.h \
        aboutusdialog.h \
    updatedialog.h \
    ftpmanager.h \
    useuserinfo.h

FORMS += \
        mainwindow.ui \
        aboutusdialog.ui \
    updatedialog.ui

RESOURCES += \
    images.qrc

OTHER_FILES  += \
                Block.qml \
                Block.qml \
                Dialog.qml \
                frame.qml \
                samegame.qml \
                pics/*.png
