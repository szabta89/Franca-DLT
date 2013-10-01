# Important: the pro file assumes that both this project's parent (Franca-DLT)
# and the dlt-viewer project are checked out into the same folder (on the same level)
include( ../../dlt-viewer/plugin/plugin.pri )

HEADERS += \
    src/form.h \
    src/form.h \
    src/contextelement.h \
    src/tracevalidatorclient.h \
    src/json.h \
    src/tracevalidatorserver.h \
    src/params.h \
    src/contractvalidatorplugin.h \
    src/traceelementresponseprocessor.h

SOURCES += \
    src/form.cpp \
    src/contextelement.cpp \
    src/tracevalidatorclient.cpp \
    src/json.cpp \
    src/tracevalidatorserver.cpp \
    src/contractvalidatorplugin.cpp \
    src/traceelementresponseprocessor.cpp

TARGET = $$qtLibraryTarget(traceviewerplugin)

FORMS += \
    src/form.ui

QT += network \
      script

INCLUDEPATH += ../../dlt-viewer/src \
            ../../dlt-viewer/qdlt

CONFIG(debug, debug|release) {
    DESTDIR = debug
}
else {
    DESTDIR = release
}
