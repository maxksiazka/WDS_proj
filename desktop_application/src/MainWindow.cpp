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
    pfd_ = new PFD(this);

    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);

    QFrame* top_bar = new QFrame();
    top_bar->setStyleSheet(
        "QFrame { background-color: #555; border-radius: 10px; }");

    QHBoxLayout* top_bar_layout = new QHBoxLayout(top_bar);
    top_bar_layout->addWidget(
        new QLabel("There will be knobs and buttons here"), 1, Qt::AlignCenter);
    layout->addWidget(top_bar);

    QHBoxLayout* content_layout = new QHBoxLayout();

    QVBoxLayout* right_layout = new QVBoxLayout();
    right_layout->addWidget(
        createPanel("Wskaźniki statusu - tylko dla dopełnienia wyglądu",
                    "#5d4037"),
        1);
    right_layout->addWidget(createPanel("Mapa nawigacyjna", "#2c3e50"), 2);

    content_layout->addWidget(pfd_, 2);
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
