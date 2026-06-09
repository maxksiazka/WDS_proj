#ifndef NAVIGATION_SCREEN_HPP_
#define NAVIGATION_SCREEN_HPP_
/**
 * @file NavigationScreen.hpp
 * @brief Defines the NavigationScreen class for displaying the navigation map
 * and waypoints.
 *
 * The NavigationScreen class is responsible for displaying a navigation map
 * with the plane's current position, heading, and waypoints. It provides an API
 * to add waypoints and update the plane's position based on GPS data.
 *
 * @author Maksymilian Ksiazka
 * @date 2026-05-07
 */

#include <QFrame>
#include <QList>
#include <QString>
#include <QWidget>

/**
 * @brief Struct representing a navigation waypoint, containing a name and GPS
 * coordinates.
 */
struct Waypoint {
    /**
     * @brief -- The name of the waypoint, to be displayed on the navigation
     * map.
     */
    QString name;
    /**
     * @brief -- The latitude of the waypoint in degrees.
     */
    double lat;
    /**
     * @brief -- The longitude of the waypoint in degrees.
     */
    double lon;
};

/**
 * @brief NavigationScreen class responsible for displaying the navigation map
 * and waypoints.
 *
 * The NavigationScreen class is responsible for rendering a navigation map that
 * shows the plane's current position, and waypoints defined in the flight plan.
 */
class NavigationScreen : public QFrame {
    Q_OBJECT
  private:
    /**
     * @brief -- The current latitude of the plane, in degrees.
     */
    double m_lat = 0.0;
    /**
     * @brief -- The current longitude of the plane, in degrees.
     */
    double m_lon = 0.0;
    /**
     * @brief -- The current heading of the plane, in degrees.
     */
    double m_heading = 0.0;
    /**
     * @brief -- The current zoom level of the map, in pixels per degree of
     * latitude/longitude.
     */
    double m_scale = 3000.0; // Default zoom
    /**
     * @brief -- List of waypoints to be displayed on the navigation map, each
     * containing a name and GPS coordinates.
     */
    QList<Waypoint> m_waypoints;
    /**
     * @brief Internal method to handle the painting of the navigation map,
     * including the plane's position, heading, and waypoints.
     *
     * @param[in] event -- The paint event that triggered this method.
     */
    void paintEvent(QPaintEvent* event) override;

  public:
    /**
     * @brief Constructor of the NavigationScreen class, which initializes the
     * navigation map panel.
     *
     * Standard for a QWidget subclass.
     *
     * @param[in] parent -- The parent widget of the navigation screen, default
     * is nullptr.
     */
    explicit NavigationScreen(QWidget* parent = nullptr);
    /**
     * @brief API method to add a new flight plan waypoint to the navigation
     * map.
     *
     * @param[in] wp -- The waypoint to be added, as defined in the \link Waypoint \endlink struct.
     */
    void addWaypoint(const Waypoint& wp);
    /**
     * @brief API method to clear all waypoints from the navigation map.
     */
    void clearWaypoints();
    /**
     * @brief API method to set the current zoom level of the navigation map.
     *
     * @param[in] scale -- The new zoom level.
     */
    void setZoom(double scale);
  public slots:
    /**
     * @brief Called when new GPS-based position and heading data is available,
     * to update the plane's position on the navigation map.
     *
     * This method is intended to be connected to the \link GPSHeadingUpdated
     * \endlink signal from the DataFusionEngine.
     *
     * @param[in] lat -- The new latitude of the plane in degrees.
     * @param[in] lon -- The new longitude of the plane in degrees.
     * @param[in] heading -- The new heading of the plane in degrees.
     */
    void setPlanePosition(double lat, double lon, double heading);
};

#endif // NAVIGATION_SCREEN_HPP_
