#ifndef MOSQUITTOCLIENT_H
#define MOSQUITTOCLIENT_H


#include <QObject>
#include <QHostAddress>
#include <QString>
#include <QMap>
#include <mosquittopp.h>
#include <QMqttMessage.h>

/**
 * @brief The QMqttClient class provides mqtt networking functions and events.
 *
 * @note
 * This inherits the mosqpp::mosquittopp class with the private visiblity. In this way, you cannot access directly to functions provided by the libmosquitto library.
 *
 * <br>
 * @par Overview
 * This class provides slots such as subscribe() or publish() and also signals like connected(), published(), or subscribed().
 * It also provides a mqtt error management by sending the signal error() when an error occured during the communication with the broker.
 *
 * A mqtt client is identified by its unique mqtt-identifier, represented by a string. This id is set at the instanciation by passing it to the constructor,
 * and can be accessed by calling id().
 *
 * <br>
 * @par Connection management
 * The QMqttClient class provides the connect() and disconnect() slots to start and stop communication with a broker.
 * Before calling these slots, you have to set the host and port on which the broker can be joined by calling setHost() and setPort(), or, passing the host and port to
 * the constructor.
 * As the id member is const, it cannot be changed after the instanciation, and no method setId() is provided.
 *
 * @code
 * #include <QMqttClient.h>
 *
 * ...
 *
 * QMqttClient* client = new QMqttClient("myclient", QHostAddress::LocalHost, 1883);
 * client->connect();
 *
 * ...
 *
 * client->disconnect();
 *
 * @endcode
 * Once connected, the client instance emits the connected() signal. Once disconnected, it emits the disconnected() signal.
 * You can also check the connection state by calling the isConnected() method.
 *
 * <br>
 * @par Subscribing topics
 * @todo doc
 *
 * @par Publishing messages
 * @todo doc
 *
 * @note
 * It is a wrapper to the mosquittopp class. In this way, it depends on the libmosquittopp library that must be installed and accessible on your platform.
 * The mqtt4Qt library is developped on Ubuntu and with the mosquitto broker, and has not be tested on other platforms or with other brokers.<br><b>Feed back are welcome ! ;)</b>
 */
class QMqttClient : public QObject, private mosqpp::mosquittopp
{
    Q_OBJECT
private:
    const QString m_id;
    QHostAddress m_host;
    int m_port;
    int m_keepAlive;
    bool m_connected;
    int MID;
    QStringList m_subscribedTopics;
    QMap<int, QString> m_subscribingTopics;
    QMqttMessage m_lastWillMessage;

    void on_disconnect(int rc);
    void on_connect(int rc);
    void on_message(const struct mosquitto_message* mqtt_message);
    void on_subscribe(int mid, int qos_count, const int* granted_qos);
    int send_message(const QString& topic, const QString& data, bool retain, QMqttMessage::QoS qos = QMqttMessage::AT_MOST_ONCE);
    void on_publish(int mid);

public:
    /**
     * Creates a QMqttClient object.
     * parent is passed on to the QObject constructor.
     */
    explicit QMqttClient(const QString& mqtt_id, QObject *parent = 0);

    /**
     * Creates a QMqttClient object with the specified id. parent is passed on to the QObject constructor.
     * @param id
     * @param address
     * @param port
     * @param parent
     */
    QMqttClient(const QString& id, const QHostAddress& address, int port, QObject* parent = 0);

    /**
     * Destroys the object, closing the connection if necessary.
     */
    ~QMqttClient();

    /**
     * @brief isConnected
     * @return Returs true if the client is actually connected to a broker. Otherwise, returns false.
     */
    bool isConnected() const { return m_connected; }

    /**
     * @brief host Returns the host address of the broker.
     * @return
     */
    const QHostAddress& host() const { return m_host; }

    /**
     * @brief setHost
     * @param host
     */
    void setHost(const QHostAddress& host) { m_host = host; }

    /**
     * @brief subscribedTopics
     * @return
     */
    const QStringList& subscribedTopics() const { return m_subscribedTopics; }

    /**
     * @brief id
     * @return
     */
    const QString& id() const { return m_id; }

    /*
     * @brief setId
     * @param id
     */
    //void setId(const QString& aid) { m_id = aid; }

    /**
     * @brief port
     * @return
     */
    int port() const { return m_port; }

    /**
     * @brief setPort
     * @param port
     */
    void setPort(int _port) { m_port = _port; }

    /**
     * @brief keepAlive
     * @return
     */
    int keepAlive() const { return m_keepAlive; }

    /**
     * @brief setKeepAlive
     * @param keepAlive
     */
    void setKeepAlive(int _keepAlive) { m_keepAlive = _keepAlive; }

    /**
     * @brief setLastWillMessage
     * @param will
     */
    void setLastWillMessage(const QMqttMessage& will) { m_lastWillMessage = will; }

    /**
     * @brief getLastWillMessage
     * @return
     */
    const QMqttMessage& getLastWillMessage() const { return m_lastWillMessage; }



signals:
    /**
     * @brief connected
     * This signal is emitted when the client is connected
     */
    void connected();
    /**
     * @brief disconnected
     * This signal is emitted when the client is disconnected
     */
    void disconnected();
    /**
     * @brief subscribed
     * This signal is emitted when the client has subscribed successfully to a topic
     */
    void subscribed(QString topic = "");
    /**
     * @brief published
     * This signal is emitted when the client has successfully published a message
     */
    void published();
    /**
     * @brief messageReceived
     * This signal is emitted when the mqtt-client has received a mqtt-message
     */
    void messageReceived(QMqttMessage message);
    /**
     * @brief error
     * this signal is emitted when an error occured.
     * @param errorMessage A human-readable string that describes the error
     */
    void error(QString errorMessage);

public slots:
    /**
     * @brief publish
     * Publish a data to the specified topic
     * @param topic
     * @param data
     * @param qos
     * @param retain
     */
    void publish(QString topic, QByteArray data,
                 QMqttMessage::QoS qos = QMqttMessage::AT_MOST_ONCE,
                 bool retain = false);
    /**
     * @brief publish
     * @param message
     */
    void publish(QMqttMessage message, QMqttMessage::QoS qos = QMqttMessage::AT_MOST_ONCE);
    /**
     * Subscribe to a topic.
     * @param topic
     */
    void subscribe(QString topic, QMqttMessage::QoS qos = QMqttMessage::AT_MOST_ONCE);
    /**
     * Initiate a connection to a broker.
     */
    void connect();
    /**
     * Disable the connection with the broker.
     */
    void disconnect();

    /**
     * @brief unsubscribe
     * @param topic
     */
    void unsubscribe(QString topic);

};

#endif // MOSQUITTOCLIENT_H
