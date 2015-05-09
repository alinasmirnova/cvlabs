#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->choose_b, SIGNAL(clicked()), this, SLOT(chooseFile()));
    connect(ui->start_b, SIGNAL(clicked()), this, SLOT(findPoints()));
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
            img1 = Image::fromFile(fileNames[0]);
            ui->image->setText(fileNames[0]);
            curFolder = dialog.directory();
        }
    }
}

vector<Point> MainWindow::findPoints(const Image& image)
{
    auto points = Detectors::Harris(image, 5, 5, 0);
    return Detectors::AdaptiveNonMaximumSuppression(points, 50, max(image.getHeight(), image.getWidth()));
}

vector<shared_ptr<Descriptor>> MainWindow::findDescriptors(const Image& image, vector<Point> points)
{
    vector<shared_ptr<Descriptor>> descriptors;
    DescriptorGenerator generator(image);
    for(int i=0; i<points.size(); i++)
    {
       descriptors.push_back(generator.getDescriptor(points[i], 16, 4, 8));
    }
    return descriptors;
}

QImage MainWindow::findAndDrawPairs(const Image& img1, const Image& img2,
                        vector<Point> points1, vector<Point> points2,
                        vector<shared_ptr<Descriptor>> desc1, vector<shared_ptr<Descriptor>> desc2)
{
    QImage result = QImage(img1.getWidth()*2 + 1, img1.getHeight(), QImage::Format_RGB32);
    result.fill(0);
    QPainter painter(&result);
    painter.setPen(QPen(QColor(Qt::red)));
    painter.drawImage(0,0,img1.toQImage());
    painter.drawImage(img1.getWidth()+1, 0, img2.toQImage());

    for(uint i=0; i<points1.size(); i++) {
        painter.drawRect(points1[i].x - 1, points1[i].y - 1, 3, 3);
     //   painter.drawEllipse(QPoint(points1[i].x, points1[i].y), points1[i].scale*8, points1[i].scale*8);
    }
    for(uint i=0; i<points2.size(); i++) {
        painter.drawRect(points2[i].x + img1.getWidth(), points2[i].y, 3, 3);
    //    painter.drawEllipse(QPoint(points2[i].x, points2[i].y), points2[i].scale*8, points2[i].scale*8);
    }

    shared_ptr<Descriptor> closest;
    int closestNum;
    for(int i=0; i<desc1.size(); i++)
    {
        closestNum = desc1[i]->findClosest(desc2);
        if(closestNum != -1)
        {
            closest = desc2[closestNum];
            painter.setPen(QPen(QColor(rand()%255, rand()%255, rand()%255)));
            painter.drawLine(QPoint(desc1[i]->point.x, desc1[i]->point.y), QPoint(closest->point.x + img1.getWidth() + 1, closest->point.y));
        }
    }
    painter.end();
    return result;
}

vector<Point> MainWindow::findScaledPoints(const Image& image, const Pyramid& pyramid)
{
    auto points = Detectors::ScaleInvariant(image, pyramid, 8);
    return points;//Detectors::AdaptiveNonMaximumSuppression(points, 200, max(image.getHeight(), image.getWidth()));
}

vector<shared_ptr<Descriptor>> MainWindow::findScaledDescriptors(vector<Point> points, const Pyramid& pyramid)
{
    vector<shared_ptr<Descriptor>> descriptors;
    for(int i=0; i<points.size(); i++)
    {
        descriptors.push_back(pyramid.getDescriptor(points[i], 16, 4, 8));
    }
    return descriptors;
}

void MainWindow::findPoints()
{
    img2 = Image::fromFile("E:/Pictures/examples/scaled.png");
    //img2  = FilterManager::Filter(*img1, *MaskFactory::Shift(10, Direction::DOWN));
    //img2 = Image::getNoisy(*img1);
    //img2 = Image::changeBrightness(*img1, 30);

    //lab4
//    auto points1 = findPoints(*img1);
//    auto points2 = findPoints(*img2);

//    auto desc1 = findDescriptors(*img1, points1);
//    auto desc2 = findDescriptors(*img2, points2);

    //lab5
    qDebug()<<"First pyramid";
    auto pyramid1 = Pyramid::build(*img1, 6, 4);
    pyramid1->saveToFolder("E:/Pictures/1");
    qDebug()<<"Second pyramid";
    auto pyramid2 = Pyramid::build(*img2, 6, 4);

    qDebug()<<"First points";
    auto points1 = findScaledPoints(*img1, *pyramid1);
    qDebug()<<"Second points";
    auto points2 = findScaledPoints(*img2, *pyramid2);

    qDebug()<<"First descriptors";
    auto desc1 = findScaledDescriptors(points1, *pyramid1);
    qDebug()<<"Second descriptors";
    auto desc2 = findScaledDescriptors(points2, *pyramid2);

    qDebug()<<"Drawing";


    auto result = findAndDrawPairs(*img1, *img2, points1, points2, desc1, desc2);

    QString savePath = curFolder.absolutePath() + "/descriptors/1.png";
    result.save(savePath);
}

MainWindow::~MainWindow()
{
    delete ui;
}
