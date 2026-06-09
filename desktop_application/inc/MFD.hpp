#ifndef MFD_HPP_
#define MFD_HPP_

#include <QFrame>
#include <QVBoxLayout>
#include <QWidget>

class MFD : public QFrame {
    Q_OBJECT
  private:
    /**
     * @brief -- Pointer to the telemetry/engine data panel. 
     * Currently a placeholder QFrame, to be switched for a dedicated class later.
     */
    QFrame* m_telemetry_panel;

    /**
     * @brief -- Pointer to the navigation map panel.
     * Currently a placeholder QFrame, to be switched for a dedicated class later.
     */
    QFrame* m_nav_map_panel;

    /**
     * @brief Internal method to set up the layout structure of the MFD.
     * @param[in] layout -- The vertical layout where components are stacked.
     */
    void setupRightPanel(QVBoxLayout* layout);

  public:
    /**
     * @brief MFD constructor.
     * @param[in] parent -- The parent widget of the MFD panel, default is nullptr.
     */
    explicit MFD(QWidget* parent = nullptr);
    ~MFD() = default;
};

#endif // MFD_HPP_
