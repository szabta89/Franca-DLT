#include "contextelement.h"

ContextElement::ContextElement(QString _contextId)
{
    init(_contextId);
}

ContextElement::ContextElement()
{
    init(QString::fromAscii(""));
}

void ContextElement::init(QString _contextId)
{
    this->contextId = _contextId;
    this->valid = -1;
    this->unsentMessages = new QList<int>();
    this->status = new QMap<int, int>();
    this->messages = new QMap<int, QDltMsg>();
    this->expectations = new QMap<int, QString>();
}

ContextElement::~ContextElement()
{
    delete this->expectations;
    delete this->status;
    delete this->messages;
    delete this->unsentMessages;
}

