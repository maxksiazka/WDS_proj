#include <MainWindow.hpp>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpacerItem>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("Virtual Cockpit Simulator");
    resize(700, 1000);
    setStyleSheet("QMainWindow { background-color: #333; }");
    m_pfd = new PFD(this);
    m_dcp = new DCP(this);

    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);

    m_dcp->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    layout->addWidget(m_dcp, 0, Qt::AlignTop);

    QHBoxLayout* content_layout = new QHBoxLayout();

    QVBoxLayout* right_layout = new QVBoxLayout();
    right_layout->addWidget(
        createPanel("Wskaźniki statusu - tylko dla dopełnienia wyglądu",
                    "#5d4037"),
        1);
    right_layout->addWidget(createPanel("Mapa nawigacyjna", "#2c3e50"), 2);

    content_layout->addWidget(m_pfd, 2);
    content_layout->addLayout(right_layout, 1);
    layout->addLayout(content_layout);
    setCentralWidget(centralWidget);
}
QFrame* MainWindow::createPanel(const QString& title, const QString& color) {
    QFrame* panel = new QFrame();
    panel->setStyleSheet(
        QString(
            "QFrame { background-color: %1; border-radius: 0px; border: 0px; }")
            .arg(color));
    QVBoxLayout* layout = new QVBoxLayout(panel);

    return panel;
}
void MainWindow::connectDataFusionEngineToPFD(DataFusionEngine* engine) {
    m_pfd->connectDataFusionEngineToArtificialHorizon(engine);
}
void MainWindow::connectDataFusionEngineToDCP(DataFusionEngine* engine) {
    m_dcp->connectDataFusionEngineToQNHKnob(engine);
}
