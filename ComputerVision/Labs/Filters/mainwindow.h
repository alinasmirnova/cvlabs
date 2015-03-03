#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <memory>
#include "core.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    shared_ptr<Image> image;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void openFile();
    void leftShift();
    void rightShift();
    void topShift();
    void bottomShift();
    void blur();
    void sobelX();
    void sobelY();
    void pruitX();
    void pruitY();
    void sharrX();
    void sharrY();
    void sobel();
    void sobelSep();
    void sobelSepX();
    void sobelSepY();
    void gauss();
    void gaussSep();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
