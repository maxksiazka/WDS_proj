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
 */
struct SensorData {
    /**
     * @brief -- Fixed value (0xDEADBEEF) used to identify the start of a
     * valid sensor data packet.
     */
    uint32_t sync_word;
    /**
     * @brief -- Timestamp in microseconds since MCU startup.
     */
    uint64_t timestamp_us;
    /**
     * @brief -- Accelerometer measurements in m/s^2 for X, Y, Z axes.
     */
    float accel[3];
    /**
     * @brief -- Gyroscope measurements in rad/s for X, Y, Z axes.
     */
    float gyro[3];
    /**
     * @brief -- Magnetometer measurements in microteslas for X, Y, Z axes.
     */
    float mag[3];
    /**
     * @brief -- Atmospheric pressure in hPa.
     */
    float pressure;
    /**
     * @brief -- Ambient temperature in degrees Celsius.
     */
    float temperature;
    /**
     * @brief -- Altitude in meters above sea level.
     */
    float altitude;
    /**
     * @brief -- Airspeed in knots.
     */
    float airspeed;
    /**
     * @brief -- Ground speed from GPS in knots.
     */
    float gps_ground_speed;
    /**
     * @brief -- Latitude from GPS in degrees.
     */
    float gps_lat;
    /**
     * @brief -- Longitude from GPS in degrees.
     */
    float gps_lon;
    /**
     * @brief -- Number of GPS satellites in view.
     */
    uint8_t gps_sats;
    /**
     * @brief -- Fix quality from GPS (0 = no fix, 1 = 2D fix, 2 = 3D fix).
     */
    uint8_t gps_fix;
    /**
     * @brief -- CRC-16-CCITT checksum of the sensor data.
     */
    uint16_t checksum;
} __attribute__((packed));

/**
 * @brief Class responsible for network communication with the MCU.
 *
 * The SensorLink class handles broadcasting its presence on the local network
 * using UDP and accepting incoming TCP connections from device.
 * It's main responsibility is to facilitate the communication between the MCU
 * and the Qt visualization app.
 *
 */
class SensorLink : public QObject {
    Q_OBJECT
  private:
    /**
     * @brief -- QUdpSocket used for broadcasting the presence of
     * this sensor link on the local network.
     */
    QUdpSocket* m_udp_socket;
    /**
     * @brief -- QTcpServer used to listen for incoming TCP connections.
     */
    QTcpServer* m_tcp_server;
    /**
     * @brief -- QTcpSocket representing the currently connected TCP
     * client.
     *
     * This will be nullptr if no client is currently connected.
     */
    QTcpSocket* m_tcp_client = nullptr;
    /**
     * @brief -- QTimer used to periodically trigger the broadcast
     * of this sensor link's presence on the local network.
     */
    QTimer* m_broadcast_timer;
    /**
     * @brief -- The UDP port number used for broadcasting the presence of
     * this sensor link.
     */
    uint16_t m_udp_port;
    /**
     * @brief -- Buffer to accumulate incoming data from the TCP client.
     */
    QByteArray m_buffer;
    /**
     * @brief -- Fixed sync word used to identify the start of a valid sensor data packet.
     */
    static constexpr uint32_t SYNC_WORD = 0xDEADBEEF;
    /**
     * @brief -- The expected size of the sensor data packet, including the sync word and checksum.
     */
    static constexpr size_t SENSOR_DATA_SIZE = sizeof(SensorData);
    /**
     * @brief Verifies the CRC-16-CCITT checksum of received sensor data.
     */
    bool verify_checksum(const SensorData& data);
    /**
     * @brief Calculates CRC-16-CCITT checksum.
     */
    uint16_t calculate_crc16(const uint8_t* data, size_t length);

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
    /**
     * @brief Handles incoming sensor data from the connected TCP client.
     *
     */
    void handle_data_received();
  signals:
    /**
     * @brief Emitted when a new device connects to the TCP server.
     * @param ip_addr -- The IP address of the connected device.
     */
    void device_connected(QString ip_addr);
    /**
     * @brief Emitted when new sensor data is received from the MCU.
     * @param data -- The received sensor data.
     */
    void data_received(const SensorData& data);

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
