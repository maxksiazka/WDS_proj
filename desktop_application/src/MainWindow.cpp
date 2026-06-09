#include <MainWindow.hpp>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpacerItem>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("Virtual Cockpit Simulator");
    resize(1920, 1080);
    setStyleSheet("QMainWindow { background-color: #333; }");
    m_pfd = new PFD(this);
    m_mfd = new MFD(this);
    m_dcp = new DCP(this);

    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);

    m_dcp->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    layout->addWidget(m_dcp, 0, Qt::AlignTop);

    m_pfd->connectDCPToPFD(m_dcp->getCRS(), m_dcp->getQNH());

    QHBoxLayout* content_layout = new QHBoxLayout();
    content_layout->addWidget(m_pfd, 2);
    content_layout->addWidget(m_mfd, 1);
    layout->addLayout(content_layout);

    setCentralWidget(centralWidget);
}
void MainWindow::connectDataFusionEngineToPFD(DataFusionEngine* engine) {
    m_pfd->connectDataFusionEngineToPFDComponents(engine);
}
void MainWindow::connectDataFusionEngineToDCP(DataFusionEngine* engine) {
    m_dcp->connectDataFusionEngineToQNHKnob(engine);
}
