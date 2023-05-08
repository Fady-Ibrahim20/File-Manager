#include "searchfilewidget.h"
#include <QHeaderView>
#include <QBoxLayout>
#include <QPushButton>
#include <QLineEdit>


searchfilewidget::searchfilewidget(QWidget *parent)
    : QWidget{parent}
{

    this->allocateWidget();
    this->initiateSearchFilePage();
    this->mRegisterSignals();
   // searchString  = "adel" ;

}


void searchfilewidget::recieveSearchContentMapFromCont(const std::multimap<int, std::string>&result)
{
    clearResultTable();
    fillResultTableModel(result);
}

void searchfilewidget::onAddPathButton()
{
    searchPopUp->show();
}

void searchfilewidget::CallSearchContentBE()
{
    // should be called when the search button is pressed
    fillPathVector();
    if (isValidEnteredString())
    {
        emit searchContentPathsSignal(filePaths, searchString);
    }
}

void searchfilewidget::fillResultTableModel(const std::multimap<int, std::string>&result)
{
    // Iterate through the map and add each key-value pair to the table
    for (const auto& pair : result) {
        int row = tableResultWidget->rowCount();
        tableResultWidget->insertRow(row);
        QTableWidgetItem* l_item = new QTableWidgetItem(QString::number(pair.first));
        QTableWidgetItem* r_item = new QTableWidgetItem(QString::fromStdString(pair.second));
        l_item->setTextAlignment(Qt::AlignCenter); // center align the contents of the item
        tableResultWidget->setItem(row, 0, l_item);
        tableResultWidget->setItem(row, 1, r_item);
    }

}

void searchfilewidget::fillPathVector()
{
    filePaths.clear();
    qInfo() <<"iam here *********" ;
    filePaths.push_back("/home/adel/playing/file1.txt");
    filePaths.push_back("/home/adel/playing/file2.txt");
    // iterate over the table to get the entered paths in the table


}

void searchfilewidget::clearResultTable()
{
      tableResultWidget->setRowCount(0);
}

void searchfilewidget::clearPathsTable()
{
      tablePathsWidget->setRowCount(0);
}

void searchfilewidget::mRegisterSignals()
{
      connect(searchButton,&QPushButton::clicked , this , &searchfilewidget::CallSearchContentBE);
      connect(addPathButton,&QPushButton::clicked,this,&searchfilewidget::onAddPathButton);
}

bool searchfilewidget::isValidEnteredString()
{
      bool returnFlag = true ;
      QString temp = searchLine->text() ;
      if ( temp.isEmpty() )
      {
        returnFlag =  false ;
      }
      else
      {
        this->searchString = temp.toStdString();
      }
  return returnFlag ;
}

void searchfilewidget::initiateSearchFilePage()
{
    searchLine->setPlaceholderText("search here");
    searchLine->setAlignment(Qt::AlignCenter);

    // Set the headers for the table
    tableResultWidget->setHorizontalHeaderLabels({"Line Number", "File Path"});
    tablePathsWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("File Path"));

    // Set the size policy for the table widget
    tableResultWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    tablePathsWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Set the width of the columns
    tableResultWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    tableResultWidget->setColumnWidth(0, 100); // left column
    tableResultWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch); // right column

    tablePathsWidget->setColumnWidth(0, 400);

    // Create a horizontal layout for the button and add the button to it

    addPathButton->setStyleSheet("background-color: green");

    removePathButton->setEnabled(false);
    removePathButton->setStyleSheet("background-color: #f0f0f0; color: #808080;");

    button_layout->addWidget(addPathButton);
    button_layout->addWidget(removePathButton);
    lineButtonHlayout->addWidget(searchButton);
    lineButtonHlayout->addWidget(searchLine);
    hLayOut->addLayout(button_layout);
    hLayOut->addWidget(tablePathsWidget);

    // Create a vertical layout and add the button layout and table widget to it

    layout->addLayout(hLayOut);
    layout->addLayout(lineButtonHlayout);

    // Add the table widget and center the layout vertically and horizontally
    layout->addWidget(tableResultWidget);
    layout->setAlignment(Qt::AlignCenter);

}

void searchfilewidget::allocateWidget()
{
    // Create a new QTableWidget with two column
    tableResultWidget = new QTableWidget(0, 2, this);
    tablePathsWidget  = new QTableWidget(0,1,this);
    searchLine = new QLineEdit(this);
    hLayOut = new QHBoxLayout();
    button_layout = new QVBoxLayout();
    lineButtonHlayout = new QHBoxLayout();
    addPathButton= new QPushButton("add path", this);
    removePathButton = new QPushButton("remove path",this);
    searchButton= new QPushButton("search", this);
    layout = new QVBoxLayout(this);
    searchPopUp = new rename_widget("add","please enter a valid file path","\tfile path\t");
}

searchfilewidget::~searchfilewidget()
{
    delete tableResultWidget ;
    delete tablePathsWidget ;
    delete searchLine ;
    delete hLayOut ;
    delete button_layout ;
    delete lineButtonHlayout ;
    delete addPathButton ;
    delete removePathButton ;
    delete searchButton ;
    delete layout ;
    delete searchPopUp ;
}

