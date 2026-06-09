#include "MFD.hpp"
#include <QLabel>

static QFrame* createPlaceholder(const QString& title, const QString& color,
                                 QWidget* parent) {
    QFrame* panel = new QFrame(parent);
    panel->setStyleSheet(
        QString(
            "QFrame { background-color: %1; border-radius: 4px; border: 0px; }")
            .arg(color));

    QVBoxLayout* layout = new QVBoxLayout(panel);
    QLabel* label = new QLabel(title, panel);
    label->setStyleSheet(
        "QLabel { color: #ffffff; font-weight: bold; font-size: 13px; }");
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);

    return panel;
}

MFD::MFD(QWidget* parent) : QFrame(parent) {
    setStyleSheet("QFrame { background-color: #0a0a0a; border-radius: 2px; "
                  "border: 1px solid #333; }");

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(4, 4, 4, 4);
    layout->setSpacing(4);
    m_telemetry_panel =
        createPlaceholder("Wskaźniki statusu / Telemetria", "#5d4037", this);
    m_nav_map_panel = createPlaceholder("Mapa nawigacyjna", "#2c3e50", this);

    setupRightPanel(layout);
}

void MFD::setupRightPanel(QVBoxLayout* layout) {
    layout->addWidget(m_telemetry_panel, 1);
    layout->addWidget(m_nav_map_panel, 2);
}
