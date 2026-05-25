#include "PFD.hpp"
#include "DataFusionEngine.hpp"

static QFrame* createPanel(const QString& title, const QString& color) {
    QFrame* panel = new QFrame();
    panel->setStyleSheet(
        QString(
            "QFrame { background-color: %1; border-radius: 0px; border: 0px; }")
            .arg(color));
    QVBoxLayout* layout = new QVBoxLayout(panel);
    return panel;
}
PFD::PFD(QWidget* parent) : QFrame(parent) {
    setStyleSheet(
        "QFrame { background-color: #0a0a0a; border-radius: 2px solid #333; }");
    QGridLayout* layout = new QGridLayout(this);
    m_artificial_horizon = new ArtificialHorizon(this);
    m_airspeed_indicator = new AirspeedIndicator(this);
    m_altimeter = new Altimeter(this);
    m_hsi = new HSI(this);
    setupLeftPanel(layout);
}
void PFD::setupLeftPanel(QGridLayout* grid) {
    grid->setSpacing(1);

    constexpr uint8_t horizon_height = 8;
    constexpr uint8_t ias_alt_height = 2 * horizon_height - 3;
    constexpr uint8_t horizon_hsi_span = 13;
    constexpr uint8_t total_span = 15;
    constexpr uint8_t fma_height = 1;
    constexpr uint8_t ias_alt_span = 1;
    constexpr uint8_t ias_alt_start_row = fma_height;
    constexpr uint8_t alt_start_row = ias_alt_start_row + ias_alt_height;

    grid->addWidget(createPanel("FMA", "black"), 0, 0, fma_height, total_span);
    grid->addWidget(m_airspeed_indicator, ias_alt_start_row, 0, ias_alt_height,
                    ias_alt_span);
    grid->addWidget(m_artificial_horizon, 1, 1, horizon_height,
                    horizon_hsi_span);
    grid->addWidget(m_hsi, horizon_height, 1, horizon_height - 2,
                    horizon_hsi_span);
    grid->addWidget(m_altimeter, ias_alt_start_row, alt_start_row,
                    ias_alt_height, ias_alt_span);
}
void PFD::connectDataFusionEngineToPFDComponents(DataFusionEngine* engine) {
    connect(engine, SIGNAL(orientationUpdated(const Eigen::Quaterniond&)),
            m_artificial_horizon,
            SLOT(updateOrientation(const Eigen::Quaterniond&)));
    connect(engine, SIGNAL(airspeedUpdated(double)), m_airspeed_indicator,
            SLOT(updateAirspeed(double)));
    connect(engine, SIGNAL(altitudeUpdated(double)), m_altimeter,
            SLOT(updateAltitude(double)));
    connect(engine, SIGNAL(headingUpdated(double)), m_hsi,
            SLOT(setCurrentHeading(double)));
}
void PFD::connectDCPToPFD(CRS_knob* crs_knob) {
    connect(crs_knob, SIGNAL(courseChanged(int16_t)), m_hsi, SLOT(setCurrentCRS(int16_t)));
}
