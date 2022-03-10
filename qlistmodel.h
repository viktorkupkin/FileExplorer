#ifndef QLISTMODEL_H
#define QLISTMODEL_H

#include <QAbstractListModel>


class QListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit QListModel(QObject *parent = 0);

    //возвращает количество элементов списка
    int rowCount(const QModelIndex &parent = QModelIndex());

    //возвращает значение элемента
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole);

    //сохраняет новое значение элемента
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    //возвращает параметры редактируемости элемента
    Qt::ItemFlags flags(const QModelIndex &index) const;
};

#endif // QLISTMODEL_H
