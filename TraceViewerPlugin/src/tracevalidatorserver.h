#ifndef TRACEVALIDATORSERVER_H
#define TRACEVALIDATORSERVER_H

#include <QObject>
#include <QThread>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "params.h"
#include "json.h"
#include "contractvalidatorplugin.h"

class TraceValidatorServer: public QThread
{
    Q_OBJECT
public:
    explicit TraceValidatorServer(ContractValidatorPlugin* _plugin, QObject *parent = 0);
    ~TraceValidatorServer();
    void interruptThread();
private:
    int serverSocket, clientSocket, port;
    socklen_t cli_len;
    struct sockaddr_in serverAddress, clientAddress;
    ContractValidatorPlugin* plugin;
    volatile bool interrupted;
    void init();
protected:
    void run();
};

#endif // TRACEVALIDATORSERVER_H
