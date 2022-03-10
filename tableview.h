#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <QDialog>
#include <QAbstractListModel>
#include <QModelIndex>
#include <QStringListModel>

namespace Ui {
class TableView;
}

class TableView : public QDialog
{
    Q_OBJECT

public:
    explicit TableView(QStringListModel *stringListModel, QWidget *parent = nullptr);
    ~TableView();

private:
    Ui::TableView *ui;
};

#endif // TABLEVIEW_H
