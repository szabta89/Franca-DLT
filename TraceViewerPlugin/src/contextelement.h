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
    // trace failure at failedAt
    int failedAt;
    // expected transition set pretty print at failedAt position
    QString failedAtExpectation;

    QMap<int, QDltMsg> messages;
    QList<int>* unsentMessages;
};

#endif // CONTEXTELEMENT_H
