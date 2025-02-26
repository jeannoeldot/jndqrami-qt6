#-------------------------------------------------
#
# Project created by QtCreator 2014-07-20T21:28:21
#
#-------------------------------------------------
isEmpty( PREFIX ){
 PREFIX =  /usr
}

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app

CONFIG += qt \
    warn_on

DEFINES += PREFIX=\"$${PREFIX}\"

TARGET = jndqrami

SOURCES += main.cpp\
        cmainwindow.cpp \
    chelpbrowser.cpp \
    cmeme.cpp \
    cresultat.cpp \
    csuite.cpp \
    sdlplayer.cpp \
    crami.cpp \
    cafficheurimage.cpp \
    cbasejoueurs.cpp \
    ctricher.cpp \
    cvaleurjeu.cpp \
    cpreferences.cpp \
    cstatistique.cpp

HEADERS  += cmainwindow.h \
    constantes.h \
    miseaupoint.h \
    chelpbrowser.h \
    cmeme.h \
    cresultat.h \
    cretourpose.h \
    csuite.h \
    cvaleurcarte.h \
    sdlplayer.h \
    crami.h \
    cafficheurimage.h \
    cbasejoueurs.h \
    ctricher.h \
    cvaleurjeu.h \
    cpreferences.h \
    cstatistique.h

RESOURCES += \
    jndqrami.qrc

FORMS += \
    DlgInsererCartes.ui \
    DlgPreferences.ui

LIBS += -lSDL_mixer -lSDL

OTHER_FILES += \
    COPYING \
    desktop/jndqrami.desktop \
    desktop/jndqrami.png

unix:!mac {
    binary.path += $${PREFIX}/bin/
    binary.files += jndqrami
    pixmap.path +=  $${PREFIX}/share/pixmaps/
    pixmap.files +=  desktop/jndqrami.png
    desktop.path +=  $${PREFIX}/share/applications/
    desktop.files +=  desktop/jndqrami.desktop
    INSTALLS += binary \
                pixmap \
                desktop
}

DISTFILES += \
    README
