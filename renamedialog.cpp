#include "renamedialog.h"
#include "ui_renamedialog.h"
#include "widget.h"

renameDialog::renameDialog(QWidget *parent, QString nfn) :
    QDialog(parent),
    ui(new Ui::renameDialog), source_file_name(nfn)
{
    ui->setupUi(this);
    ui->lineEdit_newNameDialog->setText(get_source_file_name());
    connect(ui->lineEdit_newNameDialog, SIGNAL(textChanged(QString)), this, SLOT(TextChanged(QString)));//делает кнопку Rename неактивной при пустом QLineEdit
    connect(ui->renameButton, SIGNAL(clicked()), this, SLOT(on_renameButton_clicked())); //передает текст после нажатия кнопки Rename
    connect(ui->cancelButton_RenameDialog, SIGNAL(clicked()), this, SLOT(on_cancelButton_RenameDialog_clicked()));//передает сигнал кнопки Cancel
}

renameDialog::~renameDialog()
{
    delete ui;
}

void renameDialog::on_renameButton_clicked()
{
    emit this->newNameEntered(ui->lineEdit_newNameDialog->text());
    close();
}

void renameDialog::on_cancelButton_RenameDialog_clicked()
{
    emit this->cancelRename();
    close();
}


void renameDialog::TextChanged(QString nfn)
{
    ui->renameButton->setEnabled(!nfn.isEmpty());
}
