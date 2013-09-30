#include <QtGui>

#include "traceviewerplugin.h"

TraceViewerPlugin::TraceViewerPlugin()
{
    dltFile = 0;
    dltControl = 0;
    messageCounter = 1;

    this->traceValidatorServer = new TraceValidatorServer(this);
    this->traceValidatorClient = new TraceValidatorClient(this);
    this->traceValidatorClient->connect();
}

TraceViewerPlugin::~TraceViewerPlugin()
{
    this->traceValidatorClient->disconnect();
    delete traceValidatorClient;
    delete traceValidatorServer;
    delete dltFile;
}

QString TraceViewerPlugin::name()
{
    return QString("Trace Viewer Plugin");
}

QString TraceViewerPlugin::pluginVersion(){
    return PLUGIN_VERSION;
}

QString TraceViewerPlugin::pluginInterfaceVersion(){
    return PLUGIN_INTERFACE_VERSION;
}

QString TraceViewerPlugin::description()
{
    return QString();
}

QString TraceViewerPlugin::error()
{
    return QString();
}

bool TraceViewerPlugin::loadConfig(QString /* filename */)
{
    return true;
}

bool TraceViewerPlugin::saveConfig(QString /* filename */)
{
    return true;
}

QStringList TraceViewerPlugin::infoConfig()
{
    return QStringList();
}

QWidget* TraceViewerPlugin::initViewer()
{
    form = new Form(this);
    return form;
}

bool TraceViewerPlugin::initControl(QDltControl *control)
{
    dltControl = control;
    return true;
}

bool TraceViewerPlugin::initConnections(QStringList)
{
    return true;
}

bool TraceViewerPlugin::controlMsg(int , QDltMsg&)
{
    return false;
}

bool TraceViewerPlugin::stateChanged(int, QDltConnection::QDltConnectionState)
{
    return true;
}

void TraceViewerPlugin::selectedIdxMsg(int , QDltMsg &) {

}

void TraceViewerPlugin::selectedIdxMsgDecoded(int , QDltMsg &){

}

void TraceViewerPlugin::initFileStart(QDltFile *file){
    dltFile = file;
}

void TraceViewerPlugin::initMsg(int , QDltMsg &){

}

void TraceViewerPlugin::initMsgDecoded(int , QDltMsg &){

}

void TraceViewerPlugin::initFileFinish(){

}

void TraceViewerPlugin::updateFileStart(){

}

void TraceViewerPlugin::updateMsg(int, QDltMsg &msg){
    this->contextElementsLock.lock();
    if (!this->contextElements.keys().contains(msg.getCtid())) {
        ContextElement* ctx = new ContextElement(msg.getCtid());
        this->contextElements.insert(msg.getCtid(), ctx);
        this->form->addContext(msg.getCtid());
    }

    this->contextElements[msg.getCtid()]->messages.insert(messageCounter, msg);
    this->contextElements[msg.getCtid()]->unsentMessages->append(messageCounter);
    messageCounter++;

    trySendMessages();
    this->contextElementsLock.unlock();

    form->setMessages();
}

// this will send the messages in the order they have arrived
void TraceViewerPlugin::trySendMessages() {
    for (int i = 0;i<this->contextElements.values().size();i++) {
        ContextElement* element = this->contextElements.values().at(i);
        if (!element->filePath.isNull()) {
            while (!element->unsentMessages->isEmpty()) {
                int messageId = element->unsentMessages->at(0);

                QMap<QString, QVariant> message;
                message["messageId"] = messageId;
                message["traceElement"] = element->messages[messageId].toStringPayload();
                message["filePath"] = element->filePath;
                message["contextId"] = element->contextId;
                QString encoded = json.encode(message);
                bool result = this->traceValidatorClient->send(encoded);
                if (!result) {
                    // do not try it anymore
                    return;
                }
                else {
                    element->unsentMessages->removeAt(0);
                }
            }
        }
    }
}

void TraceViewerPlugin::updateMsgDecoded(int, QDltMsg&) {
    //updateMsg(0, msg);
}

void TraceViewerPlugin::updateFileFinish(){

}

#ifndef QT5
Q_EXPORT_PLUGIN2(traceviewerplugin, TraceViewerPlugin)
#endif
