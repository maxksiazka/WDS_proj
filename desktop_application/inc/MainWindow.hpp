#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP
#include "DCP.hpp"
#include "DataFusionEngine.hpp"
#include "PFD.hpp"
#include <QFrame>
#include <QGridLayout>
#include <QMainWindow>

class MainWindow : public QMainWindow {
    Q_OBJECT
  private:
    PFD* m_pfd;
    DCP* m_dcp;
    QFrame* createPanel(const QString& title, const QString& color);

  public:
    MainWindow(QWidget* parent = nullptr);
    void connectDataFusionEngineToPFD(DataFusionEngine* engine);
    void connectDataFusionEngineToDCP(DataFusionEngine* engine);
};
#endif // MAINWINDOW_HPP
