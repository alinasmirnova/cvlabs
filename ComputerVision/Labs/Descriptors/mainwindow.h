#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QPainter>
#include "core.h"


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
    vector<Descriptor> findDescriptors(const Image& image, vector<Point> points);

    vector<Point> findScaledPoints(const Image& image, const Pyramid &pyramid);
    vector<Descriptor> findScaledDescriptors(vector<Point> points, const Pyramid &pyramid);

    QImage findAndDrawPairs(const Image& img1, const Image& img2,
                    vector<Point> points1, vector<Point> points2,
                    vector<Descriptor> desc1, vector<Descriptor> desc2);

private slots:
    void chooseFile();
    void findPoints();
};

#endif // MAINWINDOW_H