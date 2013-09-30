#include "tracevalidatorserver.h"

TraceValidatorServer::TraceValidatorServer(TraceViewerPlugin* _plugin, QObject* parent) : QObject(parent)
{
    this->plugin = _plugin;
    connect(&server, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
    server.listen(QHostAddress::LocalHost, SERVER_PORT);
}

TraceValidatorServer::~TraceValidatorServer()
{
    server.close();
}

void TraceValidatorServer::acceptConnection()
{
    client = server.nextPendingConnection();
    connect(client, SIGNAL(readyRead()), this, SLOT(read()));
}

void TraceValidatorServer::read()
{
    if (client != 0) {
        if (client->bytesAvailable() > 0) {
            char buffer[1024] = {0};
            client->read(buffer, client->bytesAvailable());
            QMap<QString, QVariant> decoded = json.decode(QString::fromAscii(buffer));
            this->plugin->contextElementsLock.lock();

            QString contextId = decoded["contextId"].toString();
            int valid = decoded["valid"].toInt();
            ContextElement* element = this->plugin->contextElements[contextId];

            // set failure for the first time
            if (element->valid == 0 && valid == 1) {
                int messageId = decoded["messageId"].toInt();
                QString expected = decoded["data"].toString();
                element->failedAt = messageId;
                element->failedAtExpectation = expected;
            }
            // set valid flag in both cases
            element->valid = valid;

            qDebug() << decoded << "\n";

            this->plugin->contextElementsLock.unlock();
        }
        client->close();
        //delete client;
    }

    this->plugin->form->setMessages();
}
