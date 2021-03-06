#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
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
    shared_ptr<Pyramid> pyramid;

    QString folder;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
private slots:
    void openFile();
    void buildPyramid();
    void findL();
};

#endif // MAINWINDOW_H
