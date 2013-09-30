#ifndef TRACEVALIDATORSERVER_H
#define TRACEVALIDATORSERVER_H

#include <QtNetwork>
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <iostream>
#include "params.h"
#include "json.h"
#include "traceviewerplugin.h"

class TraceValidatorServer: public QObject
{
    Q_OBJECT
public:
    explicit TraceValidatorServer(TraceViewerPlugin* _plugin, QObject *parent = 0);
    ~TraceValidatorServer();
private:
    TraceViewerPlugin* plugin;
    QTcpServer server;
    QTcpSocket* client;
    Json json;
private slots:
    void acceptConnection();
    void read();
};

#endif // TRACEVALIDATORSERVER_H
