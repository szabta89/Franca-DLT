#ifndef JSON_H
#define JSON_H

#include <QtScript/QScriptEngine>
#include <QtScript/QScriptValueIterator>

class Json
{
public:
    Json();
    QString encode(const QMap<QString,QVariant> &map);
    QMap<QString, QVariant> decode(const QString &jsonStr);
private:
    QScriptValue encodeInner(const QMap<QString,QVariant> &map, QScriptEngine* engine);
    QMap<QString, QVariant> decodeInner(QScriptValue object);
    QList<QVariant> decodeInnerToList(QScriptValue arrayValue);
};

#endif // JSON_H
