#include "SensorLink.hpp"
#include <QApplication>
#include "MainWindow.hpp"
int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    SensorLink link;
    w.show();
    link.start(12345, 5001);

    return a.exec();
}
