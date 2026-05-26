#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP
#include "DCP.hpp"
#include "DataFusionEngine.hpp"
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
     * information such as attitude, airspeed, and altitude.
     */
    PFD* m_pfd;
    /**
     * @brief -- Pointer to the DCP widget, which contains controls for
     * adjusting settings like barometric pressure (QNH) and course (CRS).
     */
    DCP* m_dcp;
    /**
     * @brief Internal method to create a styled panel with a title and
     * background color.
     *
     * This method creates a QFrame with a specified title and background color,
     * which can be used to contain the PFD and DCP widgets. It sets up the
     * layout and styling for the panel to ensure a consistent appearance.
     *
     * @param[in] title -- The title to display at the top of the panel.
     * @param[in] color -- The background color for the panel, specified as a
     * string (e.g., "#333333" for dark gray).
     * @return A pointer to the created QFrame panel.
     */
    [[deprecated]] QFrame* createPanel(const QString& title,
                                       const QString& color);

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
     * @param[in] engine -- Pointer to the DataFusionEngine instance that will provide sensor data.
     */
    void connectDataFusionEngineToPFD(DataFusionEngine* engine);
    /**
     * @brief Method used to connect the DataFusionEngine instance to the DCP components, such as the QNH knob.
     *
     * @param[in] engine -- Pointer to the DataFusionEngine instance that will provide sensor data.
     */
    void connectDataFusionEngineToDCP(DataFusionEngine* engine);
};
#endif // MAINWINDOW_HPP
