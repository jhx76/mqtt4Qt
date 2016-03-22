#include "QMqttMessage.h"

#include <QSharedData>

//-----------------------------------------------------------
/*
 * metatype registration
 */
class QMqttMessageInit {
public:
    QMqttMessageInit() {
        qRegisterMetaType<QMqttMessage>();
    }
};
static QMqttMessageInit mosquittoMessageInit;
//-----------------------------------------------------------
QMqttMessage::QMqttMessage()
    : d(new QMqttMessageData)
{
    setRetainFlag(false);
}
//-----------------------------------------------------------
QMqttMessage::QMqttMessage(const QString& topic, const QByteArray &payload, bool retain)
    : d(new QMqttMessageData)
{
    setTopic(topic);
    setPayload(payload);
    setRetainFlag(retain);
}
//-----------------------------------------------------------
QMqttMessage::QMqttMessage(const QMqttMessage &other)
    : d(other.d)
{

}
//-----------------------------------------------------------
QMqttMessage& QMqttMessage::operator =(const QMqttMessage& other)
{
    if(other.d != d) {
        d = other.d;
    }
    return *this;
}
//-----------------------------------------------------------
const QString& QMqttMessage::topic() const
{
    return d->topic;
}
//-----------------------------------------------------------
void QMqttMessage::setTopic(const QString &topic)
{
    d->topic = topic;
}
//-----------------------------------------------------------
const QByteArray& QMqttMessage::payload() const
{
    return d->payload;
}
//-----------------------------------------------------------
void QMqttMessage::setPayload(const QByteArray &data)
{
    d->payload = data;
}
//-----------------------------------------------------------
bool QMqttMessage::hasRetainFlag() const
{
    return d->retainFlag;
}
//-----------------------------------------------------------
void QMqttMessage::setRetainFlag(bool flag)
{
    d->retainFlag = flag;
}
