#include "SensorLink.hpp"
#include <QDebug>

SensorLink::SensorLink(QObject* parent) : QObject(parent) {
    m_udp_socket = new QUdpSocket(this);
    m_tcp_server = new QTcpServer(this);
    m_broadcast_timer = new QTimer(this);

    connect(m_broadcast_timer, &QTimer::timeout, this,
            &SensorLink::broadcast_presence);
    connect(m_tcp_server, &QTcpServer::newConnection, this,
            &SensorLink::handle_new_connection);
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
    emit device_connected(m_tcp_client->peerAddress().toString());
    qDebug() << "MCU connected!";
}
