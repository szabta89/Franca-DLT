#ifndef CONTEXTELEMENT_H
#define CONTEXTELEMENT_H

#include <QString>
#include <QMap>
#include "qdlt.h"

class ContextElement
{
public:
    explicit ContextElement();
    explicit ContextElement(QString contextId);
    ~ContextElement();
    QString contextId;
    QString filePath;

    int valid; // -1 unknown, 0 valid, 1 invalid

    // expectations mapped to msg ids
    QMap<int, QString>* expectations;

    // validness values mapped to msg ids
    QMap<int, int>* status;

    // messages mapped to msg ids
    QMap<int, QDltMsg>* messages;

    // unsent msg ids
    QList<int>* unsentMessages;
private:
    void init(QString _contextId);
};

#endif // CONTEXTELEMENT_H
