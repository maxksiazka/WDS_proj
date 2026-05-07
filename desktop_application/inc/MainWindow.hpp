#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP
#include <QFrame>
#include <QMainWindow>
#include "DataFusionEngine.hpp"

class MainWindow : public QMainWindow {
    Q_OBJECT
  private:
    DataFusionEngine* data_fusion_engine_;
    QFrame* createPanel(const QString& title, const QString& color);

  public:
    MainWindow(QWidget* parent = nullptr);
};
#endif // MAINWINDOW_HPP
