#ifndef SYSTEM_STATUS_HEADER_HPP_
#define SYSTEM_STATUS_HEADER_HPP_
#include <QFrame>
#include <QLabel>
class SystemStatusHeader : public QFrame {
    Q_OBJECT

  private:
    QLabel* m_time_label;
    QLabel* m_qnh_label;
    QLabel* m_oat_label;
    QLabel* m_gps_label;
    QTimer* m_update_timer;

  public:
    explicit SystemStatusHeader(QWidget* parent = nullptr);
  public slots:
    void updateTime();
    void updateSystemData(float oat, uint8_t gps_sats,
                          uint8_t gps_fix);
    void updateQNH(double qnh);
};
#endif // SYSTEM_STATUS_HEADER_HPP_
