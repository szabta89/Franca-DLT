#include "contextelement.h"

ContextElement::ContextElement(QString _contextId)
{
    this->contextId = _contextId;
    this->valid = -1;
    this->failedAt = -1;
    this->unsentMessages = new QList<int>();
}

ContextElement::ContextElement()
{
    this->unsentMessages = new QList<int>();
}

ContextElement::~ContextElement()
{
    delete this->unsentMessages;
}

