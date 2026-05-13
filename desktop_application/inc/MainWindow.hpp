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
    PFD* pfd_;
    DCP* dcp_;
    DataFusionEngine* data_fusion_engine_;
    QFrame* createPanel(const QString& title, const QString& color);

  public:
    MainWindow(QWidget* parent = nullptr);
    void connectDataFusionEngineToPFD(DataFusionEngine* engine);
};
#endif // MAINWINDOW_HPP
