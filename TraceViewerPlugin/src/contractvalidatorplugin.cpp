#include "contractvalidatorplugin.h"

ContractValidatorPlugin::ContractValidatorPlugin()
{
    dltControl = 0;
    messageCounter = 1;

    this->traceElementResponseProcessor = new TraceElementResponseProcessor(this);
    this->traceElementResponseProcessor->start();
    this->traceValidatorServer = new TraceValidatorServer(this);
    this->traceValidatorServer->start();
    this->traceValidatorClient = new TraceValidatorClient(this);
    this->traceValidatorClient->connect();
}

ContractValidatorPlugin::~ContractValidatorPlugin()
{
    this->traceValidatorClient->disconnect();
    this->traceValidatorServer->interruptThread();
    this->traceElementResponseProcessor->interruptThread();

    delete traceValidatorClient;
    delete traceValidatorServer;
    delete traceElementResponseProcessor;
}

QString ContractValidatorPlugin::name()
{
    return QString("Contract validator");
}

QString ContractValidatorPlugin::pluginVersion(){
    return PLUGIN_VERSION;
}

QString ContractValidatorPlugin::pluginInterfaceVersion(){
    return PLUGIN_INTERFACE_VERSION;
}

QString ContractValidatorPlugin::description()
{
    return QString();
}

QString ContractValidatorPlugin::error()
{
    return QString();
}

bool ContractValidatorPlugin::loadConfig(QString /* filename */)
{
    return true;
}

bool ContractValidatorPlugin::saveConfig(QString /* filename */)
{
    return true;
}

QStringList ContractValidatorPlugin::infoConfig()
{
    return QStringList();
}

QWidget* ContractValidatorPlugin::initViewer()
{
    form = new Form(this);
    return form;
}

void ContractValidatorPlugin::selectedIdxMsg(int , QDltMsg &) {

}

void ContractValidatorPlugin::selectedIdxMsgDecoded(int , QDltMsg &){

}

void ContractValidatorPlugin::initFileStart(QDltFile*){

}

void ContractValidatorPlugin::initMsg(int , QDltMsg &){

}

void ContractValidatorPlugin::initMsgDecoded(int , QDltMsg &){

}

void ContractValidatorPlugin::initFileFinish(){

}

void ContractValidatorPlugin::updateFileStart(){

}

void ContractValidatorPlugin::updateMsg(int, QDltMsg &msg){
    QString payload = msg.toStringPayload();

    if (payload.startsWith("respond_", Qt::CaseSensitive) ||
        payload.startsWith("call_", Qt::CaseSensitive) ||
        payload.startsWith("update_", Qt::CaseSensitive)) {

        this->contextElementsLock.lock();
        if (!this->contextElements.keys().contains(msg.getCtid())) {
            ContextElement* ctx = new ContextElement(msg.getCtid());
            this->contextElements.insert(msg.getCtid(), ctx);
            this->form->addContext(msg.getCtid());
        }

        this->contextElements[msg.getCtid()]->messages->insert(messageCounter, msg);
        this->contextElements[msg.getCtid()]->unsentMessages->append(messageCounter);
        messageCounter++;
        this->contextElementsLock.unlock();
    }

    form->setMessages();
    trySendMessages();
}

// this will send the messages in the order they have arrived
void ContractValidatorPlugin::trySendMessages() {
    this->contextElementsLock.lock();
    for (int i = 0;i<this->contextElements.values().size();i++) {
        ContextElement* element = this->contextElements.values().at(i);
        if (!element->filePath.isNull()) {
            while (!element->unsentMessages->isEmpty()) {
                int messageId = element->unsentMessages->at(0);

                QMap<QString, QVariant> message;

                message["messageId"] = messageId;

                QDltMsg m = element->messages->value(messageId);
                message["traceElement"] = m.toStringPayload();

                message["filePath"] = element->filePath;

                message["contextId"] = element->contextId;

                QString encoded = json.encode(message);
                bool result = this->traceValidatorClient->send(encoded);
                if (!result) {
                    // do not try it anymore
                    this->contextElementsLock.unlock();
                    return;
                }
                else {
                    element->unsentMessages->removeAt(0);
                }
            }
        }
    }
    this->contextElementsLock.unlock();
}

void ContractValidatorPlugin::updateMsgDecoded(int, QDltMsg&) {
    //updateMsg(0, msg);
}

void ContractValidatorPlugin::updateFileFinish(){

}

#ifndef QT5
Q_EXPORT_PLUGIN2(ContractValidatorPlugin, ContractValidatorPlugin)
#endif
