#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->chooseFileButton, SIGNAL(clicked()), this, SLOT(openFile()));
    connect(ui->buildPiramid, SIGNAL(clicked()), this, SLOT(buildPiramid()));
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

void MainWindow::buildPiramid()
{
    image = Image::fromFile(ui->fileName->text());
    if(image != nullptr)
    {
        shared_ptr<Piramida> piramid = Piramida::Build(*image, ui->octaveNumber->value(), ui->levelNumber->value());
        bool result = piramid->saveToFolder(folder);
        QMessageBox msgBox;
        if(result)
        {
            msgBox.setText("Piramid was saved to " + folder);
        }
        else
        {
            msgBox.setText("Error while saving piramid to " + folder);
        }
        msgBox.exec();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
