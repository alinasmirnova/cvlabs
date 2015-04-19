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

    vector<Point> findPoints(const Image& img1);
    vector<Descriptor> findDescriptors(const Image& img1, vector<Point> points);
    QImage findAndDrawPairs(const Image& img1, const Image& img2,
                    vector<Point> points1, vector<Point> points2,
                    vector<Descriptor> desc1, vector<Descriptor> desc2);

private slots:
    void chooseFile();
    void findPoints();
};

#endif // MAINWINDOW_H
