include( ../../git/dlt-viewer/plugin/plugin.pri )

HEADERS += \
    src/traceviewerplugin.h \
    src/form.h \
    src/form.h \
    src/contextelement.h \
    src/tracevalidatorclient.h \
    src/json.h \
    src/tracevalidatorserver.h \
    src/params.h

SOURCES += \
    src/traceviewerplugin.cpp \
    src/form.cpp \
    src/contextelement.cpp \
    src/tracevalidatorclient.cpp \
    src/json.cpp \
    src/tracevalidatorserver.cpp

TARGET = $$qtLibraryTarget(traceviewerplugin)

FORMS += \
    src/form.ui

QT += network \
      script

INCLUDEPATH += ../../git/dlt-viewer/src \
            ../../git/dlt-viewer/qdlt

CONFIG(debug, debug|release) {
    DESTDIR = debug
}
else {
    DESTDIR = release
}
