#ifndef TRACEVIEWERPLUGIN_H
#define TRACEVIEWERPLUGIN_H

#include <QObject>
#include <QMap>
#include "plugininterface.h"
#include "form.h"
#include "tracevalidatorclient.h"
#include "tracevalidatorserver.h"
#include "json.h"

#define PLUGIN_VERSION "1.0.0"

class TraceValidatorServer;
class TraceValidatorClient;

class TraceViewerPlugin : public QObject, QDLTPluginInterface, QDltPluginViewerInterface, QDltPluginControlInterface
{
    Q_OBJECT
    Q_INTERFACES(QDLTPluginInterface)
    Q_INTERFACES(QDltPluginViewerInterface)
    Q_INTERFACES(QDltPluginControlInterface)
#ifdef QT5
    Q_PLUGIN_METADATA(IID "org.franca.tools.contracts.TraceViewerPlugin")
#endif

public:
    TraceViewerPlugin();
    ~TraceViewerPlugin();

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

    /* QDltPluginControlInterface */
    bool initControl(QDltControl *control);
    bool initConnections(QStringList list);
    bool controlMsg(int index, QDltMsg &msg);
    bool stateChanged(int index, QDltConnection::QDltConnectionState connectionState);

    /* internal variables */
    Form *form;
    int counterMessages;
    int counterNonVerboseMessages;
    int counterVerboseMessages;

    void show(bool value);
    void updateCounters(int start,int end);
    QDltControl *dltControl;
    QMap<QString, ContextElement*> contextElements;
    QMutex contextElementsLock;
private:
    QDltFile* dltFile;
    QString errorText;
    TraceValidatorClient* traceValidatorClient;
    TraceValidatorServer* traceValidatorServer;
    Json json;
    int messageCounter;
    void trySendMessages();
};

#endif // TRACEVIEWERPLUGIN_H
