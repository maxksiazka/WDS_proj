#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP
#include <QFrame>
#include <QMainWindow>

class MainWindow : public QMainWindow {
    Q_OBJECT
  private:
    QFrame* createPanel(const QString& title, const QString& color);

  public:
    MainWindow(QWidget* parent = nullptr);
};
#endif // MAINWINDOW_HPP
