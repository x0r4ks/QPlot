QT       += core gui printsupport

win32-g++ {
LIBS += -static-libgcc \
        -static-libstdc++ \
        -lmuparser
}

unix {
LIBS += -ldl
}


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17



# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
win32 {
SOURCES += \
    graph_vect.cpp \
    main.cpp \
    qcustomplot.cpp \
    qplot.cpp \
    editdialog.cpp \
    editror.cpp

}

unix {
SOURCES += \
    graph_vect.cpp \
    main.cpp \
    qcustomplot.cpp \
    qplot.cpp \
    muparser/muParser.cpp \
    muparser/muParserBase.cpp \
    muparser/muParserBytecode.cpp \
    muparser/muParserCallback.cpp \
    muparser/muParserDLL.cpp \
    muparser/muParserError.cpp \
    muparser/muParserInt.cpp \
    muparser/muParserTest.cpp \
    muparser/muParserTokenReader.cpp \
    editdialog.cpp \
    editror.cpp

}

win32 {
HEADERS += \
    graph_vect.h \
    qcustomplot.h \
    qplot.h \
    editdialog.h \
    editror.h

}

unix {
HEADERS += \
    graph_vect.h \
    muparser/muParser.h \
    muparser/muParserBase.h \
    muparser/muParserBytecode.h \
    muparser/muParserCallback.h \
    muparser/muParserDLL.h \
    muparser/muParserDef.h \
    muparser/muParserError.h \
    muparser/muParserFixes.h \
    muparser/muParserInt.h \
    muparser/muParserTemplateMagic.h \
    muparser/muParserTest.h \
    muparser/muParserToken.h \
    muparser/muParserTokenReader.h \
    qcustomplot.h \
    qplot.h \
    editdialog.h \
    editror.h
}

FORMS += \
    editdialog.ui \
    editror.ui \
    promrules.ui \
    qplot.ui \
    rootsforms.ui

TRANSLATIONS += \
    QPlot_en_US.ts \
    QPlot_ru_RU.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    libmuParse.so \
    muParser.dll

RESOURCES += resources.qrc

HEADERS += \
    formulelement.h \
    promrules.h \
    rootsforms.h

SOURCES += \
    formulelement.cpp \
    promrules.cpp \
    rootsforms.cpp



