#include "syseqrootform.h"
#include "./ui_syseqrootform.h"

SysEqRootForm::SysEqRootForm(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SysEqRootForm)
{
    ui->setupUi(this);
    chart = new KPEq::Chart();
    qchartview = new QChartView(chart, this);
    qchartview->setRenderHints(QPainter::Antialiasing);
    ui->chartForm->addWidget(qchartview);
}

SysEqRootForm::~SysEqRootForm()
{
    delete ui;
}


void SysEqRootForm::on_btnCalcAux_clicked()
{
    // syseroot = std::make_unique();
        
}

