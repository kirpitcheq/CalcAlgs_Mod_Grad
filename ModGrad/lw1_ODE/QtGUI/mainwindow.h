#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <KPEq/chart.h>
#include <KPEq/ODE.hpp>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_calcBtn_clicked();

    void on_decimalsCountSpin_valueChanged(int arg1);

    void on_analytAprxSpin_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;
    KPEq::Chart * chart;

};
#endif // MAINWINDOW_H

namespace KPEq{
    bool findIntervalValues(RPN_KP &expression, double interval_start, double interval_end, double step, std::list<std::pair<double, double> > &out_vals, bool stepByX = true, KPEq::TwinVectT refCoords = KPEq::TwinVectT());
}
