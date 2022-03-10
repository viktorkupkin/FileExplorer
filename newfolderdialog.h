#ifndef NEWFOLDERDIALOG_H
#define NEWFOLDERDIALOG_H

#include <QDialog>
#include <QDir>

namespace Ui {
class newFolderDialog;
}

class newFolderDialog : public QDialog
{
    Q_OBJECT

public:
    explicit newFolderDialog(QWidget *parent = nullptr);
    ~newFolderDialog();
    QString & get_new_folder_name(){ return new_folder_name; }
    QString & get_current_folder_path(){ return current_folder_path; }

private:
    Ui::newFolderDialog *ui;
    QString new_folder_name;
    QString current_folder_path;

private slots:
    void on_cancelButton_newFolder_clicked();
    void on_createButton_newFolder_clicked(); //слот для передачи имени новой папки в главный класс Widget
    void TextChanged(QString);

signals:
    void newFolderNameEntered(QString nfn);
    void cancelNewFolderCreate();
};

#endif // NEWFOLDERDIALOG_H
