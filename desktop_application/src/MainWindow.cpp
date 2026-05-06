#include <MainWindow.hpp>
#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("Virtual Cockpit Simulator");
    resize(1000, 700);
    setStyleSheet("QMainWindow { background-color: #333; }");

    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);

    QFrame* top_bar = new QFrame();
    top_bar->setStyleSheet(
        "QFrame { background-color: #555; border-radius: 10px; }");


    QHBoxLayout* top_bar_layout = new QHBoxLayout(top_bar);
    top_bar_layout->addWidget(new QLabel("There will be knobs and buttons here"),1,Qt::AlignCenter);
    layout->addWidget(top_bar);

    QHBoxLayout* content_layout = new QHBoxLayout();
    QGridLayout* left_grid = new QGridLayout();

    left_grid->addWidget(createPanel("FMA", "black"), 0, 0, 1, 3);
    left_grid->addWidget(createPanel("IAS", "black"), 1, 0, 2, 1);
    left_grid->addWidget(createPanel("SZTUCZNY\nHORYZONT", "#7f8c8d"), 1, 1, 1,
                         1);
    left_grid->addWidget(createPanel("HSI", "#9b59b6"), 2, 1, 1, 1);
    left_grid->addWidget(createPanel("ALTITUDE", "black"), 1, 2, 2, 1);

    QVBoxLayout* right_layout = new QVBoxLayout();
    right_layout->addWidget(
        createPanel("Wskaźniki statusu - tylko dla dopełnienia wyglądu",
                    "#5d4037"),
        1);
    right_layout->addWidget(createPanel("Mapa nawigacyjna", "#2c3e50"), 2);

    content_layout->addLayout(left_grid, 2);
    content_layout->addLayout(right_layout, 1);
    layout->addLayout(content_layout);
    setCentralWidget(centralWidget);
}
QFrame* MainWindow::createPanel(const QString& title, const QString& color) {
    QFrame* panel = new QFrame();
    panel->setStyleSheet(
        QString("QFrame { background-color: %1; border-radius: 10px; }")
            .arg(color));
    QVBoxLayout* layout = new QVBoxLayout(panel);
    QLabel* label = new QLabel(title);

    return panel;
}
