#include "tableview.h"
#include "ui_tableview.h"
#include "stringlistmodel.h"

TableView::TableView(QStringListModel *stringListModel, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TableView)
{
    ui->setupUi(this);
    ui->listView->setModel(stringListModel);

}

TableView::~TableView()
{
    delete ui;
}
