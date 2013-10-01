#ifndef CONTRACTVALIDATORPLUGIN_H
#define CONTRACTVALIDATORPLUGIN_H

#include <QObject>
#include <QMap>
#include <QtGui>
#include "plugininterface.h"
#include "form.h"
#include "tracevalidatorclient.h"
#include "tracevalidatorserver.h"
#include "traceelementresponseprocessor.h"
#include "json.h"

#define PLUGIN_VERSION "1.0.0"

class TraceValidatorServer;
class TraceValidatorClient;
class TraceElementResponseProcessor;

class ContractValidatorPlugin : public QObject, QDLTPluginInterface, QDltPluginViewerInterface
{
    Q_OBJECT
    Q_INTERFACES(QDLTPluginInterface)
    Q_INTERFACES(QDltPluginViewerInterface)
#ifdef QT5
    Q_PLUGIN_METADATA(IID "org.franca.tools.contracts.ContractValidatorPlugin")
#endif

public:
    ContractValidatorPlugin();
    ~ContractValidatorPlugin();

    /* QDLTPluginInterface interface */
    QString name();
    QString pluginVersion();
    QString pluginInterfaceVersion();
    QString description();
    QString error();
    bool loadConfig(QString filename);
    bool saveConfig(QString filename);
    QStringList infoConfig();

    /* QDltPluginViewerInterface */
    QWidget* initViewer();
    void initFileStart(QDltFile *file);
    void initFileFinish();
    void initMsg(int index, QDltMsg &msg);
    void initMsgDecoded(int index, QDltMsg &msg);
    void updateFileStart();
    void updateMsg(int index, QDltMsg &msg);
    void updateMsgDecoded(int index, QDltMsg &msg);
    void updateFileFinish();
    void selectedIdxMsg(int index, QDltMsg &msg);
    void selectedIdxMsgDecoded(int index, QDltMsg &msg);

    /* internal variables */
    Form *form;  
    QDltControl *dltControl;
    QMap<QString, ContextElement*> contextElements;
    QMutex contextElementsLock;

    void trySendMessages();

    TraceValidatorClient* traceValidatorClient;
    TraceValidatorServer* traceValidatorServer;
    TraceElementResponseProcessor* traceElementResponseProcessor;
private:
    QString errorText;
    Json json;
    int messageCounter;
};

#endif // CONTRACTVALIDATORPLUGIN_H
