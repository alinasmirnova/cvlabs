#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->chooseFile, SIGNAL(clicked()), this, SLOT(chooseFile()));
    connect(ui->findPoints, SIGNAL(clicked()), this, SLOT(findPoints()));

    ui->algo->addItem("Моравек");
    ui->algo->addItem("Харрис");
    ui->algo->setCurrentIndex(0);
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

void MainWindow::findPoints()
{

    vector<Point> points;
    if(ui->algo->currentIndex() == 0)
    {
        points = Detectors::Moravec(*image, ui->windowSize->value(), ui->localMax->value(), ui->minS->value());
    }
    else
    {
        points = Detectors::Harris(*image, ui->windowSize->value(), ui->localMax->value(), ui->minS->value());
    }

    if(ui->suppress->isChecked())
    {
        int pointNum = ui->pointNum->value();
        points = Detectors::AdaptiveNonMaximumSuppression(points, pointNum, max(image->getHeight(), image->getWidth()));
    }

    QImage qImg = image->toQImage();
    QPainter p(&qImg);

    p.setPen(QPen(QColor(Qt::red)));
    p.setBrush(QBrush(QColor(Qt::red)));

    for(uint i=0; i<points.size(); i++) {
        p.fillRect(points[i].x - 1, points[i].y - 1, 3, 3, QColor(Qt::red));
    }

    p.end();

    qImg.save("E:\\Pictures\\1.jpg");
}

MainWindow::~MainWindow()
{
    delete ui;
}
