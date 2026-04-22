#ifndef SENSORLINK_H_
#define SENSORLINK_H_
/**
 * @file SensorLink.hpp
 * @brief Defines the SensorLink class for broadcasting sensor data over a
 * network.
 *
 * This class uses Qt's networking capabilities to broadcast its presence on the
 * local network using UDP and to accept incoming TCP connections from devices
 * that want to receive sensor data.
 *
 * The SensorData struct is defined to represent the sensor data that will be
 * transmitted, including timestamp, IMU data, GPS data, and pressure.
 *
 * The SensorLink class provides methods to start the service and handles
 * broadcasting and incoming connections.
 *
 * @author Maksymilian Ksiazka
 * @date 2026-04-20
 */

#include <QObject>

#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include <QUdpSocket>
#include <stdint.h>

/**
 * @brief Struct of sensor data to be received from the MCU.
 *
 */
struct SensorData {
    uint32_t timestamp;
    float imu[6]; // 3 for accelerometer, 3 for gyroscope
    float gps[3]; // latitude, longitude, altitude
    float pressure;

} __attribute__((packed));

/**
 * @brief Class responsible for network communication with the MCU.
 *
 * The SensorLink class handles broadcasting its presence on the local network
 * using UDP and accepting incoming TCP connections from device.
 * It's main responsibility is to facilitate the communication between the MCU and the Qt visualization app.
 *
 */
class SensorLink : public QObject {
    Q_OBJECT
  private:
    QUdpSocket* m_udp_socket;
    QTcpServer* m_tcp_server;
    QTcpSocket* m_tcp_client = nullptr;
    QTimer* m_broadcast_timer;
    uint16_t m_udp_port;
    QByteArray m_buffer;

  signals:
    /**
     * @brief Emitted when a new device connects to the TCP server.
     * @param ip_addr -- The IP address of the connected device.
     */
    void device_connected(QString ip_addr);

  private slots:
    /**
     * @brief Broadcasts the presence of this sensor link on the local network
     * using UDP.
     *
     */
    void broadcast_presence();
    /**
     * @brief Handles incoming TCP connections from devices.
     *
     * When a new device connects, it emits the `device_connected` signal with
     * the IP address of the connected device.
     *
     */
    void handle_new_connection();

  public:
    /**
     * @brief Constructs a new SensorLink object.
     *
     *
     * @param[in] parent -- The parent QObject, if any.
     */
    explicit SensorLink(QObject* parent = nullptr);
    /**
     * @brief Starts the sensor link by setting up the UDP broadcast and TCP
     * server.
     *
     * @param[in] udp_port -- UDP port to use for broadcasting the presence of
     * this sensor link on the local network.
     * @param[in] tcp_port -- TCP port to listen for incoming connections from
     * devices.
     */
    void start(uint16_t udp_port, uint16_t tcp_port);
};

#endif /* SENSORLINK_H_ */
