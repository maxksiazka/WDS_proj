#ifndef SYSTEM_STATUS_HEADER_HPP_
#define SYSTEM_STATUS_HEADER_HPP_
/**
 * @file SystemStatusHeader.hpp
 * @brief Defines the SystemStatusHeader class for displaying system status
 * information such as GPS satellite count, GPS fix status, outside air
 * temperature (OAT), and current time.
 * @author Maksymilian Ksiazka
 * @date 2026-05-26
 */
#include <QFrame>
#include <QLabel>
/**
 * @brief SystemStatusHeader widget for displaying system status information
 * such as GPS satellite count, GPS fix status, outside air temperature (OAT),
 * and current time.
 *
 *
 * The SystemStatusHeader is a custom widget -- that takes space usually
 * reserved for FMS data. It substitutes NAV data with GPS satellite count and
 * fix status, and substitutes A/P data with OAT.
 */
class SystemStatusHeader : public QFrame {
    Q_OBJECT

  private:
    /**
     * @brief m_time_label -- QLabel to display the current time in HH:MM:SS
     * format.
     */
    QLabel* m_time_label;
    /**
     * @brief m_qnh_label -- QLabel to display the current QNH value in hPa,
     * with 2 decimal places.
     */
    QLabel* m_qnh_label;
    /**
     * @brief m_oat_label -- QLabel to display the current outside air
     * temperature (OAT) in degrees Celsius, with 1 decimal place.
     */
    QLabel* m_oat_label;
    /**
     * @brief m_gps_label -- QLabel to display the GPS satellite count and fix
     * status in the format "GPS: (FIX STATUS): ((SATS: ))".
     */
    QLabel* m_gps_label;
    /**
     * @brief m_update_timer -- QTimer used to update the displayed
     * time every second.
     */
    QTimer* m_update_timer;

  public:
    /**
     * @brief Constructs a new SystemStatusHeader widget.
     *
     * This widget is typically a child widget within the PFD, located at the
     * top of the display.
     *
     * @param[in] parent -- The parent widget of the SystemStatusHeader, default
     * is nullptr.
     */
    explicit SystemStatusHeader(QWidget* parent = nullptr);
  public slots:
    /**
     * @brief Updates the displayed time to the current system time in HH:MM:SS
     * format.
     *
     * This slot is connected to a QTimer that triggers every second
     * to ensure the time display is always up to date.
     */
    void updateTime();
    /**
     * @brief Updates the displayed system status information.
     *
     * This slot should beconnected to a signal emitted by the
     * DataFusionEngine whenever the OAT, GPS sat count, or GPS fix is updated.
     *
     * @param[in] oat -- The outside air temperature (OAT) in degrees Celsius.
     * @param[in] gps_sats -- The number of GPS satellites in view.
     * @param[in] gps_fix -- The GPS fix status (0 for no fix, 1 for 2D fix, 2
     * for 3D fix).
     */
    void updateSystemData(float oat, uint8_t gps_sats, uint8_t gps_fix);
    /**
     * @brief Updates the displayed QNH value in hPa with 2 decimal places.
     *
     * This slot should be connected to a signal emitted by the QNH_knob
     * whenever the QNH value is adjusted by the user. It updates the QNH
     * display in the status header to reflect the new value.
     *
     * @param[in] qnh -- The new QNH value in hPa to be displayed on the status
     * header.
     */
    void updateQNH(double qnh);
};
#endif // SYSTEM_STATUS_HEADER_HPP_
