#ifndef SENSORLINK_H_
#define SENSORLINK_H_

#include <QObject>

#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include <QUdpSocket>
#include <stdint.h>

struct SensorData {
    uint32_t timestamp;
    float imu[6]; // 3 for accelerometer, 3 for gyroscope
    float gps[3]; // latitude, longitude, altitude
    float pressure;

} __attribute__((packed));

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
    void device_connected(QString ip_addr);

  private slots:
    void broadcast_presence();
    void handle_new_connection();

  public:
    explicit SensorLink(QObject* parent = nullptr);
    void start(uint16_t udp_port, uint16_t tcp_port);
};

#endif /* SENSORLINK_H_ */
