#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP
/**
 * @file MainWindow.hpp
 * @brief Defines the MainWindow class for the virtual cockpit simulator
 * application.
 *
 * @author Maksymilian Ksiazka
 * @date 2026-05-07
 */
#include "DCP.hpp"
#include "DataFusionEngine.hpp"
#include "MFD.hpp"
#include "PFD.hpp"
#include <QFrame>
#include <QGridLayout>
#include <QMainWindow>

/**
 * @brief MainWindow class for the virtual cockpit simulator application.
 *
 * Nothing crazy, just a container for the PFD, MFD and DCP widgets.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT
  private:
    /**
     * @brief -- Pointer to the PFD widget, which displays the primary flight
     * information: attitude, airspeed, and altitude.
     */
    PFD* m_pfd;
    /**
     * @brief -- Pointer to the MFD widget, which displays additional
     * information: telemetry status and navigation maps.
     */
    MFD* m_mfd;
    /**
     * @brief -- Pointer to the DCP widget, which contains controls for
     * adjusting settings: barometric pressure (QNH) and course (CRS).
     */
    DCP* m_dcp;

  public:
    /**
     * @brief Constructs the main window of the virtual cockpit simulator,
     * initializing the PFD and DCP widgets and setting up the layout.
     *
     * @param[in] parent -- Pointer to the parent widget, defaulting to nullptr.
     * The main window will be the top-level widget in this application.
     */
    MainWindow(QWidget* parent = nullptr);
    /**
     * @brief Method used to connect the DataFusionEngine instance to the PFD
     * components.
     *
     * @param[in] engine -- Pointer to the DataFusionEngine instance that will
     * provide sensor data.
     */
    void connectDataFusionEngineToPFD(DataFusionEngine* engine);
    /**
     * @brief Method used to connect the DataFusionEngine instance to the DCP
     * components, such as the QNH knob.
     *
     * @param[in] engine -- Pointer to the DataFusionEngine instance that will
     * provide sensor data.
     */
    void connectDataFusionEngineToDCP(DataFusionEngine* engine);
    /**
     * @brief API method to connect the SensorLink instance to the MFD components, to be called from main
     *
     * @param[in] link -- Pointer to the SensorLink instance.
     */
    void connectSensorLinkToMFD(SensorLink* link);
    /**
     * @brief API method to connect the DataFusionEngine instance to the MFD components, to be called from main
     *
     * @param[in] engine -- Pointer to the DataFusionEngine instance.
     */
    void connectDataFusionEngineToMFD(DataFusionEngine* engine);
};
#endif // MAINWINDOW_HPP
