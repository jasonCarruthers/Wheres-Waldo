#ifndef REGION_H
#define REGION_H

#include <vector>
#include <unordered_map>
#include "myimage.h"
#include "vector2i.h"


class Region
{
public:
    /*Constructors/destructor*/
    Region();
    Region(MyImage::ColorType newColorType, std::vector<Vector2I*> *newPointVec);
    ~Region();

    /*Accessors*/
    MyImage::ColorType GetColorType() const;
    int GetSize() const;
    Vector2I* GetPoint(int pointIndex) const;
    Vector2I* GetCentroid() const;

    /*Mutators*/
    void SetColorType(MyImage::ColorType newColorType);
    void PushBack(const Vector2I& newPoint);

    /*Overloaded operators*/
    //Vector2I operator[](int index);

private:
    MyImage::ColorType mColorType;
    std::vector<Vector2I*> *mPointVec;
};


class RegionSet
{
public:
    /*Constructors/destructor*/
    RegionSet();
    ~RegionSet();

    /*Accessors*/
    std::vector<Region*>* GetRegionsByColor(int colorIndex);
    int GetSize() const;

    /*Mutators*/
    void PushBack(MyImage::ColorType regionColor, const Region& newRegion);

    /*Overloaded operators*/
    //std::vector<Region*> operator[](int colorIndex);

private:
    std::unordered_multimap<MyImage::ColorType, Region*>* mRegionMultiMap;
};





/*
 * Global function prototypes.
 */
extern void GetColoredRegion(const MyImage &image, int x, int y, MyImage::ColorType color,
                     Region* newRegion,
                     bool **visitedArr);
extern RegionSet *GetColoredRegions(
        const MyImage &image,
        const std::vector<MyImage::ColorType>& colorVec);


#endif // REGION_H
