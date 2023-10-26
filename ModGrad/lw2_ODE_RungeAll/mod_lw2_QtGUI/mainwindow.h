#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <KPEq/integral.hpp>
#include <KPEq/chart.h>
#include <KPEq_Q_Libs/itemmodel.h>`

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

    void on_calculateBtn_clicked();

private:
    KPEq::Q::ItemModel * initdata_model, * I_T0_m_data_model, * Tk_sigma_model;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
