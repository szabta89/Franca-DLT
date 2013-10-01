#ifndef TRACEVALIDATORCLIENT_H
#define TRACEVALIDATORCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QString>
#include "params.h"
#include "contractvalidatorplugin.h"

// The client is implemented with QT sockets
// becasue a name clash occurrs with connect if the
// sys/socket.h header is imported
class TraceValidatorClient : public QObject
{
    Q_OBJECT
public:
    explicit TraceValidatorClient(ContractValidatorPlugin* _plugin, QObject *parent = 0);
    ~TraceValidatorClient();
    void connect();
    void disconnect();
public slots:
    bool send(QString data);
private:
    QTcpSocket* socket;
    ContractValidatorPlugin* plugin;
};

#endif // TRACEVALIDATORCLIENT_H
