#include "QMqttClient.h"
#include <QDebug>

//--------------------------------------------------------------------
QMqttClient::QMqttClient(const QString& mqtt_id, QObject *parent)
    : QObject(parent),
      mosquittopp(mqtt_id.toLocal8Bit().data()),
      m_id(mqtt_id),
      m_port(1883),
      m_keepAlive(60),
      m_connected(false),
      MID(0)
{

}
//--------------------------------------------------------------------
QMqttClient::QMqttClient(const QString& mqtt_id, const QHostAddress& host, int port, QObject* parent)
    : QObject(parent),
      mosquittopp(mqtt_id.toLocal8Bit().data()),
      m_id(mqtt_id),
      m_host(host),
      m_port(port),
      m_keepAlive(60), //seconds
      m_connected(false),
      MID(0)

{

}
//--------------------------------------------------------------------
QMqttClient::~QMqttClient()
{
    disconnect();
}
//--------------------------------------------------------------------
void QMqttClient::connect()
{
    if(!m_connected) {
        mosqpp::lib_init();
        // non blocking connection to broker
        int connect_rslt = connect_async(m_host.toString().toLatin1().data(), m_port, m_keepAlive);
        switch(connect_rslt) {
            case MOSQ_ERR_INVAL:
                emit error(QString("the input parameters were invalid"));
                return;
            case MOSQ_ERR_SUCCESS:
                // Start thread managing connection / publish / subscribe
                loop_start();
                return;
            default: {
                QString errorMessage = mosquitto_strerror(connect_rslt);
                emit error(errorMessage);
            }
        }
    }
    else {
        qDebug() << "client is already connected...";
    }
}
//--------------------------------------------------------------------
void QMqttClient::disconnect()
{
    if(m_connected) {
        loop_stop(true); // Kill the thread
        mosqpp::lib_cleanup(); // mosquitto library cleanup
        emit disconnected();
        m_connected = false;
    }
}
//--------------------------------------------------------------------
void QMqttClient::on_connect(int rc)
{
    switch(rc)
    {
    case 0: //success
        m_connected = false;
        emit connected();
        break;
    case 1: //connection refused (unacceptable protocol version)
        m_connected = false;
        emit error(QString("Connection refused: unacceptable protocol version"));
        break;
    case 2: //connection refused (identifier rejected)
        m_connected = false;
        emit error(QString("Connection refused: identifier rejected"));
        break;
    case 3: // connection refused (broker unavailable)
        m_connected = false;
        emit error(QString("Connection refused: broker unavailable"));
        break;
    }
}
//--------------------------------------------------------------------
int QMqttClient::send_message(const QString& topic, const QString& data, bool retain, QMqttMessage::QoS qos)
{
    return mosquittopp::publish(&MID,
                                topic.toLocal8Bit().data(),
                                strlen(data.toLocal8Bit().data()),
                                data.toLocal8Bit().data(),
                                qos,
                                retain);
}
//--------------------------------------------------------------------
void QMqttClient::on_disconnect(int rc)
{
    Q_UNUSED(rc)
    m_connected = false;
    emit disconnected();
}
//--------------------------------------------------------------------
void QMqttClient::on_message(const mosquitto_message *mqtt_message)
{
    if(mqtt_message->payloadlen) {
        QByteArray message_received = (char*)mqtt_message->payload;
        QMqttMessage message(QString(mqtt_message->topic), message_received, mqtt_message->retain);
        emit messageReceived(message);
    }
    else {
        qDebug() << "MOSQ - " << mqtt_message->topic << "(no data)";
    }
}
//--------------------------------------------------------------------
void QMqttClient::on_subscribe(int mid, int qos_count, const int *granted_qos)
{
    // TODO WE DON'T UNDERSTAND WHAT THE FUCK ARE THESE TWO PARAMS !!!
    Q_UNUSED(qos_count)
    Q_UNUSED(granted_qos)
    qDebug() << "MOSQ - Subscription succeeded (" << mid << ")";
    if(mid != 0) {
        QString subscribedTopic = m_subscribingTopics.value(mid);
        if(!subscribedTopic.isEmpty()) {
            m_subscribingTopics.remove(mid);
            emit subscribed(subscribedTopic);
        }
    }
    MID++;
    qDebug() << "MOSQ - updated MID (" << MID << ")";
}
//--------------------------------------------------------------------
void QMqttClient::on_publish(int mid)
{
    qDebug() << "MOSQ - Message (" << mid << ") succeed to be published";
    MID++;
    qDebug() << "MOSQ - updated MID (" << MID << ")";
    emit published();
}
//--------------------------------------------------------------------
void QMqttClient::publish(QString topic, QByteArray data, QMqttMessage::QoS qos, bool retain)
{
    publish(QMqttMessage(topic, data, retain), qos);
}
//--------------------------------------------------------------------
void QMqttClient::publish(QMqttMessage message, QMqttMessage::QoS qos)
{
    int rslt = send_message(message.topic(), message.payload(), message.hasRetainFlag(), qos);
    if(rslt != MOSQ_ERR_SUCCESS) {
        qDebug() << "MOSQ - Failed to publish message...";
        QString errorMessage = mosquitto_strerror(rslt);
        emit error(errorMessage);
    }
}
//--------------------------------------------------------------------
void QMqttClient::subscribe(QString topic, QMqttMessage::QoS qos)
{
    int rslt = mosquittopp::subscribe(&MID, topic.toLocal8Bit().data(), qos);
    m_subscribingTopics.insert(MID, topic);
    qDebug() << "Subscribing to topic (id = " << MID << ")";
    if(rslt != MOSQ_ERR_SUCCESS) {
        QString errorMessage = mosquitto_strerror(rslt);
        emit error(errorMessage);
    }
}
//--------------------------------------------------------------------
void QMqttClient::unsubscribe(QString topic)
{
    int rslt = mosquittopp::unsubscribe(&MID, topic.toLocal8Bit().data());
    if(rslt != MOSQ_ERR_SUCCESS) {
        QString errorMessage = mosquitto_strerror(rslt);
        emit error(errorMessage);
    }
}
