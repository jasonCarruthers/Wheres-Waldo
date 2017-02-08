#include <QDebug>
#include <vector>
#include <unordered_map>
#include "myimage.h"
#include "vector2i.h"
#include "region.h"



/*
 * Implementation of class Region.
 */
/*Constructors/destructor*/
Region::Region()
{
    mColorType = MyImage::ctOther;
    mPointVec = nullptr;
}

Region::Region(MyImage::ColorType newColorType, std::vector<Vector2I*> *newPointVec)
{
    mColorType = newColorType;
    mPointVec = newPointVec;
}

Region::~Region()
{
    if(mPointVec)
    {
        mPointVec->clear();
        delete mPointVec;
    }
}

/*Accessors*/
MyImage::ColorType Region::GetColorType() const
{
    return mColorType;
}

int Region::GetSize() const
{
    return (mPointVec) ? (int)mPointVec->size() : 0;
}

Vector2I* Region::GetPoint(int pointIndex) const
{
    if(!mPointVec || pointIndex < 0 || pointIndex >= mPointVec->size())
    {
        qDebug() << "Error: Region::GetPoint(): pointIndex out of bounds.";
        exit(0);
    }

    Vector2I* retVal = new Vector2I((*mPointVec)[pointIndex]->GetX(), (*mPointVec)[pointIndex]->GetY());
    return retVal;
}

Vector2I* Region::GetCentroid() const
{
    if(!mPointVec)
    {
        qDebug() << "Error: Region::GetCentroid(): No points in mPointVec.";
        return nullptr;
    }

    Vector2I* centroid = new Vector2I(0, 0);
    for(int pointIndex = 0; pointIndex < mPointVec->size(); pointIndex++)
    {
        *centroid += *((*mPointVec)[pointIndex]);
    }
    *centroid /= (int)mPointVec->size();

    return centroid;
}

/*Mutators*/
void Region::SetColorType(MyImage::ColorType newColorType)
{
    mColorType = newColorType;
}

void Region::PushBack(const Vector2I& newPoint)
{
    if(!mPointVec)
    {
        mPointVec = new std::vector<Vector2I*>;
    }

    Vector2I* newPointPtr = new Vector2I(newPoint.GetX(), newPoint.GetY());
    mPointVec->push_back(newPointPtr);
}

/*Overloaded operators*/
//Vector2I Region::operator[](int index)
//{
//    if(!mPointVec || index < 0 || index >= mPointVec->size())
//    {
//        qDebug() << "Error: Region::operator[]: index out of range.";
//        exit(0);
//    }

//    return *((*mPointVec)[index]);
//}




/*
 * Implementation of class RegionSet.
 */
/*Constructors/destructor*/
RegionSet::RegionSet()
{
    mRegionMultiMap = nullptr;
}

RegionSet::~RegionSet()
{
    if(mRegionMultiMap)
    {
        mRegionMultiMap->clear();
        delete mRegionMultiMap;
    }
}

/*Accessors*/
std::vector<Region*>* RegionSet::GetRegionsByColor(int colorIndex)
{
    //qDebug() << "colorIndex = " << colorIndex;

    if(!mRegionMultiMap)
    {
        //qDebug() << "Error: RegionSet::GetRegionsByColor(): mRegionMultiMap doesn't exist.\t";
        return nullptr;
        //exit(0);
    }

    std::vector<Region*>* retVal = new std::vector<Region*>;
    std::pair<std::unordered_multimap<MyImage::ColorType, Region*>::iterator,
              std::unordered_multimap<MyImage::ColorType, Region*>::iterator> iterRange =
              mRegionMultiMap->equal_range((MyImage::ColorType)colorIndex);
    for(std::unordered_multimap<MyImage::ColorType, Region*>::iterator iter = iterRange.first;
        iter != iterRange.second; iter++)
    {
        retVal->push_back(iter->second);
    }

    return (retVal->size() == 0) ? nullptr : retVal;
}

int RegionSet::GetSize() const
{
    return (mRegionMultiMap) ? (int)(mRegionMultiMap->size()) : 0;
}

/*Mutators*/
void RegionSet::PushBack(MyImage::ColorType regionColor, const Region& newRegion)
{
    if(!mRegionMultiMap)
    {
        mRegionMultiMap = new std::unordered_multimap<MyImage::ColorType, Region*>;
    }

    Region* newRegionPtr = new Region();
    newRegionPtr->SetColorType(newRegion.GetColorType());

    for(int pointIndex = 0; pointIndex < newRegion.GetSize(); pointIndex++)
    {
        Vector2I* tempPointPtr = newRegion.GetPoint(pointIndex);
        Vector2I tempPoint = Vector2I(tempPointPtr->GetX(), tempPointPtr->GetY());
        newRegionPtr->PushBack(Vector2I(tempPoint.GetX(), tempPoint.GetY()));
    }

    std::pair<MyImage::ColorType, Region*>* newPair = new std::pair<MyImage::ColorType, Region*>;
    newPair->first = regionColor;
    newPair->second = newRegionPtr;
    mRegionMultiMap->insert(*newPair);
}

/*Overloaded operators*/
//which is better? Return vector of region* or return RegionSet???
//std::vector<Region*> RegionSet::operator[](int colorIndex)
//{
//    if(!mRegionMultiMap || colorIndex < 0 || colorIndex >= mRegionMultiMap->size())
//    {
//        qDebug() << "Error: RegionSet::operator[]: colorIndex out of bounds.";
//        exit(0);
//    }

//    return (*mRegionVec)[colorIndex];
//}




/*
 * Global functions.
 */
void GetColoredRegion(const MyImage &image, int x, int y, MyImage::ColorType color,
                     Region* newRegion,
                     bool** visitedArr)
{
    visitedArr[x][y] = true;
    newRegion->PushBack(Vector2I(x, y));

    if(x - 1 >= 0 && visitedArr[x - 1][y] == false && image.GetPixelColorType(x - 1, y) == color)
    {
        GetColoredRegion(image, x - 1, y, color, newRegion, visitedArr);
    }
    if(x + 1 < MyImage::IMAGE_WIDTH && visitedArr[x + 1][y] == false && image.GetPixelColorType(x + 1, y) == color)
    {
        GetColoredRegion(image, x + 1, y, color, newRegion, visitedArr);
    }
    if(y - 1 >= 0 && visitedArr[x][y - 1] == false && image.GetPixelColorType(x, y - 1) == color)
    {
        GetColoredRegion(image, x, y - 1, color, newRegion, visitedArr);
    }
    if(y + 1 < MyImage::IMAGE_HEIGHT && visitedArr[x][y + 1] == false && image.GetPixelColorType(x, y + 1) == color)
    {
        GetColoredRegion(image, x, y + 1, color, newRegion, visitedArr);
    }
}

RegionSet* GetColoredRegions(
        const MyImage &image,
        const std::vector<MyImage::ColorType>& colorVec)
{
    /*Init variables.*/
    Region* newRegion;
    RegionSet* regionVec = new RegionSet();
    bool ***visitedArr = new bool**[colorVec.size()];

    /*Init visitedArr.*/
    for(int colorIndex = 0; colorIndex < colorVec.size(); colorIndex++)
    {
        visitedArr[colorIndex] = new bool*[MyImage::IMAGE_WIDTH];
        for(int visited_X = 0; visited_X < MyImage::IMAGE_WIDTH; visited_X++)
        {
            visitedArr[colorIndex][visited_X] = new bool[MyImage::IMAGE_HEIGHT];
            for(int visited_Y = 0; visited_Y < MyImage::IMAGE_HEIGHT; visited_Y++)
            {
                visitedArr[colorIndex][visited_X][visited_Y] = false;
            }
        }
    }

    /* For each pixel, if it is one of the colors in colorVec, then recursively tag all connected pixels
     * of that color and put all of those pixels into a vector, denoted as a "region." Iteratively
     * go through each pixel in the image and repeat this process until all pixels have been checked.
     * Note that pixels that have a color not matching any color in colorVec will not be grouped into
     * any region.
     */
    for(int pixelIndex_Y = 0; pixelIndex_Y < MyImage::IMAGE_HEIGHT; pixelIndex_Y++)
    {
        for(int pixelIndex_X = 0; pixelIndex_X < MyImage::IMAGE_WIDTH; pixelIndex_X++)
        {
            for(int colorIndex = 0; colorIndex < colorVec.size(); colorIndex++)
            {
                int pixelColorType = (int)(image.GetPixelColorType(pixelIndex_X, pixelIndex_Y));
                int colorVecColorType = (int)colorVec[colorIndex];
                if(pixelColorType == colorVecColorType)
                {
                    /*Get a new region, then insert it into the regionVec.*/
                    newRegion = new Region((MyImage::ColorType)colorIndex, new std::vector<Vector2I*>);
                    GetColoredRegion(image, pixelIndex_X, pixelIndex_Y, colorVec[colorIndex], newRegion, visitedArr[colorIndex]);
                    regionVec->PushBack((MyImage::ColorType)colorVec[colorIndex], *newRegion);
                }
            }
        }
    }

    return regionVec;
}

