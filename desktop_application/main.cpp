#include "SensorLink.hpp"
#include <QApplication>
#include <QThread>
#include "MainWindow.hpp"
int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    SensorLink link;
    link.start(12345, 5001);

    QThread data_thread;
    DataFusionEngine data_engine;
    data_engine.moveToThread(&data_thread);
    data_engine.connectToSensorLink(&link);
    QObject::connect(&data_thread, SIGNAL(finished()),&data_thread, SLOT(deleteLater()));
    data_thread.start();

    MainWindow w;
    w.connectDataFusionEngineToPFD(&data_engine);
    w.connectDataFusionEngineToDCP(&data_engine);
    w.connectSensorLinkToMFD(&link);
    w.connectDataFusionEngineToMFD(&data_engine);
    w.show();
    return a.exec();
}
