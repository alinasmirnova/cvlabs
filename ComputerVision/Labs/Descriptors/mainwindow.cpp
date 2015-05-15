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
    for(uint i=0; i<points.size(); i++)
    {
       descriptors.push_back(generator.getDescriptor(points[i], 16, 4, 8));
    }
    return descriptors;
}

QImage MainWindow::findAndDrawPairs(const Image& img1, const Image& img2,
                        vector<Point> points1, vector<Point> points2,
                        vector<shared_ptr<Descriptor>> desc1, vector<shared_ptr<Descriptor>> desc2)
{
    QImage result = QImage(img1.getWidth() + img2.getWidth() + 1, max(img1.getHeight(), img2.getHeight()), QImage::Format_RGB32);
    result.fill(0);
    QPainter painter(&result);
    painter.setPen(QPen(QColor(Qt::red)));
    painter.drawImage(0,0,img1.toQImage());
    painter.drawImage(img1.getWidth()+1, 0, img2.toQImage());

    for(uint i=0; i<points1.size(); i++) {
        painter.drawRect(points1[i].x - 1, points1[i].y - 1, 3, 3);
    //    painter.drawEllipse(QPoint(points1[i].x, points1[i].y), points1[i].scale*sqrt(2)*2, points1[i].scale*sqrt(2)*2);
    }
    for(uint i=0; i<points2.size(); i++) {
        painter.drawRect(points2[i].x + img1.getWidth(), points2[i].y -1, 3, 3);
    //    painter.drawEllipse(QPoint(points2[i].x, points2[i].y), points2[i].scale*8, points2[i].scale*8);
    }

//    for(uint i=0; i<desc1.size(); i++) {
//        painter.translate(desc1[i]->point.x, desc1[i]->point.y);
//        painter.rotate(-desc1[i]->point.angle);

//        painter.drawRect(-8, -8, 17, 17);
//        painter.rotate(desc1[i]->point.angle);
//        painter.translate(-desc1[i]->point.x, -desc1[i]->point.y);

//    }

//    for(uint i=0; i<desc2.size(); i++) {
//        painter.translate(desc2[i]->point.x + img1.getWidth() + 1, desc2[i]->point.y);
//        painter.rotate(-desc2[i]->point.angle);

//        painter.drawRect(-8, -8, 17, 17);
//        painter.rotate(desc2[i]->point.angle);
//        painter.translate(-desc2[i]->point.x - img1.getWidth() - 1, -desc2[i]->point.y);

//    }

//    for(uint i=0; i<desc2.size(); i++) {
//        painter.drawRect(points2[i].x + img1.getWidth(), points2[i].y, 3, 3);
//    //    painter.drawEllipse(QPoint(points2[i].x, points2[i].y), points2[i].scale*8, points2[i].scale*8);
//    }

    shared_ptr<Descriptor> closest;
    int closestNum;
    for(uint i=0; i<desc1.size(); i++)
    {
        //painter.drawRect(desc1[i]->point.x - 1, desc1[i]->point.y - 1, 3, 3);
        closestNum = desc1[i]->findClosest(desc2);
        if(closestNum != -1)
        {
            closest = desc2[closestNum];
            //painter.drawRect(closest->point.x + img1.getWidth(), closest->point.y - 1, 3, 3);
            painter.setPen(QPen(QColor(rand()%255, rand()%255, rand()%255)));
            painter.drawLine(QPoint(desc1[i]->point.x, desc1[i]->point.y), QPoint(closest->point.x + img1.getWidth() + 1, closest->point.y));
        }
    }
    painter.end();
    return result;
}

QImage MainWindow::drawBestPoints(const Models &models, const Image &img1, const Image& img2)
{
    QImage result = QImage(img1.getWidth() + img2.getWidth() + 1, max(img1.getHeight(), img2.getHeight()), QImage::Format_RGB32);
    result.fill(0);
    QPainter painter(&result);
    painter.setPen(QPen(QColor(Qt::red)));
    painter.drawImage(0,0,img1.toQImage());
    painter.drawImage(img1.getWidth()+1, 0, img2.toQImage());

    for(uint i=0; i<models.best.size(); i++)
    {
        if(i%2 == 0) painter.setPen(QPen(QColor(Qt::red)));
        else painter.setPen(QPen(QColor(Qt::green)));

        painter.drawRect(models.best[i].first.x - 1, models.best[i].first.y - 1, 3, 3);
        painter.drawEllipse(QPoint(models.best[i].first.x, models.best[i].first.y), models.best[i].first.scale*8/1.6, models.best[i].first.scale*8/1.6);

        painter.drawRect(models.best[i].second.x + img1.getWidth(), models.best[i].second.y -1, 3, 3);
        painter.drawEllipse(QPoint(models.best[i].second.x + img1.getWidth() + 1, models.best[i].second.y), models.best[i].second.scale*8/1.6, models.best[i].second.scale*8/1.6);

        painter.drawLine(QPoint(models.best[i].first.x, models.best[i].first.y), QPoint(models.best[i].second.x + img1.getWidth() + 1, models.best[i].second.y));
    }
    painter.end();
    return result;
}

QImage MainWindow::createPanorama(const Image &img1, const Image& img2, float *h)
{
    QImage result = QImage(1500,1500, QImage::Format_RGB32);
    result.fill(0);
    QPainter painter(&result);
    int delta = 50;

    painter.drawImage(delta,delta,img1.toQImage());
    int xInit, yInit, xCur, yCur;

    for(int i=0; i<img2.getWidth(); i++)
    {
        for(int j=0; j<img2.getHeight(); j++)
        {
            xInit = i;
            yInit = j;

            xCur = round((h[0]*xInit + h[1]*yInit + h[2]) / (h[6]*xInit + h[7]*yInit + h[8])) + delta;
            yCur = round((h[3]*xInit + h[4]*yInit + h[5]) / (h[6]*xInit + h[7]*yInit + h[8])) + delta;

            if(xCur >= 0 && xCur <1000 && yCur >= 0 && yCur <1000)
            {
                result.setPixel(xCur, yCur, qRgb(img2.getPixel(j, i),img2.getPixel(j, i),img2.getPixel(j, i)));
                //result.setPixel(xCur, yCur, img2.getPixel(j, i));
            }
        }
    }

    painter.end();
    return result;
}

vector<Point> MainWindow::findScaledPoints(const Pyramid& pyramid)
{
    auto points = Detectors::ScaleInvariant(pyramid, 8);
    return points;//Detectors::AdaptiveNonMaximumSuppression(points, 200, max(image.getHeight(), image.getWidth()));
}

vector<shared_ptr<Descriptor>> MainWindow::findScaledDescriptors(vector<Point> points, const Pyramid& pyramid)
{
    vector<shared_ptr<Descriptor>> descriptors;
    for(uint i=0; i<points.size(); i++)
    {
        descriptors.push_back(pyramid.getDescriptor(points[i], 16, 4, 8));
    }
    return descriptors;
}

void MainWindow::findPoints()
{
    //img2 = Image::fromFile("E:/Pictures/examples/111.png");
    img2 = Image::fromFile("E:/Pictures/examples/04.png");
    //img2 = Image::fromFile("E:/Pictures/examples/scaled.png");
    //img2 = Image::fromFile("E:/Pictures/examples/affin.png");
    //img2  = FilterManager::Filter(*img1, *MaskFactory::Shift(15, Direction::DOWN));
    //img2 = Image::getNoisy(*img1);
    //img2 = Image::changeBrightness(*img1, 30);

    //lab4
//    auto points1 = findPoints(*img1);
//    DescriptorGenerator gen1(*img1);
//    //pair<float,float> angle;
//    for(int i=0; i<points1.size(); i++)
//    {
//        points1[i].angle = gen1.getAngleDescriptor(points1[i], 16)->getMaxAngle().first;
//    }

//    auto points2 = findPoints(*img2);
//    DescriptorGenerator gen2(*img2);
//    for(int i=0; i<points2.size(); i++)
//    {
//        points2[i].angle = gen2.getAngleDescriptor(points2[i], 16)->getMaxAngle().first;
//    }

//    auto desc1 = findDescriptors(*img1, points1);
//    auto desc2 = findDescriptors(*img2, points2);

    //lab5
    qDebug()<<"First pyramid";
    auto pyramid1 = Pyramid::build(*img1, 3, 4, 1.6);
    //pyramid1->saveToFolder("E:/Pictures/1");
    qDebug()<<"Second pyramid";
    auto pyramid2 = Pyramid::build(*img2, 3, 4, 1.6);

    qDebug()<<"First points";
    auto points1 = findScaledPoints(*pyramid1);
    qDebug()<<"Second points";
    auto points2 = findScaledPoints(*pyramid2);

    qDebug()<<"First descriptors";
    auto desc1 = findScaledDescriptors(points1, *pyramid1);
    qDebug()<<"Second descriptors";
    auto desc2 = findScaledDescriptors(points2, *pyramid2);

    qDebug()<<"Drawing";


//    auto result = findAndDrawPairs(*img1, *img2, points1, points2, desc1, desc2);

//    QString savePath = curFolder.absolutePath() + "/descriptors/11.png";
//    result.save(savePath);

    qDebug()<<"Ransac";
    auto models = make_shared<Models>(desc2, desc1);
    auto model = models->RanSaC(1500, 1);
    auto image = createPanorama(*img1, *img2, model);
    QString savePath = curFolder.absolutePath() + "/descriptors/panorama.png";
    image.save(savePath);

    auto result1 =drawBestPoints(*models, *img2, *img1);
    savePath = curFolder.absolutePath() + "/descriptors/best.png";
    result1.save(savePath);
}

MainWindow::~MainWindow()
{
    delete ui;
}
