#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include "DrawWindow.h"
#include "MyImage.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void InitWaldoImages();
    void InitNonWaldoImages();
    void DrawImages() const;

private:
    Ui::MainWindow *ui;
    DrawWindow* mDrawWindow;
    std::vector<std::vector<MyImage*>*>* mWaldoImageVec; /*Outer vec is filters, inner vec is images.*/
    std::vector<std::vector<MyImage*>*>* mNonWaldoImageVec; /*Outer vec is filters, inner vec is images.*/
};

#endif // MAINWINDOW_H
