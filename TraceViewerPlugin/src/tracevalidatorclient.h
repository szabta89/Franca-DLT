#ifndef TRACEVALIDATORCLIENT_H
#define TRACEVALIDATORCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QString>
#include "params.h"
#include "traceviewerplugin.h"

class TraceValidatorClient : public QObject
{
    Q_OBJECT
public:
    explicit TraceValidatorClient(TraceViewerPlugin* _plugin, QObject *parent = 0);
    ~TraceValidatorClient();
    void connect();
    void disconnect();
public slots:
    bool send(QString data);
private:
    QTcpSocket* socket;
    TraceViewerPlugin* plugin;
};

#endif // TRACEVALIDATORCLIENT_H
