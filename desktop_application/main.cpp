#include "SensorLink.hpp"
#include <QApplication>
int main(int argc, char* argv[]) {
    QCoreApplication a(argc, argv);
    SensorLink link;
    link.start(12345, 5001);

    return a.exec();
}
