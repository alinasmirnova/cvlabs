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
    auto points1 = Detectors::Harris(image, 5, 5, 10);
    return Detectors::AdaptiveNonMaximumSuppression(points1, 50, max(image.getHeight(), image.getWidth()));
}

vector<Descriptor> MainWindow::findDescriptors(const Image& image, vector<Point> points)
{
    vector<Descriptor> descriptors;
    DescriptorGenerator generator(image);
    for(int i=0; i<points.size(); i++)
    {
       descriptors.push_back(generator.getDescriptor(points[i].x, points[i].y, 16, 4, 8));
    }
    return descriptors;
}

QImage MainWindow::findAndDrawPairs(const Image& img1, const Image& img2,
                        vector<Point> points1, vector<Point> points2,
                        vector<Descriptor> desc1, vector<Descriptor> desc2)
{
    QImage result = QImage(img1.getWidth()*2 + 1, img1.getHeight(), QImage::Format_RGB32);
    result.fill(0);
    QPainter painter(&result);
    painter.setPen(QPen(QColor(Qt::red)));
    painter.drawImage(0,0,img1.toQImage());
    painter.drawImage(img1.getWidth()+1, 0, img2.toQImage());

    for(uint i=0; i<points1.size(); i++) {
        painter.drawRect(points1[i].x - 1, points1[i].y - 1, 3, 3);
    }
    for(uint i=0; i<points2.size(); i++) {
        painter.drawRect(points2[i].x + img1.getWidth(), points2[i].y, 3, 3);
    }

    shared_ptr<Descriptor> closest;
    for(int i=0; i<desc1.size(); i++)
    {
        closest = desc1[i].findClosest(desc2);
        if(closest != nullptr)
        {
            painter.drawLine(QPoint(desc1[i].x, desc1[i].y), QPoint(closest->x + img1.getWidth() + 1, closest->y));
        }
    }
    painter.end();
    return result;
}


void MainWindow::findPoints()
{
    //image = FilterManager::SeparatedFilter(*image, *MaskFactory::GaussSeparated(2));
    img2  = FilterManager::Filter(*img1, *MaskFactory::Shift(10, Direction::DOWN));

    auto points1 = findPoints(*img1);
    auto points2 = findPoints(*img2);

    auto desc1 = findDescriptors(*img1, points1);
    auto desc2 = findDescriptors(*img2, points2);

    auto result = findAndDrawPairs(*img1, *img2, points1, points2, desc1, desc2);

    QString savePath = curFolder.absolutePath() + "/descriptors/1.png";
    result.save(savePath);
}

MainWindow::~MainWindow()
{
    delete ui;
}
