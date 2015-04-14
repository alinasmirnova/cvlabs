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
            curFolder = dialog.directory();
        }
    }
}

vector<Point> MainWindow::findPointsForImage(const Image& img, QString saveFolder, QString saveName)
{
    vector<Point> points;
    if(ui->algo->currentIndex() == 0)
    {
        points = Detectors::Moravec(img, ui->windowSize->value(), ui->localMax->value(), ui->minS->value());
    }
    else
    {
        points = Detectors::Harris(img, ui->windowSize->value(), ui->localMax->value(), ui->minS->value());
    }

    if(ui->suppress->isChecked())
    {
        int pointNum = ui->pointNum->value();
        points = Detectors::AdaptiveNonMaximumSuppression(points, pointNum, max(image->getHeight(), image->getWidth()));
    }

    QImage qImg = img.toQImage();
    QPainter p(&qImg);

    p.setPen(QPen(QColor(Qt::red)));
    p.setBrush(QBrush(QColor(Qt::red)));

    for(uint i=0; i<points.size(); i++) {
        p.fillRect(points[i].x - 1, points[i].y - 1, 3, 3, QColor(Qt::red));
    }

    p.end();
    if(!curFolder.exists(saveFolder))
    {
        curFolder.mkdir(saveFolder);
    }
    QString savePath = curFolder.absolutePath() + "/" + saveFolder + "/" + saveName;
    qImg.save(savePath);
    return points;
}

void MainWindow::findPoints()
{
    findPointsForImage(*image, "points", "init.jpg");

    shared_ptr<Image> shifted = FilterManager::Filter(*image, *MaskFactory::Shift(10,Direction::LEFT));
    findPointsForImage(*shifted, "points", "shifted.jpg");

    shared_ptr<Image> noisy = Image::getNoisy(*image);
    findPointsForImage(*noisy, "points", "noisy.jpg");

    shared_ptr<Image> bright = Image::changeBrightness(*image, 30);
    findPointsForImage(*bright, "points", "bright.jpg");

    shared_ptr<Image> contrast = Image::changeContrast(*image, 1.5);
    findPointsForImage(*contrast, "points", "contrast.jpg");
}

MainWindow::~MainWindow()
{
    delete ui;
}
