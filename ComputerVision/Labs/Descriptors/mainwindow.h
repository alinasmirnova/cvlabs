#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QPainter>
#include "core.h"
#include "models.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    shared_ptr<Image> img1;
    shared_ptr<Image> img2;
    QDir curFolder;

    vector<Point> findPoints(const Image& image);
    vector<shared_ptr<Descriptor>> findDescriptors(const Image& image, vector<Point> points);

    vector<Point> findScaledPoints(const Pyramid &pyramid);
    vector<shared_ptr<Descriptor>> findScaledDescriptors(vector<Point> points, const Pyramid &pyramid);

    QImage findAndDrawPairs(const Image& img1, const Image& img2,
                    vector<Point> points1, vector<Point> points2,
                    vector<shared_ptr<Descriptor> > desc1, vector<shared_ptr<Descriptor> > desc2);

    QImage createPanorama(const Image &img1, const Image& img2, float * model);

private slots:
    void chooseFile();
    void findPoints();
};

#endif // MAINWINDOW_H
