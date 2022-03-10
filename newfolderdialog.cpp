#include "newfolderdialog.h"
#include "ui_newfolderdialog.h"
#include "widget.h"
#include <QMessageBox>

newFolderDialog::newFolderDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newFolderDialog)
{
    ui->setupUi(this);
    ui->lineEdit_NewFolderName->setText("New folder");
//    ui->lineEdit_NewFolderName->backspace();
    connect(ui->lineEdit_NewFolderName, SIGNAL(textChanged(QString)), this, SLOT(TextChanged(QString)));//делает кнопку Create неактивной при пустом QLineEdit
    connect(ui->createButton_newFolder, SIGNAL(clicked()), this, SLOT(on_createButton_newFolder_clicked())); //передает текст после нажатия кнопки Create
    connect(ui->cancelButton_newFolder, SIGNAL(clicked()), this, SLOT(on_cancelButton_newFolder_clicked()));//передает сигнал от нажатия кнопки Cancel
}

newFolderDialog::~newFolderDialog()
{
    delete ui;
}


void newFolderDialog::on_cancelButton_newFolder_clicked()
{
    emit this->cancelNewFolderCreate();
    close();
}

void newFolderDialog::on_createButton_newFolder_clicked()
{
    emit this->newFolderNameEntered(ui->lineEdit_NewFolderName->text());//сигнал
    close();
}


void newFolderDialog::TextChanged(QString nfn)
{
    ui->createButton_newFolder->setEnabled(!nfn.isEmpty());
}
