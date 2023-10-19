#ifndef SYSEQROOTFORM_H
#define SYSEQROOTFORM_H

#include <QMainWindow>
#include <QChartView>
#include <QLineSeries>
#include <QStandardItemModel>
#include "../lw1_lib/modules/newtpoly.hpp"
#include "../lw1_lib/modules/newtpolyroots.hpp"
#include "../lw1_lib/modules/sysequasroot.hpp"
#include "../lw1_libQt/MsgBoxSmpl/msgboxsmpl.h"
#include "../lw1_libQt/Chart/chart.h"

QT_BEGIN_NAMESPACE
namespace Ui { class SysEqRootForm; }
QT_END_NAMESPACE

class SysEqRootForm : public QMainWindow
{
    Q_OBJECT

public:
    SysEqRootForm(QWidget *parent = nullptr);
    ~SysEqRootForm();

private slots:
    void on_btnCalcAux_clicked();

private:
    Ui::SysEqRootForm *ui;
    Chart *chart;
    QChartView *qchartview;

    std::unique_ptr<InterpKPEq::SysEquasRoot> syseroot;
};
#endif // SYSEQROOTFORM_H
