#ifndef DRAWWINDOW_H
#define DRAWWINDOW_H

#include <QColor>
#include <QTWidgets>
#include "MyImage.h"


//class DrawWindow : public QWidget
//{
//    Q_OBJECT

//public:
//    DrawWindow(QWidget *parent = 0);

//    //void SetPenColor(const QColor &newColor);
//    //void SetPenWidth(int newWidth);

//    //QColor GetPenColor() const { return myPenColor; }
//    //int GetPenWidth() const { return myPenWidth; }

//public slots:
//   // void ClearImage();
//    //void Print();

//protected:
//    //void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
//    //void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
//    //void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
//    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
//    //void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

//private:
//    MyImage* mImage;
//};



class DrawWindow : public QGraphicsScene
{
public:
    DrawWindow();
    //void GetGraphicPos(int*, int*);

private:
};



#endif // DRAWWINDOW_H
