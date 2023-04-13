#include "view.h"
#include "./ui_view.h"
#include "controller.h"

void View::mRegisterSignals()
{
    // create a QShortcut object for Ctrl+C
    QShortcut *shortcutCopy = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_C), this);
    // create a QShortcut object for Ctrl+V
    QShortcut *shortcutPaste = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_V), this);
    QShortcut *shortcutDel = new QShortcut(QKeySequence(Qt::Key_Delete), this);
    // connect the shortcuts to your copy and paste functions
    QObject::connect(shortcutCopy, &QShortcut::activated, this, &View::onCopy);
    QObject::connect(shortcutPaste, &QShortcut::activated, this, &View::onPaste);
    QObject::connect(shortcutDel, &QShortcut::activated, this, &View::onDel);
}

View::View(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::View)
{
    ui->setupUi(this);
    fileSystemModel = new QFileSystemModel(this);
    mRegisterSignals();
    TreeView();
    contentUi = new FileContentView(this);
}

void View::TreeView()
{

    QModelIndex modelIndex = fileSystemModel->setRootPath(QDir::rootPath());
    ui->treeView->setModel(fileSystemModel);
    //    ui->treeView->horizontalScrollBar();
    ui->treeView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    //hide the columns we don't want
    /*The first column shows the names of the files and folders in the hierarchy.
     * The second column shows the size of the files in bytes.
     * The third column shows the type of the files (e.g. directory, file, symlink, etc.).
     * The fourth column shows the date and time when the files were last modified.
     */
    for (int i=1; i<4; ++i) ui->treeView->hideColumn(i);
    ui->treeView->header()->setStretchLastSection(true);
    ui->treeView->setColumnWidth(0, 1000);
}

View::~View()
{
    delete ui;
}


void View::on_treeView_clicked(const QModelIndex &index)
{

    this->index = index;
    ui->tableView->setModel(fileSystemModel);
    ui->tableView->setRootIndex(index);
    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->setColumnWidth(0,250);
    ui->tableView->setColumnWidth(3,250);
    ui->tableView->horizontalScrollBar();
    ui->tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

}


void View::on_tableView_clicked(const QModelIndex &index)
{
    this->index = index;
//    on_treeView_clicked(index);
    qInfo() << "line 71";
}
void View::onCopy()
{
        filePath = fileSystemModel->filePath(index);
}

void View::onPaste()
{

        emit copyFile(filePath.toStdString(), fileSystemModel->filePath(index).toStdString());
}

void View::onDel()
{
        filePath = fileSystemModel->filePath(index);
        emit delFile(filePath.toStdString());
}





void View::on_lineEditPath_textEdited(const QString &arg1)
{

}


void View::on_tableView_doubleClicked(const QModelIndex &index)
{
        qInfo() << "95";
        this->index = index;
        on_treeView_clicked(index);
            // Get the path of the selected file

        filePath = fileSystemModel->filePath(index);
        qInfo() << filePath;
        // Open the file and read its contents
        contentUi->file = new QFile (filePath);
        if (!contentUi->file->open(QIODevice::ReadWrite))
        return;
        QString fileContents = contentUi->file->readAll();
        //        file.close();
        //| QIODevice::Text

        // Display the contents of the file in a QTextEdit widget
        contentUi->ui->textEdit->clear();
        contentUi->ui->textEdit->setPlainText(fileContents);
        contentUi->show();
}

