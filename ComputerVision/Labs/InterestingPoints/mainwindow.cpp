#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->chooseFile, SIGNAL(clicked()), this, SLOT(chooseFile()));
    connect(ui->findMoravek, SIGNAL(clicked()), this, SLOT(findMoravek()));
}

void MainWindow::chooseFile()
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
            ui->fileName->setText(fileNames[0]);
        }
    }
}

void MainWindow::findMoravek()
{
    vector<Point> points = Detectors::Moravec(*image, ui->windowSize->value(), ui->localMax->value(), ui->minS->value());

    QImage qImg = image->toQImage();
    QPainter p(&qImg);

    p.setPen(QPen(QColor(Qt::red)));
    p.setBrush(QBrush(QColor(Qt::red)));

    for(int i=0; i<points.size(); i++) {
        p.fillRect(points[i].x, points[i].y, 1,1, QColor(Qt::red));
    }

    p.end();



    qImg.save("E:\\Pictures\\1.jpg");
}

void MainWindow::findHarris()
{

}

MainWindow::~MainWindow()
{
    delete ui;
}
