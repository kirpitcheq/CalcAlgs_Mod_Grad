#ifndef KPEQ_Q_ITEMMODEL_EXAMPLE_H
#define KPEQ_Q_ITEMMODEL_EXAMPLE_H

#include <QMainWindow>
#include <KPEq_Q_Libs/itemmodel.h>

QT_BEGIN_NAMESPACE
namespace Ui { class KPEq_Q_ItemModel_Example; }
QT_END_NAMESPACE

class KPEq_Q_ItemModel_Example : public QMainWindow
{
    Q_OBJECT

public:
    KPEq_Q_ItemModel_Example(QWidget *parent = nullptr);
    ~KPEq_Q_ItemModel_Example();

private:
    Ui::KPEq_Q_ItemModel_Example *ui;
    KPEq::Q::ItemModel * model;

};
#endif // KPEQ_Q_ITEMMODEL_EXAMPLE_H
