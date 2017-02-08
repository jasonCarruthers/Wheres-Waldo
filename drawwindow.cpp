#include <QColor>
#include <QPainter>
#include <QWidget>
#include <QImage>
#include <QPaintEvent>
#include <QtPrintSupport/QPrinter>
#include "DrawWindow.h"
#include "MyImage.h"


/*Constructors*/
//DrawWindow::DrawWindow(QWidget *parent) :
//    QWidget(parent)
//{
//    mImage = new MyImage("Nature 03.jpg");
//}



/*
void DrawWindow::Print()
{
#if !defined(QT_NO_PRINTER) && !defined(QT_NO_PRINTDIALOG)
    QPrinter printer(QPrinter::HighResolution);

    QPrintDialog printDialog(&printer, this);
    if (printDialog.exec() == QDialog::Accepted)
    {
        QPainter painter(&printer);
        QRect rect = painter.viewport();
        QSize size = image.size();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(image.rect());
        painter.drawImage(0, 0, image);
    }
#endif // QT_NO_PRINTER
}
*/


//void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

//void DrawWindow::paintEvent(QPaintEvent *event)
//{
//    //QPainter painter(this);
//    QRect dirtyRect = event->rect();
//    //painter.drawImage(dirtyRect, image, dirtyRect);

//    //mImage->Draw(0, 0, event);


//    QPrinter printer(QPrinter::HighResolution);
//    QPainter painter(&printer);
//    QRect rect = painter.viewport();
//    QSize size = mImage->GetQImage()->size();
//    size.scale(rect.size(), Qt::KeepAspectRatio);
//    painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
//    painter.setWindow(mImage->GetQImage()->rect());
//    painter.drawImage(0, 0, *(mImage->GetQImage()));
//}
//void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;





DrawWindow::DrawWindow() : QGraphicsScene()
{
}


