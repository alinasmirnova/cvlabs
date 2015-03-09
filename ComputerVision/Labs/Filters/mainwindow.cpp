#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    image = nullptr;
    ui->setupUi(this);
    connect(ui->openFile, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(ui->shiftLeft, SIGNAL(triggered()), this, SLOT(leftShift()));
    connect(ui->shiftRight, SIGNAL(triggered()), this, SLOT(rightShift()));
    connect(ui->shiftTop, SIGNAL(triggered()), this, SLOT(topShift()));
    connect(ui->shiftBottom, SIGNAL(triggered()), this, SLOT(bottomShift()));
    connect(ui->blur, SIGNAL(triggered()), this, SLOT(blur()));
    connect(ui->sobelX, SIGNAL(triggered()), this, SLOT(sobelX()));
    connect(ui->sobelY, SIGNAL(triggered()), this, SLOT(sobelY()));
    connect(ui->pruitX, SIGNAL(triggered()), this, SLOT(pruitX()));
    connect(ui->pruitY, SIGNAL(triggered()), this, SLOT(pruitY()));
    connect(ui->sharrX, SIGNAL(triggered()), this, SLOT(sharrX()));
    connect(ui->sharrY, SIGNAL(triggered()), this, SLOT(sharrY()));
    connect(ui->sobel, SIGNAL(triggered()), this, SLOT(sobel()));
    connect(ui->sobelSep, SIGNAL(triggered()), this, SLOT(sobelSep()));
    connect(ui->sobelXSep, SIGNAL(triggered()), this, SLOT(sobelSepX()));
    connect(ui->sobelYSep, SIGNAL(triggered()), this, SLOT(sobelSepY()));
    connect(ui->gauss, SIGNAL(triggered()), this, SLOT(gauss()));
    connect(ui->gaussSep, SIGNAL(triggered()), this, SLOT(gaussSep()));
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
            image = Image::fromFile(fileNames[0]);
            ui->result->setPixmap(QPixmap::fromImage(image->toQImage()));
        }
    }
}

void MainWindow::leftShift()
{
    image = FilterManager::Filter(*image, *MaskFactory::Shift(10,Direction::LEFT));
    ui->result->setPixmap(QPixmap::fromImage(image->toQImage()));
}

void MainWindow::rightShift()
{
    image = FilterManager::Filter(*image, *MaskFactory::Shift(10,Direction::RIGHT));
    ui->result->setPixmap(QPixmap::fromImage(image->toQImage()));
}
void MainWindow::topShift()
{
    image = FilterManager::Filter(*image, *MaskFactory::Shift(10,Direction::UP));
    ui->result->setPixmap(QPixmap::fromImage(image->toQImage()));
}
void MainWindow::bottomShift()
{
    image = FilterManager::Filter(*image, *MaskFactory::Shift(10,Direction::DOWN));
    ui->result->setPixmap(QPixmap::fromImage(image->toQImage()));
}
void MainWindow::blur()
{
    image = FilterManager::Filter(*image, *MaskFactory::Blur(4,9));
    ui->result->setPixmap(QPixmap::fromImage(image->toQImage()));
}
void MainWindow::sobelX()
{
    image = FilterManager::Filter(*image, *MaskFactory::Sobel(Asix::X));
    ui->result->setPixmap(QPixmap::fromImage(image->toQImage()));
}
void MainWindow::sobelY()
{
    image = FilterManager::Filter(*image, *MaskFactory::Sobel(Asix::Y));
    ui->result->setPixmap(QPixmap::fromImage(image->toQImage()));
}
void MainWindow::pruitX()
{
    image = FilterManager::Filter(*image, *MaskFactory::Pruit(Asix::X));
    ui->result->setPixmap(QPixmap::fromImage(image->toQImage()));
}
void MainWindow::pruitY()
{
    image = FilterManager::Filter(*image, *MaskFactory::Pruit(Asix::Y));
    ui->result->setPixmap(QPixmap::fromImage(image->toQImage()));
}
void MainWindow::sharrX()
{
    image = FilterManager::Filter(*image, *MaskFactory::Shar(Asix::X));
    ui->result->setPixmap(QPixmap::fromImage(image->toQImage()));
}
void MainWindow::sharrY()
{
    image = FilterManager::Filter(*image, *MaskFactory::Shar(Asix::Y));
    ui->result->setPixmap(QPixmap::fromImage(image->toQImage()));
}

void MainWindow::sobel()
{
    image = FilterManager::SobelOperator(*image);
    ui->result->setPixmap(QPixmap::fromImage(image->toQImage()));
}
void MainWindow::sobelSepX()
{
    image = FilterManager::SeparatedFilter(*image, *MaskFactory::SobelSeparated(Asix::X));
    ui->result->setPixmap(QPixmap::fromImage(image->toQImage()));
}

void MainWindow::sobelSepY()
{
    image = FilterManager::SeparatedFilter(*image, *MaskFactory::SobelSeparated(Asix::Y));
    ui->result->setPixmap(QPixmap::fromImage(image->toQImage()));
}
void MainWindow::sobelSep()
{
    image = FilterManager::SobelOperatorSeparated(*image);
    ui->result->setPixmap(QPixmap::fromImage(image->toQImage()));
}
void MainWindow::gauss()
{
    image = FilterManager::Filter(*image, *MaskFactory::Gauss(5), true);
    ui->result->setPixmap(QPixmap::fromImage(image->toQImage()));
}

void MainWindow::gaussSep()
{
    image = FilterManager::SeparatedFilter(*image, *MaskFactory::GaussSeparated(5), true);
    ui->result->setPixmap(QPixmap::fromImage(image->toQImage()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
