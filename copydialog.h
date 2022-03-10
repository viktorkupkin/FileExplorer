#ifndef COPYDIALOG_H
#define COPYDIALOG_H

#include <QDialog>
#include <QFileSystemModel>

namespace Ui {
class copyDialog;
}

class copyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit copyDialog(QWidget *parent = nullptr);
    ~copyDialog();

    void set_soure_file_path(QString & sfl) { soure_file_path = sfl; };
    void set_dest_file_name(QString & sfn) { dest_file_name = sfn; };

    QString & return_dest_file_path() { return dest_file_path; }

private slots:
    void on_listView_copyDialog_doubleClicked(const QModelIndex &index);

    void on_listView_copyDialog_clicked(const QModelIndex &index);

    void on_copyButton_copyDialog_clicked();

    void on_cancelButton_copyDialog_clicked();

private:
    Ui::copyDialog *ui;
    QFileSystemModel *model_copyDialog;
    QString soure_file_path;
    QString dest_file_path;
    QString dest_file_name;
};

#endif // COPYDIALOG_H
