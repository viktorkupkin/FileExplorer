
#include "stringlistmodel.h"

/*Мы хотим, чтобы количество строк в модели было таким же,
как количество строк в списке строк. Помня об этом, мы реализуем функцию rowCount()*/
int StringListModel :: rowCount(const QModelIndex & parent) const
{
    return stringList.count();
}

/*Для элементов в представлении мы хотим вернуть строки в списке строк.
Функция data () отвечает за возврат элемента данных,
который соответствует аргументу индекса:*/
QVariant StringListModel::data(const QModelIndex & index, int role) const
{
    if(!index.isValid())
        return QVariant();
    if(index.row() >= stringList.size())
        return QVariant();
    if(role == Qt::DisplayRole)
        return stringList.at(index.row());
    else
        return QVariant();
}

/*Некоторые представления, такие как QTreeView и QTableView,
могут отображать заголовки вместе с данными элемента.
Если наша модель отображается в виде с заголовками, мы хотим,
чтобы заголовки отображали номера строк и столбцов. Мы можем предоставить информацию о заголовках,
создав подкласс функции headerData () :*/
QVariant StringListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
    {
        return QVariant();
    }
    if(orientation == Qt::Horizontal)
    {
        return QStringLiteral("Column %1").arg(section);
    }
    else
    {
        return QStringLiteral("Row %1").arg(section);
    }
}
