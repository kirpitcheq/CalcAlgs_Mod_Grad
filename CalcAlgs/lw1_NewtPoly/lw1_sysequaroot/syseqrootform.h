#ifndef SYSEQROOTFORM_H
#define SYSEQROOTFORM_H

#include <QMainWindow>
#include <QChartView>
#include <QLineSeries>
#include <QStandardItemModel>
#include <KPEq/newtpoly.hpp>
#include <KPEq/newtpolyroots.hpp>
#include <KPEq/sysequasroot.hpp>
#include <KPEq/chart.h>
#include <KPEq/msgboxsmpl.h>

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
    KPEq::Chart *chart;
    QChartView *qchartview;

    std::unique_ptr<KPEq::Interpoll::SysEquasRoot> syseroot;
};
#endif // SYSEQROOTFORM_H
