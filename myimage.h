#ifndef MYIMAGE_H
#define MYIMAGE_H

#include <QImage>
#include <QColor>
#include <string>
#include "feature.h"

/*From online QT documentation:
 * There are functions to convert between QImage and QPixmap.
 * Typically, the QImage class is used to load an image file,
 * optionally manipulating the image data, before the QImage
 * object is converted into a QPixmap to be shown on screen.
 * Alternatively, if no manipulation is desired, the image file
 * can be loaded directly into a QPixmap.
 */

class MyImage
{
public:
    enum FilterType
    {
        ftNoFilter,
        ftRedWhiteOther, /*Used to detect red and white adjacent regions (stripe detection).*/
        ftBlackOther, /*Used to detect black circles (glasses detection).*/
        ftBlueOther, /*Used to detect medium-sized blue regions (for super basic pant detection).*/

        ftNum__FilterTypes,
    };

    enum ColorType
    {
        ctRed,
        ctWhite,
        ctBlack,
        ctBlue,
        ctOther,

        ctNum__ColorTypes,
    };

public:
    static const QColor QCOLOR_ARR[ctNum__ColorTypes];
    //static const QColor* RED;
    //static const QColor* WHITE;
    //static const QColor* OTHER_COLOR;
    static const int IMAGE_WIDTH = 64;
    static const int IMAGE_HEIGHT = 64;

public:
    /*Constructors/destructor*/
    MyImage();
    MyImage(const std::string &filename);
    MyImage(const MyImage &other);
    ~MyImage();

    /*Accessors*/
    //void Save(const std::string& filename);
    MyImage* GetFilteredImage(FilterType filter) const;
    QImage* GetQImage() const;
    MyImage::ColorType GetPixelColorType(int x, int y) const;
    int GetFeatureValue(FeatureSet::FeatureType featureType) const;

    /*Mutators*/
    //void Load(const std::string& filename);
    void ApplyFilter(FilterType filter);
    void SetFeatureValue(FeatureSet::FeatureType feature, int value);
private:
    //void InitFeatureSet();
public:

    /*Overloaded operators*/
    MyImage& operator=(const MyImage& right);
private:
    void ApplyRedWhiteOtherFilter();
    void ApplyBlackOtherFilter();
    void ApplyBlueOtherFilter();

private:
    QImage* mImage;
    FeatureSet* mFeatureSet;
};


#endif // MYIMAGE_H
