#include "DCP.hpp"
#include <QFont>

DCP::DCP(QWidget* parent) : QFrame(parent) {
    setMinimumHeight(150);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setupUI();
    setupStylesheet();
    connect(m_btn_fpln_init, SIGNAL(clicked()), this, SIGNAL(FPLNInitPressed()));
    connect(m_btn_fpln_clr, SIGNAL(clicked()), this, SIGNAL(FPLNClrPressed()));
    connect(m_btn_zoom, SIGNAL(clicked()), this, SIGNAL(ZoomPressed()));
}

void DCP::setupUI() {
    main_layout_ = new QHBoxLayout(this);
    main_layout_->setContentsMargins(15, 10, 15, 10);
    main_layout_->setSpacing(10);
    QVBoxLayout* qnh_layout = new QVBoxLayout();
    qnh_label_ = new QLabel("BARO");
    QFont label_font;
    label_font.setPointSize(10);
    label_font.setBold(true);
    qnh_label_->setFont(label_font);
    qnh_label_->setAlignment(Qt::AlignCenter);

    qnh_knob_ = new QNH_knob();

    qnh_layout->addWidget(qnh_label_);
    qnh_layout->addWidget(qnh_knob_);
    main_layout_->addLayout(qnh_layout);

    QVBoxLayout* center_layout = new QVBoxLayout();
    QHBoxLayout* button_layout = new QHBoxLayout();

    m_btn_fpln_init = new QPushButton("FPLN INIT");
    m_btn_fpln_init->setMinimumSize(70, 40);
    button_layout->addWidget(m_btn_fpln_init);

    m_btn_fpln_clr = new QPushButton("FPLN CLR");
    m_btn_fpln_clr->setMinimumSize(70, 40);
    button_layout->addWidget(m_btn_fpln_clr);

    m_btn_zoom = new QPushButton("ZOOM");
    m_btn_zoom->setMinimumSize(70, 40);
    button_layout->addWidget(m_btn_zoom);
    center_layout->addLayout(button_layout);
    center_layout->addStretch();
    main_layout_->addLayout(center_layout, 1);

    QVBoxLayout* crs_layout = new QVBoxLayout();
    crs_label_ = new QLabel("CRS");
    crs_label_->setFont(label_font);
    crs_label_->setAlignment(Qt::AlignCenter);

    crs_knob_ = new CRS_knob();
    crs_layout->addWidget(crs_label_);
    crs_layout->addWidget(crs_knob_);
    main_layout_->addLayout(crs_layout);
}

void DCP::setupStylesheet() {
    setStyleSheet("DCP { "
                  "  background-color: #2a2a2a; "
                  "  border-bottom: 2px solid #00a8e8; "
                  "} "
                  "QPushButton { "
                  "  background-color: #1a1a1a; "
                  "  color: #00a8e8; "
                  "  border: 2px solid #00a8e8; "
                  "  border-radius: 4px; "
                  "  font-weight: bold; "
                  "  font-size: 12px; "
                  "} "
                  "QPushButton:hover { "
                  "  background-color: #00a8e8; "
                  "  color: #1a1a1a; "
                  "} "
                  "QPushButton:pressed { "
                  "  background-color: #1a1a1a; "
                  "  color: #00a8e8; "
                  "  border: 2px solid #00a8e8; "
                  "} ");
}
void DCP::connectDataFusionEngineToQNHKnob(DataFusionEngine* engine) {
    connect(qnh_knob_, SIGNAL(qnhChanged(double)), engine,
            SLOT(handleQNHKnobChange(double)));
}
QNH_knob* DCP::getQNH() const {
    return qnh_knob_;
}
CRS_knob* DCP::getCRS() const {
    return crs_knob_;
}
