#ifndef KPEQ_Q_ITEMMODEL_H
#define KPEQ_Q_ITEMMODEL_H

#include <QAbstractItemModel>
#include <QObject>
#include <QWidget>

namespace KPEq {
namespace Q {

using Tval = double;
using Tcontrow = std::vector<Tval>;
using Tcont = std::vector<Tcontrow>;
class ItemModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    ItemModel(const Tcont& items, QObject *parent = nullptr) : QAbstractItemModel{parent}, items(items){}
//    explicit ItemModel(QObject *parent = nullptr) : QAbstractItemModel{parent}{}
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
    bool insertColumns(int column, int count, const QModelIndex &index = QModelIndex()) override;
    bool removeColumns(int column, int count, const QModelIndex &index = QModelIndex()) override;

    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;

    ~ItemModel() override = default;

    Tcontrow& operator[](int rowitems){ return items[rowitems]; }
private:
    Tcont items;
};

} // namespace Q
} // namespace KPEq

#endif // KPEQ_Q_ITEMMODEL_H
