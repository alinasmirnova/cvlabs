#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->openFile, SIGNAL(triggered()), this, SLOT(openFile()));
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
            QWidget::setWindowTitle(fileNames[0]);
            resultImage = FilterManager::Filter(Image::fromFile(fileNames[0]), MaskFactory::Sobel(Asix::X));

            ui->result->setPixmap(QPixmap::fromImage(resultImage->toQImage()));
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
