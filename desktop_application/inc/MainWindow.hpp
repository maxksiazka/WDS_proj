#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP
#include <QFrame>
#include <QMainWindow>
#include <QGridLayout>
#include "DataFusionEngine.hpp"
#include "DCP.hpp"
#include "PFD.hpp"

class MainWindow : public QMainWindow {
    Q_OBJECT
  private:
    PFD* pfd_;
    DCP* dcp_;
    DataFusionEngine* data_fusion_engine_;
    QFrame* createPanel(const QString& title, const QString& color);

  public:
    MainWindow(QWidget* parent = nullptr);
};
#endif // MAINWINDOW_HPP
