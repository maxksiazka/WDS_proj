#ifndef PFD_HPP_
#define PFD_HPP_

#include "ArtificialHorizon.hpp"
#include "DataFusionEngine.hpp"
#include <QFrame>
#include <QGridLayout>
#include <QWidget>

class PFD : public QFrame {
    Q_OBJECT
  private:
    ArtificialHorizon* artificial_horizon_;
    /**
     * @brief Internal method to set up the left panel of the PFD.
     *
     * Sets up the left panel of the PFD, which includes the artificial horizon,
     * airspeed indicator, and IAS and altitude tapes.  
     * @param[in] grid -- The grid layout to which the left panel components will be added.
     */
    void setupLeftPanel(QGridLayout* grid);

  public:
    void connectDataFusionEngineToArtificialHorizon(DataFusionEngine* engine);
    /**
     * @brief PFD constructor.
     *
     * @param[in] parent -- The parent widget of the PFD panel, default is nullptr.
     */
    explicit PFD(QWidget* parent = nullptr);
    ~PFD() = default;
};

#endif // PFD_HPP_
