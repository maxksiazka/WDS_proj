#include "SensorLink.hpp"
#include <QDebug>

SensorLink::SensorLink(QObject* parent) : QObject(parent) {
    m_udp_socket = new QUdpSocket(this);
    m_tcp_server = new QTcpServer(this);
    m_broadcast_timer = new QTimer(this);

    connect(m_broadcast_timer,SIGNAL(timeout()), this, SLOT(broadcast_presence()));
    connect(m_tcp_server, SIGNAL(newConnection()), this, SLOT(handle_new_connection()));
}

void SensorLink::start(uint16_t udp_port, uint16_t tcp_port) {
    m_udp_port = udp_port;
    if (m_tcp_server->listen(QHostAddress::Any, tcp_port)) {
        qDebug() << "TCP Server started on port" << tcp_port;
        m_broadcast_timer->start(2000);
    }
}

void SensorLink::broadcast_presence() {
    QByteArray packet =
        "DISCOVER:" + QByteArray::number(m_tcp_server->serverPort());
    m_udp_socket->writeDatagram(packet, QHostAddress::Broadcast, m_udp_port);
    qDebug() << "UDP Broadcast sent...";
}

void SensorLink::handle_new_connection() {
    m_tcp_client = m_tcp_server->nextPendingConnection();
    m_broadcast_timer->stop();
    connect(m_tcp_client, SIGNAL(readyRead()), this, SLOT(handle_data_received()));
    emit device_connected(m_tcp_client->peerAddress().toString());
    qDebug() << "MCU connected!";
}

bool SensorLink::verify_checksum(const SensorData& data) {
    uint16_t crc = calculate_crc16(
        reinterpret_cast<const uint8_t*>(&data),
        SENSOR_DATA_SIZE - sizeof(data.checksum));
    return crc == data.checksum;
}

uint16_t SensorLink::calculate_crc16(const uint8_t* data, size_t length) {
    uint16_t crc = 0xFFFF;
    for (size_t i = 0; i < length; ++i) {
        crc ^= static_cast<uint16_t>(data[i]) << 8;
        for (int j = 0; j < 8; ++j) {
            crc = (crc & 0x8000) ? (crc << 1) ^ 0x1021 : crc << 1;
            crc &= 0xFFFF;
        }
    }
    return crc;
}
void SensorLink::handle_data_received() {
    if (!m_tcp_client) return;
    qDebug() << "Data received from MCU, buffering...";

    m_buffer.append(m_tcp_client->readAll());
    while (m_buffer.size() >= SENSOR_DATA_SIZE) {
        SensorData data;
        memcpy(&data, m_buffer.constData(), SENSOR_DATA_SIZE);
        m_buffer.remove(0, SENSOR_DATA_SIZE);
        qDebug() << "Received sensor data packet, verifying checksum";
        qDebug() << "Sensor Data: " << data.gps_lat << data.gps_lon;

        if (verify_checksum(data)) {
            qDebug() << "Received valid sensor data from MCU!";
            emit data_received(data);
        } else {
            qDebug() << "Checksum verification failed!";
        }
    }
}
