#ifndef PFD_HPP_
#define PFD_HPP_

#include "ArtificialHorizon.hpp"
#include <QFrame>
#include <QWidget>
#include <QGridLayout>

class PFD : public QFrame {
    Q_OBJECT
  private:
    ArtificialHorizon* artificial_horizon_;
    void setupLeftPanel(QGridLayout* grid);
  public:
    explicit PFD(QWidget* parent = nullptr);
    ~PFD() = default;

};

#endif // PFD_HPP_
