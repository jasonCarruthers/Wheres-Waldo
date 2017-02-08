#include <QDebug>
#include <vector>
#include <unordered_map>
#include "feature.h"
#include "vector2i.h"
#include "region.h"







/*Implementation of class FeatureSet.*/
/*Constructors/destructor*/
FeatureSet::FeatureSet()
{
    mFeatureMap = new std::unordered_map<FeatureSet::FeatureType, int>;
}

FeatureSet::FeatureSet(const FeatureSet& other)
{
    if(mFeatureMap)
    {
        delete mFeatureMap;
    }

    mFeatureMap = new std::unordered_map<FeatureSet::FeatureType, int>;
    for(std::unordered_map<FeatureSet::FeatureType, int>::iterator iter = other.mFeatureMap->begin();
        iter != other.mFeatureMap->end(); iter++)
    {
        std::pair<FeatureSet::FeatureType, int>* newPair = new std::pair<FeatureSet::FeatureType, int>;
        newPair->first = iter->first;
        newPair->second = iter->second;
        mFeatureMap->insert(*newPair);
    }
}

FeatureSet::~FeatureSet()
{
    if(mFeatureMap)
    {
        delete mFeatureMap;
    }
}

/*Accessors*/
int FeatureSet::GetFeature(FeatureSet::FeatureType key) const
{
    /*If the feature map doesn't exist, return from this function.*/
    if(!mFeatureMap)
    {
        //qDebug() << "Error: FeatureSet::GetFeature(): mFeatureMap doesn't exist.";
        exit(0);
    }

    return mFeatureMap->at(key);
}

int FeatureSet::GetSize() const
{
    return (int)mFeatureMap->size();
}

std::unordered_map<FeatureSet::FeatureType, int>::iterator FeatureSet::GetBegin()
{
    return mFeatureMap->begin();
}

std::unordered_map<FeatureSet::FeatureType, int>::iterator FeatureSet::GetEnd()
{
    return mFeatureMap->end();
}


/*Mutators*/
//void FeatureSet::InitFeatureSet(const MyImage& image)
//{
//    mFeatureMap = new std::unordered_map<FeatureSet::FeatureType, int>;

//    for(int i = 0; i < ftNum__FeatureTypes; i++)
//    {
//        switch(i)
//        {
//        case (int)ftHasRedAndWhiteStripes: InitFeature_HasRedAndWhiteStripes(image); break;
//        case (int)ftHasBlackGlasses:       InitFeature_HasBlackGlasses(image); break;
//        //case (int)ftHasBluePants:          InitFeature_HasBluePants(image); break;
//        default:                           break;
//        }
//    }
//}

void FeatureSet::InitFeature_HasRedAndWhiteStripes(const MyImage& image)
{
    //qDebug() << "Entering FeatureSet::InitFeature_HasRedAndWhiteStripes()";

    /*Init color vec.*/
    std::vector<MyImage::ColorType> colorVec;
    colorVec.push_back(MyImage::ctRed);
    colorVec.push_back(MyImage::ctWhite);

    /*Get red regions and white regions, stored in a multimap.*/
    RegionSet* regionSet = GetColoredRegions(image, colorVec);

    /*Get a vector of all red regions, and a vector of all white regions.*/
    std::vector<Region*>* redRegions = regionSet->GetRegionsByColor(MyImage::ctRed);
    std::vector<Region*>* whiteRegions = regionSet->GetRegionsByColor(MyImage::ctWhite);

    /*If there are no red regions or no white regions, return from this function.*/
    if(!redRegions || !whiteRegions)
    {
        /*If no stripes were detected, then set the hasRedAndWhiteStripes feature to zero.*/
        mFeatureMap->insert_or_assign(FeatureSet::ftHasRedAndWhiteStripes, (int)false);
        return;
    }

    /*Calculate the centroid of each red region and each white region.*/
    std::vector<Vector2I*>* redCentroids = new std::vector<Vector2I*>;
    std::vector<Vector2I*>* whiteCentroids = new std::vector<Vector2I*>;
    for(int redRegionIndex = 0; redRegionIndex < redRegions->size(); redRegionIndex++)
    {
        redCentroids->push_back((*redRegions)[redRegionIndex]->GetCentroid());
    }
    for(int whiteRegionIndex = 0; whiteRegionIndex < whiteRegions->size(); whiteRegionIndex++)
    {
        whiteCentroids->push_back((*whiteRegions)[whiteRegionIndex]->GetCentroid());
    }

    /*For each red region's centroid, check it's euclidian distance to each white region's centroid.
      If a set of two centroids are close enough, then that event is interpreted as having
      red and white stripes.*/
    const float MAX_DIST = 2.0f; /*In units of pixels.*/
    const float MAX_DIST_SQUARED = MAX_DIST * MAX_DIST;
    for(int redCentroidIndex = 0; redCentroidIndex < redCentroids->size(); redCentroidIndex++)
    {
        for(int whiteCentroidIndex = 0; whiteCentroidIndex < whiteCentroids->size(); whiteCentroidIndex++)
        {
            float distSquared = (*redCentroids)[redCentroidIndex]->GetDistanceSquaredFromPoint(*((*whiteCentroids)[whiteCentroidIndex]));
            if(distSquared <= MAX_DIST_SQUARED)
            {
                mFeatureMap->insert_or_assign(FeatureSet::ftHasRedAndWhiteStripes, (int)true);
                //qDebug() << "distSquared = " << distSquared;
                //qDebug() << "STRIPES DETECTED!";
                return;
            }
        }
    }

    /*If no stripes were detected, then set the hasRedAndWhiteStripes feature to zero.*/
    mFeatureMap->insert_or_assign(FeatureSet::ftHasRedAndWhiteStripes, (int)false);

    //qDebug() << "Exiting FeatureSet::InitFeature_HasRedAndWhiteStripes()";
}

void FeatureSet::InitFeature_HasBlackGlasses(const MyImage& image)
{
    /*Init color vec.*/
    std::vector<MyImage::ColorType> colorVec;
    colorVec.push_back(MyImage::ctBlack);

    /*Get black regions, stored in a multimap.*/
    RegionSet* regionSet = GetColoredRegions(image, colorVec);

    /*Get a vector of all black regions.*/
    std::vector<Region*>* blackRegions = regionSet->GetRegionsByColor(MyImage::ctBlack);

    /*If there are no black regions, return from this function.*/
    if(!blackRegions)
    {
        /*If no glasses were detected, then set the hasBlackGlasses feature to zero.*/
        mFeatureMap->insert_or_assign(FeatureSet::ftHasBlackGlasses, (int)false);
        return;
    }

    /*Calculate the centroid of each black region.*/
    std::vector<Vector2I*>* blackCentroids = new std::vector<Vector2I*>;
    for(int blackRegionIndex = 0; blackRegionIndex < blackRegions->size(); blackRegionIndex++)
    {
        blackCentroids->push_back((*blackRegions)[blackRegionIndex]->GetCentroid());
    }

    /*For each black region's centroid, get the average distance of all points in that region.
      Then, for each point in a region, check if it is within the average distance +/- the
      DELTA_RADIUS. If a high enough percentage of points in a region are within the radii,
      set the hasBlackGlasses flag to true.*/
    const float DELTA_RADIUS = 2.0f; /*In units of pixels.*/
    const int MIN_POINTS = 8;
    const float MIN_RATIO = 0.70f;
    for(int blackRegionIndex = 0; blackRegionIndex < blackRegions->size(); blackRegionIndex++)
    {
        float averageDist = 0.0f;
        int numPoints = (int)(*blackRegions)[blackRegionIndex]->GetSize();
        if(numPoints < MIN_POINTS)
        {
            continue;
        }

        for(int pointIndex = 0; pointIndex < numPoints; pointIndex++)
        {
            float dist = sqrt((double)(*blackRegions)[blackRegionIndex]->GetPoint(pointIndex)->GetDistanceSquaredFromPoint(*((*blackCentroids)[blackRegionIndex])));
            averageDist += dist;
        }
        averageDist /= blackRegions->size();

        int numOnCircle = 0;
        for(int pointIndex = 0; pointIndex < numPoints; pointIndex++)
        {
            float dist = sqrt((double)(*blackRegions)[blackRegionIndex]->GetPoint(pointIndex)->GetDistanceSquaredFromPoint(*((*blackCentroids)[blackRegionIndex])));
            if(dist >= averageDist - DELTA_RADIUS && dist <= averageDist + DELTA_RADIUS)
            {
                numOnCircle++;
            }
        }

        //qDebug() << "percent on circle = " << numOnCircle / (float)numPoints;
        if(numOnCircle / (float)numPoints >= MIN_RATIO)
        {
            mFeatureMap->insert_or_assign(FeatureSet::ftHasBlackGlasses, (int)true);
            //qDebug() << "GLASSES DETECTED!";
            return;
        }

    }

    /*If no glasses were detected, then set the hasBlackGlasses feature to zero.*/
    mFeatureMap->insert_or_assign(FeatureSet::ftHasBlackGlasses, (int)false);
}

void FeatureSet::InitFeature_HasBluePants(const MyImage& image)
{
    /*Init color vec.*/
    std::vector<MyImage::ColorType> colorVec;
    colorVec.push_back(MyImage::ctBlue);

    /*Get blue regions, stored in a multimap.*/
    RegionSet* regionSet = GetColoredRegions(image, colorVec);

    /*Get a vector of all blue regions.*/
    std::vector<Region*>* blueRegions = regionSet->GetRegionsByColor(MyImage::ctBlue);

    /*If there are no blue regions, return from this function.*/
    if(!blueRegions)
    {
        //qDebug() << "No blue regions detected.";
        /*If no blue regions are found, set the feature hasBluePants to zero (boolean false).*/
        mFeatureMap->insert_or_assign(FeatureSet::ftHasBluePants, (int)false);
        return;
    }

    /*Waldo's blue pants use at at least MIN_POINTS many pixels, so set the feature value
      hasBluePants to one (boolean true) if a blue region less than or equal to this
      threshold value is found. Large blue regions are rarely used in Where's Waldo puzzles,
      and they most commonly appear in pants and the blue background on the postcard displaying
      Waldo's face.*/
    const int MIN_POINTS = 70;
    for(int blueRegionIndex = 0; blueRegionIndex < blueRegions->size(); blueRegionIndex++)
    {
        if((*blueRegions)[blueRegionIndex]->GetSize() >= MIN_POINTS)
        {
            //qDebug() << "\t\t\tPants detected!";
            mFeatureMap->insert_or_assign(FeatureSet::ftHasBluePants, (int)true);
            return;
        }
    }

    /*If no blue regions are found, set the feature hasBluePants to zero (boolean false).*/
    mFeatureMap->insert_or_assign(FeatureSet::ftHasBluePants, (int)false);
}

 void FeatureSet::RemoveFeature(FeatureSet::FeatureType featureToRemove)
 {
     mFeatureMap->erase(mFeatureMap->find(featureToRemove));
 }

 void FeatureSet::SetFeature(FeatureType feature, int value)
 {
     mFeatureMap->insert_or_assign(feature, value);
 }

/*Overloaded operators*/
//int FeatureSet::operator[](int index)
//{
//    /*If the index is out of bounds, return from this function.*/
//    if(index < 0 || index >= mFeatureVec->size())
//    {
//        qDebug() << "Error: FeatureSet::operator[]: index out of bounds.";
//        exit(0);
//    }

//    return (*mFeatureVec)[index];
//}
