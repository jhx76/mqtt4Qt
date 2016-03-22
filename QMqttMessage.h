#ifndef QMQTTMESSAGE_H
#define QMQTTMESSAGE_H

#include <QMetaType>
#include <QSharedDataPointer>
#include <QString>
#include <QByteArray>

/*
 * private data used for shared pointer
 */
class QMqttMessageData : public QSharedData
{
public:
    QString topic;
    QByteArray payload;
    bool retainFlag;
};
//-----------------------------------------------------------
/**
 * @brief The QMqttMessage class represents a mqtt message.
 */
class QMqttMessage
{
public:
    /**
     *
     *
     */
    typedef enum {
        AT_MOST_ONCE = 0,
        AT_LEAST_ONCE = 1,
        EXACTLY_ONCE = 2
    } QoS;


    /**
     * @brief QMqttMessage
     */
    QMqttMessage();
    /**
     * @brief QMqttMessage
     * @param topic
     * @param payload
     * @param retain
     */
    QMqttMessage(const QString& topic, const QByteArray& payload, bool retain = false);
    /**
     * @brief QMqttMessage
     * @param other
     */
    QMqttMessage(const QMqttMessage& other);
    /**
     * @brief operator =
     * @param other
     * @return
     */
    QMqttMessage& operator=(const QMqttMessage& other);
    /**
     * @brief getTopic
     * @return
     */
    const QString& topic() const;
    /**
     * @brief setTopic
     * @param topic
     */
    void setTopic(const QString& topic);
    /**
     * @brief payload
     * @return
     */
    const QByteArray& payload() const;
    /**
     * @brief setPayload
     * @param data
     */
    void setPayload(const QByteArray& data);
    /**
     * @brief hasRetainFlag
     * @return
     */
    bool hasRetainFlag() const;
    /**
     * @brief setRetainFlag
     * @param flag
     */
    void setRetainFlag(bool flag);

private:
    QSharedDataPointer<QMqttMessageData> d;
};

Q_DECLARE_METATYPE(QMqttMessage)

#endif // QMQTTMESSAGE_H
