#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <memory>
#include "Image.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    shared_ptr<Image> originalImage;
    shared_ptr<Image> resultImage;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void openFile();
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
