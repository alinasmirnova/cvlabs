#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QImage *origin = new QImage("E:\\Pictures\\belka.jpg");
    ui->originPic->setPixmap(QPixmap::fromImage(*origin));


    Image *image = Image::GetFromFile("E:\\Pictures\\belka.jpg");
    ui->customPic->setPixmap(QPixmap::fromImage(*image->toQImage()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
