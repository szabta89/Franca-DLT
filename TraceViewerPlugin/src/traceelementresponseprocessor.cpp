#include "traceelementresponseprocessor.h"

TraceElementResponseProcessor::TraceElementResponseProcessor(ContractValidatorPlugin* _plugin, QObject* parent)
    : QThread(parent)
{
    this->plugin = _plugin;
    this->interrupted = false;
}

TraceElementResponseProcessor::~TraceElementResponseProcessor()
{

}

void TraceElementResponseProcessor::run()
{
    while (!interrupted) {
        QString data;
        this->lock.lock();
        if (this->queue.isEmpty()) {
            this->waitCondition.wait(&lock, 2000);
        }
        else {
            data = this->queue.at(0);
            this->queue.removeFirst();
        }
        this->lock.unlock();

        if (!data.isNull()) {
            qDebug() << "Processing response: " + data + "\n";

            QMap<QString, QVariant> decoded = json.decode(data);
            this->plugin->contextElementsLock.lock();

            QString contextId = decoded["contextId"].toString();
            int messageId = decoded["messageId"].toInt();
            int valid = decoded["valid"].toInt();
            QString expectation = decoded["data"].toString();

            ContextElement* element = this->plugin->contextElements[contextId];

            // set valid flag if it is unset yet, or it is not invalid
            if (element->valid == -1) {
                element->valid = valid;
            }
            else if (element->valid != 1) {
                element->valid = valid;
            }

            if (!expectation.isEmpty()) {
                element->expectations->insert(messageId, expectation);
            }

            element->status->insert(messageId, valid);

            qDebug() << decoded << "\n";

            this->plugin->contextElementsLock.unlock();
        }
    }
}

void TraceElementResponseProcessor::interruptThread()
{
    this->interrupted = true;
}

void TraceElementResponseProcessor::add(QString data)
{
    this->lock.lock();
    this->queue.append(data);
    this->waitCondition.wakeAll();
    this->lock.unlock();
}
