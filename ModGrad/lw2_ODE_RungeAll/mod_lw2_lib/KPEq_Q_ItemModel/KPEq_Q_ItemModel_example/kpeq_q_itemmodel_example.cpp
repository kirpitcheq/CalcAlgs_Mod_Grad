#include "kpeq_q_itemmodel_example.h"
#include "./ui_kpeq_q_itemmodel_example.h"
#include <KPEq_Q_Libs/itemmodel.h>

KPEq_Q_ItemModel_Example::KPEq_Q_ItemModel_Example(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::KPEq_Q_ItemModel_Example)
{
    ui->setupUi(this);

    KPEq::Q::Tcont items = {
        {5,4,3,2},
        {6,3,4,2},
        {2,3,4,2},
        {5,3,4,1},
    };

    auto model = new KPEq::Q::ItemModel(items);

    ui->tableView->setModel(model);
}

KPEq_Q_ItemModel_Example::~KPEq_Q_ItemModel_Example()
{
    delete ui;
}
