#include "tracevalidatorclient.h"

TraceValidatorClient::TraceValidatorClient(TraceViewerPlugin* _plugin, QObject *parent) : QObject(parent)
{
    this->plugin = _plugin;
    this->socket = new QTcpSocket(this);
}

TraceValidatorClient::~TraceValidatorClient()
{
    this->socket->close();
    delete this->socket;
}

bool TraceValidatorClient::send(QString data)
{  
    // check if the first write is successfull
    qint64 n = this->socket->write(data.toAscii());
    if (data.at(data.length()-1) != '\n') {
        this->socket->write("\n");
    }
    this->socket->flush();
    return (n != -1);
}

void TraceValidatorClient::connect()
{
    this->socket->connectToHost(CLIENT_HOST, CLIENT_PORT);
}

void TraceValidatorClient::disconnect() {
    this->socket->disconnectFromHost();
}
