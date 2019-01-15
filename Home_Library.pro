#-------------------------------------------------
#
# Project created by QtCreator 2019-01-13T21:38:49
#
#-------------------------------------------------

QTEMPLATE = app
INCLUDEPATH += .

HEADERS     = bookdelegate.h bookwindow.h \
    initdb_.h
RESOURCES   = books.qrc \
    books.qrc
SOURCES     = bookdelegate.cpp main.cpp bookwindow.cpp
FORMS       = \
    bookwindow.ui

QT += sql widgets widgets

target.path = $$[QT_INSTALL_EXAMPLES]/sql/books
INSTALLS += target
