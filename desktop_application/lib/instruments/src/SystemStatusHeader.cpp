#include "SystemStatusHeader.hpp"
#include <QDateTime>
#include <QGridLayout>
#include <QTimer>
SystemStatusHeader::SystemStatusHeader(QWidget* parent) : QFrame(parent) {
    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(15, 2, 15, 2);
    m_time_label = new QLabel("UTC: --:--:--", this);
    m_qnh_label = new QLabel("QNH: 1013.25 hPa", this);
    m_oat_label = new QLabel("OAT: --.- °C", this);
    m_gps_label = new QLabel("GPS: NO FIX", this);

    QString text_style = "QLabel { color: #00FF00; font-family: 'Consolas'; "
                         "font-size: 13px; font-weight: bold; }";

    m_time_label->setStyleSheet(text_style);
    m_qnh_label->setStyleSheet(text_style);
    m_oat_label->setStyleSheet(text_style);
    m_gps_label->setStyleSheet(text_style);

    layout->addWidget(m_time_label, 0, 0, Qt::AlignLeft);
    layout->addWidget(m_qnh_label, 0, 1, Qt::AlignCenter);
    layout->addWidget(m_oat_label, 0, 2, Qt::AlignCenter);
    layout->addWidget(m_gps_label, 0, 3, Qt::AlignRight);

    m_update_timer = new QTimer(this);
    connect(m_update_timer, &QTimer::timeout, this,
            &SystemStatusHeader::updateTime);
    m_update_timer->start(1000);

    setStyleSheet("background-color: #1a1a1a;");
}
void SystemStatusHeader::updateTime() {
    m_time_label->setText(
        QDateTime::currentDateTimeUtc().toString("'UTC:' hh:mm:ss"));
}

void SystemStatusHeader::updateQNH(double qnh) {
    m_qnh_label->setText(
        QString("QNH: %1 hPa").arg(qnh / 100.0, 0, 'f', 2, u' '));
}
void SystemStatusHeader::updateSystemData(float oat, uint8_t gps_sats,
                                          uint8_t gps_fix) {
    m_oat_label->setText(QString("OAT: %1 \u00B0C").arg(oat, 0,'f', 2, u' '));
    QString fix_status = "NO FIX";
    if (gps_fix == 1)
        fix_status = "2D FIX";
    if (gps_fix == 2)
        fix_status = "3D FIX";
    m_gps_label->setText(
        QString("GPS: %1 (%2 sats)").arg(fix_status).arg(gps_sats));
}
