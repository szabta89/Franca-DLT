#ifndef TRACEELEMENTRESPONSEPROCESSOR_H
#define TRACEELEMENTRESPONSEPROCESSOR_H

#include <QObject>
#include <QDebug>
#include <QThread>
#include <QStringList>
#include <QWaitCondition>
#include <QMutex>
#include "contractvalidatorplugin.h"

class TraceElementResponseProcessor : public QThread
{
    Q_OBJECT
public:
    TraceElementResponseProcessor(ContractValidatorPlugin *_plugin, QObject *parent = 0);
    ~TraceElementResponseProcessor();
    void add(QString data);
    QWaitCondition waitCondition;
    QMutex lock;
    void interruptThread();
protected:
    void run();
private:
    QStringList queue;
    Json json;
    volatile bool interrupted;
    ContractValidatorPlugin* plugin;
};

#endif // TRACEELEMENTRESPONSEPROCESSOR_H
