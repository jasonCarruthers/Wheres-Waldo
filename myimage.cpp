#include <QImage>
#include <QColor>
#include <QPixmap>
#include <QPainter>
#include <QWidget>
#include <QDebug>
#include <string>
#include "MyImage.h"


/*
 * Implementation of class MyImage
 */
/*Initialize const static member variables.*/
const QColor MyImage::QCOLOR_ARR[MyImage::ctNum__ColorTypes] = {
                                                        QColor(255, 0, 0, 255), /*RED*/
                                                        QColor(255, 255, 255, 255), /*WHITE*/
                                                        QColor(0, 0, 0, 255), /*BLACK*/
                                                        QColor(0, 0, 255, 255), /*BLUE*/
                                                        QColor(0, 255, 0, 255) /*OTHER*/
                                                      };
//const QColor* MyImage::RED = new QColor(255, 0, 0, 255);
//const QColor* MyImage::WHITE = new QColor(255, 255, 255, 255);
//const QColor* MyImage::OTHER_COLOR = new QColor(0, 0, 255, 255);

/*Constructors/destructor*/
MyImage::MyImage()
{
    mImage = new QImage();
    mFeatureSet = new FeatureSet();
}

MyImage::MyImage(const std::string &filename)
{
    mImage = new QImage(QString(filename.c_str()));
    mFeatureSet = new FeatureSet();
}

MyImage::MyImage(const MyImage &other) : MyImage()
{
    if(mImage)
    {
        delete mImage;
    }

    mImage = new QImage(other.mImage->copy(other.mImage->rect()));

    mFeatureSet = new FeatureSet();
    for(std::unordered_map<FeatureSet::FeatureType, int>::iterator iter = other.mFeatureSet->GetBegin();
        iter != other.mFeatureSet->GetEnd(); iter++)
    {
        mFeatureSet->SetFeature(iter->first, iter->second);
    }
}

MyImage::~MyImage()
{
    if(mImage)
    {
        delete mImage;
    }

    if(mFeatureSet)
    {
        delete mFeatureSet;
    }
}

/*Accessors*/
//void Save(const std::string &filename);
MyImage* MyImage::GetFilteredImage(FilterType filter) const
{
    MyImage *retImage = new MyImage(*this);
    retImage->ApplyFilter(filter);
    return retImage;
}

QImage* MyImage::GetQImage() const
{
    return mImage;
}

MyImage::ColorType MyImage::GetPixelColorType(int x, int y) const
{
    /*Get the QColor at the given (x, y) position.*/
    QColor tempColor = mImage->pixelColor(x, y);

    /*Compare the pixel color to each stored color, and return true if there's a match.*/
    for(int colorIndex = 0; colorIndex < ctNum__ColorTypes; colorIndex++)
    {
        if( tempColor == QCOLOR_ARR[colorIndex] )
        {
            return (MyImage::ColorType)colorIndex;
        }
    }

    /*If no color match was found, then return the "other" color (which is a color
      that is not one of the most common colors used to detect Waldo).*/
    return ctOther;
}

int MyImage::GetFeatureValue(FeatureSet::FeatureType featureType) const
{
    return mFeatureSet->GetFeature(featureType);
}


/*Mutators*/
void MyImage::ApplyFilter(FilterType filter)
{
    switch((int)filter)
    {
    case (int)ftRedWhiteOther:  ApplyRedWhiteOtherFilter(); mFeatureSet->InitFeature_HasRedAndWhiteStripes(*this);
                                break;
    case (int)ftBlackOther:     ApplyBlackOtherFilter(); mFeatureSet->InitFeature_HasBlackGlasses(*this);
                                break;
    case (int)ftBlueOther:      ApplyBlueOtherFilter(); mFeatureSet->InitFeature_HasBluePants(*this);
                                break;
    default:                    break;
    }

    //InitFeatureSet();

}

void MyImage::ApplyRedWhiteOtherFilter()
{
    //qDebug() << "Entered MyImage::ApplyRedWhiteOtherFilter()";

    /*Declare variables.*/
    QColor tempColor;

    /*Loop through each pixel in this image.*/
    for(int y = 0; y < mImage->height(); y++)
    {
        for(int x = 0; x < mImage->width(); x++)
        {
            /*Get the current pixel's color.*/
            tempColor = mImage->pixelColor(x, y);

            /*If the current pixel's color is close to red, convert it to pure red.*/
            if(tempColor.red() >= 180 && tempColor.green() <= 125 && tempColor.blue() <= 125)
            {
                mImage->setPixelColor(x, y, MyImage::QCOLOR_ARR[MyImage::ctRed]);
            }

            /*If the current pixel's color is close to white, convert it to pure white.*/
            else if(tempColor.red() >= 200 && tempColor.green() >= 200 && tempColor.blue() >= 200)
            {
                mImage->setPixelColor(x, y, MyImage::QCOLOR_ARR[MyImage::ctWhite]);
            }

            /*Otherwise, convert the current pixel's color to OTHER_COLOR.*/
            else
            {
                mImage->setPixelColor(x, y, MyImage::QCOLOR_ARR[MyImage::ctOther]);
            }
        }
    }

    //qDebug() << "Exiting MyImage::ApplyRedWhiteOtherFilter()";
}

void MyImage::ApplyBlackOtherFilter()
{
    /*Declare variables.*/
    QColor tempColor;

    /*Loop through each pixel in this image.*/
    for(int y = 0; y < mImage->height(); y++)
    {
        for(int x = 0; x < mImage->width(); x++)
        {
            /*Get the current pixel's color.*/
            tempColor = mImage->pixelColor(x, y);

            /*If the current pixel's color is close to black, convert it to pure black.*/
            if(tempColor.red() <= 80 && tempColor.green() <= 80 && tempColor.blue() <= 80)
            {
                mImage->setPixelColor(x, y, MyImage::QCOLOR_ARR[MyImage::ctBlack]);
            }

            /*Otherwise, convert the current pixel's color to OTHER_COLOR.*/
            else
            {
                mImage->setPixelColor(x, y, MyImage::QCOLOR_ARR[MyImage::ctOther]);
            }
        }
    }
}

void MyImage::ApplyBlueOtherFilter()
{
    /*Declare variables.*/
    QColor tempColor;

    /*Loop through each pixel in this image.*/
    for(int y = 0; y < mImage->height(); y++)
    {
        for(int x = 0; x < mImage->width(); x++)
        {
            /*Get the current pixel's color.*/
            tempColor = mImage->pixelColor(x, y);

            /*If the current pixel's color is close to black, convert it to pure black.*/
            if(tempColor.red() <= 180 && tempColor.green() <= 180 && tempColor.blue() >= 180)
            {
                mImage->setPixelColor(x, y, MyImage::QCOLOR_ARR[MyImage::ctBlue]);
            }

            /*Otherwise, convert the current pixel's color to OTHER_COLOR.*/
            else
            {
                mImage->setPixelColor(x, y, MyImage::QCOLOR_ARR[MyImage::ctOther]);
            }
        }
    }
}

void MyImage::SetFeatureValue(FeatureSet::FeatureType feature, int value)
{
    mFeatureSet->SetFeature(feature, value);
}

//void MyImage::InitFeatureSet()
//{
//    mFeatureSet = new FeatureSet();
//    mFeatureSet->InitFeatureSet(*this);
//}

/*Overloaded operators*/
MyImage& MyImage::operator=(const MyImage& right)
{
    if(mImage)
    {
        delete mImage;
    }

    mImage = new QImage(right.mImage->copy(right.mImage->rect()));

    return *this;
}






