#include "itemmodel.h"

namespace KPEq {
namespace Q {

void someFunc(){
    KPEq::Q::Tcont items = {
        {2,3,4,5},
        {6,7,8,9},
        {0,1,2,3},
        {1,3,4,5},
        };
    ItemModel model(items);
}

int ItemModel::rowCount(const QModelIndex &parent) const { return items.size(); }

int ItemModel::columnCount(const QModelIndex &parent) const {
    int maxsize = 0;
    for(auto item : items){
        auto item_size = item.size();
        if(item_size > maxsize) maxsize = item_size;
    }
    return maxsize;
}

QVariant ItemModel::data(const QModelIndex &index, int role) const {
    if(index.isValid() && (role == Qt::DisplayRole || role == Qt::EditRole) )
        return items[index.row()][index.column()]; //but here may be need model
    return QVariant();
}

QVariant ItemModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if(role == Qt::DisplayRole && role == Qt::EditRole){
        if(orientation == Qt::Horizontal)
            return QString("Row $1").arg(section);
        else if (orientation == Qt::Vertical)
            return QString("Column $1").arg(section);
    }
    return QVariant();
}

Qt::ItemFlags ItemModel::flags(const QModelIndex &index) const {
    if(!index.isValid()) return Qt::ItemIsEnabled;
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool ItemModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if(index.isValid() && role == Qt::EditRole){
        items[index.row()][index.column()] = (Tval)value.toDouble();
        emit dataChanged(index,index);
        return true;
    }
    return false;
}

bool ItemModel::insertRows(int position, int rows, const QModelIndex &index) {
    beginInsertRows(QModelIndex(), position, position+rows-1);
    if(index.isValid()){
        Tcontrow rowitem(this->rowCount(), 0);
        items.insert(items.begin() + position, rows, rowitem);
    }
    endInsertRows();
    return true;
}

bool ItemModel::removeRows(int position, int rows, const QModelIndex &index)
{
    beginRemoveRows(QModelIndex(), position, position+rows-1);
    if(index.isValid()){
        items.erase(items.begin()+position, items.begin() + position + rows -1);
    }
    endRemoveRows();
    return true;
}

bool ItemModel::insertColumns(int column, int count, const QModelIndex &index)
{
    beginInsertColumns(QModelIndex(), column, column+count-1);
    if(index.isValid()){
        Tval def_value = 0;
        for(auto item : items){
            item.insert(item.begin() + column, count, def_value);
        }
    }
    endInsertColumns();
    return true;
}

bool ItemModel::removeColumns(int column, int count, const QModelIndex &index)
{
    beginRemoveColumns(QModelIndex(), column, column+count-1);
    if(index.isValid()){
        Tval def_value = 0;
        for(auto item : items){
            item.erase(item.begin() + column, item.begin() + column + count - 1);
        }
    }
    endRemoveColumns();
    return true;
}

QModelIndex ItemModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!hasIndex(row, column))
        return QModelIndex();
    if(!parent.isValid())
        return createIndex(row,column,nullptr);
    return QModelIndex();
}

QModelIndex ItemModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child);
    return QModelIndex();
}

} // namespace Q
} // namespace KPEq
