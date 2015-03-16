#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->chooseFileButton, SIGNAL(clicked()), this, SLOT(openFile()));
    connect(ui->buildPiramid, SIGNAL(clicked()), this, SLOT(buildPyramid()));
    connect(ui->findL, SIGNAL(clicked()), this, SLOT(findL()));
}

void MainWindow::openFile()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilter(tr("Images (*.png *.xpm *.jpg)"));
    dialog.setViewMode(QFileDialog::Detail);

    QStringList fileNames;
    if (dialog.exec())
    {
        fileNames = dialog.selectedFiles();
        if(fileNames.length() > 0)
        {
            folder = (fileNames[0].split("."))[0];
            ui->fileName->setText(fileNames[0]);
        }
    }
}

void MainWindow::buildPyramid()
{
    image = Image::fromFile(ui->fileName->text());
    if(image != nullptr)
    {
        pyramid = Pyramid::build(*image, ui->octaveNumber->value(), ui->levelNumber->value(), ui->sigma0->value(), ui->sigmaInit->value());
        bool result = pyramid->saveToFolder(folder);
        QMessageBox msgBox;
        if(result)
        {
            msgBox.setText("Piramid was saved to " + folder);
            ui->findL->setEnabled(true);
        }
        else
        {
            msgBox.setText("Error while saving piramid to " + folder);
            ui->findL->setEnabled(false);
        }
        msgBox.exec();
    }
}

void MainWindow::findL()
{
    float l = pyramid->findPixel(ui->X->value(), ui->Y->value(), ui->Sigma->value());
    ui->L->setText(QString::number(l));
}

MainWindow::~MainWindow()
{
    delete ui;
}
