#include "widget.h"
#include "copydialog.h"
#include "newfolderdialog.h"
#include "renamedialog.h"
#include "ui_widget.h"
#include "tableview.h"
#include "stringlistmodel.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    model_Main = new QFileSystemModel(this);
    MyIconProvider = new QFileIconProvider;
    model_Main->setRootPath("");
    MyIconProvider->setOptions(QFileIconProvider::DontUseCustomDirectoryIcons);
    model_Main->setIconProvider(MyIconProvider);
    model_Main->setFilter(QDir::QDir::AllEntries);
    ui->listViewMain->setModel(model_Main);
    ui->Search_lineEdit->setText("Input search file name");
    connect(ui->Search_lineEdit, SIGNAL(textChanged(QString)), this, SLOT(SearchTextChanged(QString)));//делает кнопку Search неактивной при пустом QLineEdit
    connect(this, SIGNAL(SearchFileNameEntered(QString)), this, SLOT(SeachFileName_fromLineEdit(QString))); //передача текста из line edit при нажатии кнопки Search
}

Widget::~Widget()
{
    delete ui;
    delete MyIconProvider;
}

//Навигация по папкам
void Widget::on_listViewMain_doubleClicked(const QModelIndex &index)
{
    QListView *listView = (QListView*)sender(); //указатель на вызвавший объект
    QFileInfo fileInfo = model_Main->fileInfo(index);
    if (fileInfo.fileName() == "..")
    {
        QDir dir = fileInfo.dir();
        dir.cdUp();
        listView->setRootIndex(model_Main->index(dir.absolutePath()));
    } else if (fileInfo.fileName() == ".")
    {
        listView->setRootIndex(model_Main->index(""));
    } else if (fileInfo.isDir())
    {
        listView->setRootIndex(index);
    }
}

//проверка при создании новой папки и при переименовании существует ли папка или файл с таким же именем
bool Widget::folder_already_exists(int flag)
{
    bool folderExist = false;
    QDir destDir = QDir(model_Main->filePath(ui->listViewMain->rootIndex())); //получение пути текущей папки
    switch (flag)
    {
    case 1: //при создании новой папки
        foreach(QFileInfo destDirInfo, destDir.entryInfoList(QDir::Dirs, QDir::Name))
        {
            if(destDirInfo.fileName() == get_new_folder_name())
            {
                folderExist = true;
            }
        }
    case 2: //при переименовании папки или файла
        foreach(QFileInfo destDirInfo, destDir.entryInfoList(QDir::AllEntries, QDir::Name))
        {
            if(destDirInfo.fileName() == new_file_name)
            {
                folderExist = true;
            }
        }
    case 3: //при перемещении файла или папки
        foreach(QFileInfo destDirInfo, destDir.entryInfoList(QDir::AllEntries, QDir::Name))
        {
            if(destDirInfo.fileName() == cut_File_Name)
            {
                folderExist = true;
            }
        }
    }
    return folderExist;
}


//метод создания новой папки
void Widget::create_new_folder()
{
    if(new_folder_name == "")
    {
        return;
    }else
    {
        QMessageBox *msg = new QMessageBox(this);

        QDir destDir = QDir(model_Main->filePath(ui->listViewMain->rootIndex())); //получение пути текущей папки

        if(folder_already_exists(1)) //проверка на существование папки с таким же именем
        {
            msg->setIcon(QMessageBox::Warning);
            msg->setText("Folder with the same name is already exist.");
        }else if(destDir.mkdir(get_new_folder_name()))
        {
            msg->setIcon(QMessageBox::Information);
            msg->setText("Folder was created.");
        }else
        {
            QFileInfo fileInfo(get_source_file_path());
            if(!fileInfo.isWritable()) //проверка атрибутов файла на доступность чтения
            {
                msg->setIcon(QMessageBox::Warning);
                msg->setText("Folder could not be created.");
                msg->setInformativeText("Check file attributes.");
            }
        }
        set_new_folder_name("");

        msg->exec();
        msg->close();
    }
}

//метод переименования файла
void Widget::rename_file()
{
    QMessageBox *msg = new QMessageBox(this);
    if(new_file_name == "")
    {
        return;
    }
    else
    {

        QFile renamingFile(soure_file_path);
        QString temp = absolut_path + "/" + new_file_name;
        if(folder_already_exists(2))
        {
            msg->setIcon(QMessageBox::Warning);
            msg->setText("File with the same name is already exist.");
        }else if(renamingFile.rename(soure_file_path, temp))
        {
            msg->setIcon(QMessageBox::Information);
            msg->setText("File was renamed.");
        }else
        {
            msg->setIcon(QMessageBox::Warning);
            msg->setText("Rename was failed.");
        }
        new_file_name = "";
    }
    msg->exec();
    msg->close();
}


void Widget::on_listViewMain_clicked(const QModelIndex &index) //обработка клика на элементе списка
{
    QFileInfo fileInfo = model_Main->fileInfo(index);
    soure_file_path = fileInfo.absoluteFilePath();
    soure_file_name = fileInfo.fileName();
    absolut_path = fileInfo.absolutePath();
}


void Widget::on_copyButton_Main_clicked() //вызов окна копирования кликом на кнопке Copy
{
    copyDialog *MyCopyDialog = new copyDialog(this);
    MyCopyDialog->set_soure_file_path(soure_file_path);
    MyCopyDialog->set_dest_file_name(soure_file_name);
    MyCopyDialog->show();
    MyCopyDialog->exec();
    MyCopyDialog->close();
}


void Widget::on_newFolderButton_Main_clicked() //клик на кнопке New folder в главном окне
{
    newFolderDialog *newFolder = new newFolderDialog(this);
    connect(newFolder, SIGNAL(newFolderNameEntered(QString)), this, SLOT(newFolderName_fromDialog(QString)));//получение имени новой папки
    connect(newFolder, SIGNAL(cancelNewFolderCreate()), this, SLOT(cancel_fromNewFolderDialog()));//получение сигнала кнопки Cancel из диалога New folder
    newFolder->show();
    newFolder->exec();
    newFolder->close();

    create_new_folder();
}

void Widget::newFolderName_fromDialog(QString nfn) //public slot
{
    this->set_new_folder_name(nfn);
}

void Widget::cancel_fromNewFolderDialog()
{
    this->set_new_folder_name("");
}

void Widget::on_deleteButton_Main_clicked()
{
    QMessageBox *msg = new QMessageBox(this);
    msg->setIcon(QMessageBox::Critical);
    msg->setText("File will be move to trash.");
    msg->setInformativeText("Are you realy want to do it?");
    msg->setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    msg->setDefaultButton(QMessageBox::Cancel);
    int otvet = msg->exec();
    switch (otvet) {
    case QMessageBox::Cancel:
        break;
    case QMessageBox::Yes:

        QFile tempDir = get_source_file_path();
        QFileInfo fileInfo(tempDir);
        if(fileInfo.isWritable()) //проверка атрибутов файла на доступность чтения
        {
            tempDir.moveToTrash();
        }else
        {
            QMessageBox *msg2 = new QMessageBox(this);
            msg2->setIcon(QMessageBox::Warning);
            msg2->setText("File is not writable.");
            msg2->setInformativeText("Check file attributes.");
            msg2->exec();
            msg->close();
        }
    }
}

void Widget::on_renameButton_Main_clicked()
{
    renameDialog *renameDial = new renameDialog(this, soure_file_name);
    connect(renameDial, SIGNAL(newNameEntered(QString)), this, SLOT(newName_fromRenameDialog(QString)));//принимает сигнал с теkстом из renameDial
    connect(renameDial, SIGNAL(cancelRename()), this, SLOT(cancel_fromRenameDialog()));
    renameDial->show();
    renameDial->exec();
    renameDial->close();

    rename_file();
}

void Widget::newName_fromRenameDialog(QString new_name)
{
    set_new_file_name(new_name);
}

void Widget::cancel_fromRenameDialog()
{
    set_new_file_name("");
}


void Widget::on_SearchButton_clicked()
{
    emit this->SearchFileNameEntered(ui->Search_lineEdit->text()); //получение имени файла для поиска
    QDir dDir = QDir(model_Main->filePath(ui->listViewMain->rootIndex())); //текущая директория
    QFileInfoList searchedFileNameList = QFileInfoList();
    int count = 0;
    RecursiveSearch(dDir, searchedFileNameList, count);//функция поиска

    QMessageBox *msg = new QMessageBox(this);
    QString str_count = QString::number(count);
    switch (count)
    {
    case 0:
    {
        msg->setIcon(QMessageBox::Information);
        msg->setText("Files not found.");
        msg->show();
        msg->exec();
        break;
    }
    default:
    {
        msg->setIcon(QMessageBox::Information);
        msg->setText(str_count + " files found.");
        msg->show();
        msg->exec();
        QStringList searchedFiles_AbsolutPath;
        foreach(QFileInfo fileAbsPath, searchedFileNameList) //заполнение списка QString абсолютными путями найденных файлов
        {
            searchedFiles_AbsolutPath.append(fileAbsPath.absoluteFilePath());
        }

        QStringListModel *stringListModel = new QStringListModel(this);
        stringListModel->setStringList(searchedFiles_AbsolutPath);

        TableView *tv = new TableView(stringListModel);
        tv->show();
        tv->exec();
        delete tv;
        delete stringListModel;
    }
    }


    //проверка полученного списка searchedFileNameList найденных файлов
    //    if(1)
    //    {
    //        foreach(QFileInfo dInfo, searchedFileNameList)
    //        {
    //            qDebug() << dInfo.absoluteFilePath();

    //        }
    //        qDebug() << count;

    //        //проверка searchedFiles_AbsolutPath найденных файлов
    //        QStringList::const_iterator constIter;
    //        for(constIter = searchedFiles_AbsolutPath.begin(); constIter != searchedFiles_AbsolutPath.end(); ++constIter)
    //        {
    //            qDebug() << (*constIter).toLocal8Bit().constData() << Qt::endl;
    //        }
    //    }
}

void Widget::RecursiveSearch(QDir &dDir, QFileInfoList& searchedFileNameList, int &count)
{
    foreach(QFileInfo dInfo, dDir.entryInfoList(QDir::Dirs|QDir::Files|QDir::QDir::NoDotAndDotDot, QDir::Name|QDir::DirsFirst))
    {
        if(dInfo.baseName() == get_seach_file_name())
        {
            count++;
            searchedFileNameList.append(dInfo);
        }
        if(dInfo.isDir() && dDir.cd(dInfo.fileName()))
        {
            //           qDebug() << dInfo.absoluteFilePath();
            RecursiveSearch(dDir, searchedFileNameList, count);
            dDir.cdUp();
        }
    }

    //    QMessageBox *msg = new QMessageBox(this);
    //    QString tmp = QString::number(count);
    //    msg->setText(tmp);
    //    msg->show();
}

void Widget::SeachFileName_fromLineEdit(QString sfn)
{
    set_seach_file_name(sfn);
}
void Widget::SearchTextChanged(QString sfn) //делает кнопку Search неактивной при пустом QLineEdit
{
    ui->SearchButton->setEnabled(!sfn.isEmpty());
}



void Widget::on_cutButton_Main_clicked() //обработка клика на кнопке Cut
{
    cut_File_Path = soure_file_path;
    cut_File_Name = soure_file_name;
    QFileInfo cutFileInfo(cut_File_Path);
    //qDebug() << "cut_File_Path:" << cut_File_Path;
    //qDebug() << "cut_File_Name:" << cut_File_Name;
    QMessageBox * msg = new QMessageBox(this);
    if(cutFileInfo.isReadable()) //проверка можно ли перенести файл
    {
        msg->setIcon(QMessageBox::Information);
        msg->setText("Select a directory to paste item now.");
    }else
    {
        msg->setIcon(QMessageBox::Warning);
        msg->setText("You haven't permission for this operation.");
    }


    msg->setIcon(QMessageBox::Information);
    msg->setText("Select a directory to paste item now.");
    msg->show();
    msg->exec();
}

void Widget::on_pasteButton_Main_clicked() //обработка клика на кнопке Paste
{
    QMessageBox *msg = new QMessageBox(this);
    QDir destDir = QDir(model_Main->filePath(ui->listViewMain->rootIndex())); //получение пути текущей папки
    //qDebug() << "cut_File_Path + cut_File_Name:" << cut_File_Path << "!/!" << cut_File_Name;
    //qDebug() << "soure_file_path:" << soure_file_path;
    QString dest_path = destDir.absoluteFilePath(model_Main->filePath(ui->listViewMain->rootIndex()));//запись пути текущей папки в переменную QString
    //qDebug() << "dest_path:" << dest_path;
    QDir cutFile(cut_File_Path);
    QFileInfo cut_File_info(cut_File_Path);
    if(QFile::copy(cut_File_Path, dest_path + "/" + cut_File_Name))
    {
        //qDebug() << "if was called:";
        if(cut_File_info.isFile())
        {
            QFile::remove(cut_File_Path);
        }
        if(cut_File_info.isDir())
        {
            //qDebug() << "cut_File_Path:" << cutFile.path();
            //cutFile.cdUp();
            //qDebug() << "cut_File_Path cdUp():" << cutFile.path();
            //qDebug() << "cut_File_Name" << cut_File_Name;
            cutFile.removeRecursively();
        }
        msg->setIcon(QMessageBox::Information);
        msg->setText("File was pasted.");

    }else //проверка на существование папки в папке назначения
    {
        //qDebug() << "else was called:";
        if(folder_already_exists(3))
        {
            msg->setIcon(QMessageBox::Warning);
            msg->setText("File allready exist. Operation faild");
        }
        msg->exec();
    }
}

qint64 Widget::folder_size() //рукурсивная функция подсчета размера папки
{
    qint64 dirSize = 0;
    for(QDirIterator itDir(soure_file_path, QDir::NoDotAndDotDot | QDir::Files | QDir::Hidden | QDir::System, QDirIterator::Subdirectories);
        itDir.hasNext(); )
    {
        itDir.next();
        dirSize += itDir.fileInfo().size();
    }
    return dirSize;
}


void Widget::on_GetInfo_Button_clicked() //вывод размера файла или папки
{
    QMessageBox *msg = new QMessageBox(this);
    QFileInfo fileInfo(soure_file_path);
    double size = 0;
    QString units;
    QString dateCreate = fileInfo.birthTime().toString("hh:mm:ss dd.MM.yyyy");
    if(fileInfo.isDir())
    {
        size = folder_size();
    }else
    {
        size = fileInfo.size();
    }

    if(size < 1000)
    {
        units = " bytes.";
    }
    else if(size > 999 & size < 999999)
    {
        size /= 1000;
        units = " KB.";
    }else if(size > 999999 & size < 999999999)
    {
        size /= 1000000;
        units = " MB.";
    }else
    {
        size /= 1000000000;
        units = " GB.";
    }
    QString fileSize;
    fileSize.setNum(size, 'f', 2);
    msg->setIcon(QMessageBox::Information);
    msg->setText("File name: \"" + soure_file_name + "\" \n" + "size: " + fileSize + units + "\n" + "Date create: " + dateCreate);
    msg->show();
    msg->exec();
}
