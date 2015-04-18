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
            image = Image::fromFile(fileNames[0]);
            ui->image->setText(fileNames[0]);
            curFolder = dialog.directory();
        }
    }
}

void MainWindow::findPoints()
{
    //image = FilterManager::SeparatedFilter(*image, *MaskFactory::GaussSeparated(2));
    shared_ptr<Image> noisy  = FilterManager::Filter(*image, *MaskFactory::Shift(10, Direction::DOWN));
    noisy->toQImage().save("E:/1.png");
    vector<shared_ptr<Descriptor>> desk1, desk2;

    auto genFirst = make_shared<DescriptorGenerator>(*image);
    auto genSecond = make_shared<DescriptorGenerator>(*noisy);

    //find points for first image
    qDebug() << "First";
    auto points1 = Detectors::Harris(*image, 5, 5, 10);
    points1 = Detectors::AdaptiveNonMaximumSuppression(points1, 50, max(image->getHeight(), image->getWidth()));

    QImage img1 = image->toQImage();

    for(int i=0; i<points1.size(); i++)
    {
       desk1.push_back(genFirst->getDescriptor(points1[i].x, points1[i].y, 16, 4, 8));
    }

    //find points for second image
    qDebug() << "Second";
    auto points2 = Detectors::Harris(*noisy, 5, 5, 10);
    points2 = Detectors::AdaptiveNonMaximumSuppression(points2, 50, max(image->getHeight(), image->getWidth()));
    QImage img2 = noisy->toQImage();

    for(int i=0; i<points2.size(); i++)
    {
       desk2.push_back(genSecond->getDescriptor(points2[i].x, points2[i].y, 16, 4, 8));
    }

    qDebug() << "Drawing";
    QImage result = QImage(image->getWidth()*2 + 1, image->getHeight(), img1.format());
    result.fill(0);
    QPainter painter(&result);
    painter.setPen(QPen(QColor(Qt::red)));
    painter.drawImage(0,0,img1);
    painter.drawImage(image->getWidth()+1, 0, img2);

    for(uint i=0; i<points1.size(); i++) {
        painter.drawRect(points1[i].x - 1, points1[i].y - 1, 3, 3);
    }
    for(uint i=0; i<points2.size(); i++) {
        painter.drawRect(points2[i].x + image->getWidth(), points2[i].y, 3, 3);
    }

    shared_ptr<Descriptor> closest;
    for(int i=0; i<desk1.size(); i++)
    {
        closest = desk1[i]->findClosest(desk2);
        if(closest != nullptr)
        {
            painter.drawLine(QPoint(desk1[i]->x, desk1[i]->y), QPoint(closest->x + image->getWidth() + 1, closest->y));
        }
    }

    painter.end();

    QString savePath = curFolder.absolutePath() + "/descriptors/1.png";
    result.save(savePath);
}

MainWindow::~MainWindow()
{
    delete ui;
}
