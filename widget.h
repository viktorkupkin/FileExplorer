#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QObject>
#include <QTreeWidget>
#include <QFileSystemModel>
#include <QtGui>
#include <QLabel>
#include <QMessageBox>
#include <QFileIconProvider>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    const QString & get_source_file_path(){ return soure_file_path; }
    const QString & get_source_file_name(){ return soure_file_name; }
    void set_new_folder_name(QString nfn = ""){ new_folder_name = nfn; }
    const QString & get_new_folder_name(){ return new_folder_name; }
    void create_new_folder();
    bool folder_already_exists(int);

    void set_new_file_name(QString new_name) { new_file_name = new_name; }

    void rename_file();

    void set_seach_file_name(QString sfn) { SearchFileName = sfn; }
    QString get_seach_file_name() { return SearchFileName; }

    void RecursiveSearch(QDir&, QFileInfoList&, int&);

    void set_cut_file_path(QString sfn) { cut_File_Path = sfn; }
    QString get_cut_file_path() { return cut_File_Path; }

    qint64 folder_size(); //рукурсивная функция подсчета размера папки

private slots:
    void on_listViewMain_doubleClicked(const QModelIndex &index);
    void on_listViewMain_clicked(const QModelIndex &index);
    void on_copyButton_Main_clicked();
    void on_newFolderButton_Main_clicked();
    void on_deleteButton_Main_clicked();
    void on_renameButton_Main_clicked();

    void on_SearchButton_clicked();

    void on_cutButton_Main_clicked();

    void on_pasteButton_Main_clicked();

    void on_GetInfo_Button_clicked();

public slots:
    void newFolderName_fromDialog(const QString nfn);
    void cancel_fromNewFolderDialog();
    void newName_fromRenameDialog(const QString newName);
    void cancel_fromRenameDialog();
    void SeachFileName_fromLineEdit(QString sfn);
    void SearchTextChanged(QString sfn);

signals:
    void SearchFileNameEntered(QString sfn);


private:
    Ui::Widget *ui;
    QFileSystemModel *model_Main;
    QFileIconProvider *MyIconProvider;

    QString absolut_path;
    QString soure_file_path;
    QString soure_file_name;
    QString new_folder_name; //при создании новой папки
    QString new_file_name; //при переименовании
    QString SearchFileName; //при поиске
    QString cut_File_Path; //при вырезании файла
    QString cut_File_Name; //при вырезании фалй
};
#endif // WIDGET_H
