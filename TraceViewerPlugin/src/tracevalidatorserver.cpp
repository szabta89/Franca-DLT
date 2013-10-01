#include "tracevalidatorserver.h"

TraceValidatorServer::TraceValidatorServer(ContractValidatorPlugin* _plugin, QObject* parent) : QThread(parent)
{
    this->plugin = _plugin;
    this->interrupted = false;
    this->init();
}

TraceValidatorServer::~TraceValidatorServer()
{
    close(serverSocket);
}

void TraceValidatorServer::init()
{
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        qDebug() << "ERROR while creating socket\n";
        return;
    }

    bzero((char *) &serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(SERVER_PORT);

    if (bind(serverSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {
        serverSocket = -1;
        qDebug() << "ERROR while invoking bind\n";
        return;
    }

    // setting timout for input operations
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    if (setsockopt (serverSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0) {
        serverSocket = -1;
        qDebug() << "ERROR while setting socket timeout\n";
        return;
    }

    listen(serverSocket, 10);
    cli_len = sizeof(clientAddress);
}

void TraceValidatorServer::interruptThread()
{
    this->interrupted = true;
}

void TraceValidatorServer::run()
{
    // only start the loop if the initialization went fine
    if (serverSocket > 0) {
        while (!interrupted) {
            clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddress, &cli_len);
            if (clientSocket > 0) {
                char buffer[BUFFER_SIZE];
                bzero(buffer, BUFFER_SIZE);
                int n = read(clientSocket, buffer, BUFFER_SIZE-1);
                if (n > 0) {
                    this->plugin->traceElementResponseProcessor->add(QString::fromAscii(buffer));
                }
                close(clientSocket);
            }
        }
    }
}
