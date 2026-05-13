#include "SensorLink.hpp"
#include <QApplication>
#include "MainWindow.hpp"
int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    SensorLink link;
    DataFusionEngine data_engine;
    data_engine.connectToSensorLink(&link);
    w.connectDataFusionEngineToPFD(&data_engine);
    link.start(12345, 5001);

    w.show();

    return a.exec();
}
