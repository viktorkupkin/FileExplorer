#ifndef RENAMEDIALOG_H
#define RENAMEDIALOG_H

#include <QDialog>

namespace Ui {
class renameDialog;
}

class renameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit renameDialog(QWidget *parent = nullptr, QString nfn = "New name");
    ~renameDialog();
    void set_source_file_name(QString & str) { source_file_name = str; }
    QString & get_source_file_name() { return source_file_name; }

private:
    Ui::renameDialog *ui;
    QString source_file_name;

private slots:


    void on_renameButton_clicked();

    void on_cancelButton_RenameDialog_clicked();

    void TextChanged(QString);

signals:
    void newNameEntered(QString new_name);
    void cancelRename();
};

#endif // RENAMEDIALOG_H
