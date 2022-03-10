#include "copydialog.h"
#include "ui_copydialog.h"
#include <QFile>
#include <QMessageBox>

copyDialog::copyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::copyDialog)
{
    ui->setupUi(this);
    model_copyDialog = new QFileSystemModel(this);
    model_copyDialog->setFilter(QDir::QDir::AllEntries);
    model_copyDialog->setRootPath("");
    ui->listView_copyDialog->setModel(model_copyDialog);
}

copyDialog::~copyDialog()
{
    delete ui;
}

//Навигация по папкам при копировании
void copyDialog::on_listView_copyDialog_doubleClicked(const QModelIndex &index)
{
    QListView *listView = (QListView*)sender();
    QFileInfo fileInfo = model_copyDialog->fileInfo(index);
    if (fileInfo.fileName() == "..")
    {
        QDir dir = fileInfo.dir();
        dir.cdUp();
        listView->setRootIndex(model_copyDialog->index(dir.absolutePath()));
    } else if (fileInfo.fileName() == ".")
    {
        listView->setRootIndex(model_copyDialog->index(""));
    } else if (fileInfo.isDir())
    {
        listView->setRootIndex(index);
    }
}


void copyDialog::on_listView_copyDialog_clicked(const QModelIndex &index)
{
    QFileInfo fileInfo = model_copyDialog->fileInfo(index);
    dest_file_path = fileInfo.absoluteFilePath();
}


void copyDialog::on_copyButton_copyDialog_clicked()
{
    QMessageBox *msg = new QMessageBox(this);
    if(QFile::copy(soure_file_path, dest_file_path + "/" + dest_file_name))
    {
//        qDebug() << soure_file_path;
//        qDebug() << dest_file_path + "!/!" + dest_file_name;
        msg->setIcon(QMessageBox::Information);
        msg->setText("File was copied.");
    }else
    {
        QDir destDir = QDir(model_copyDialog->filePath(ui->listView_copyDialog->rootIndex())); //получение пути текущей папки
        foreach(QFileInfo destDirInfo, destDir.entryInfoList(QDir::AllEntries, QDir::Name))
        {
            if(destDirInfo.fileName() == dest_file_name)
            {
                msg->setIcon(QMessageBox::Warning);
                msg->setText("File allready exist. Operation faild");
            }
        }
    msg->exec();
    close();
    }
}


void copyDialog::on_cancelButton_copyDialog_clicked()
{
    close();
}

