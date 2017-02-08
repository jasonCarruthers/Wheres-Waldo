#ifndef FEATURE_H
#define FEATURE_H


#include <vector>
#include <unordered_map>
//#include "myimage.h"

class MyImage;


class FeatureSet
{
public:
    enum FeatureType
    {
        ftHasRedAndWhiteStripes, /*Discrete*/
        ftHasBlackGlasses, /*Discrete*/
        ftHasBluePants, /*Discrete*/

        //ftNumRedPixels, /*Continuous*/
        //ftNumWhitePixels, /*Continuous*/
        //ftNumBluePixels, /*Continuous*/

        ftNum__FeatureTypes,
    };
public:
    /*Constructors, destructor*/
    FeatureSet();
    FeatureSet(const FeatureSet& other);
    ~FeatureSet();

    /*Accessors*/
    int GetFeature(FeatureType key) const;
    int GetSize() const;
    std::unordered_map<FeatureType, int>::iterator GetBegin();
    std::unordered_map<FeatureType, int>::iterator GetEnd();

    /*Mutators*/
    //void InitFeatureSet(const MyImage& image);
    void InitFeature_HasRedAndWhiteStripes(const MyImage& image);
    void InitFeature_HasBlackGlasses(const MyImage& image);
    void InitFeature_HasBluePants(const MyImage& image);
    void SetFeature(FeatureType feature, int value);
    void RemoveFeature(FeatureType featureToRemove);
public:

    /*Overloaded operators*/
    //int operator[](int index);
private:
    std::unordered_map<FeatureType, int>* mFeatureMap; /*This map holds all features, which are stored as
                                     ints. Bool features have values 0 or 1 (or non-zero);
                                     continuous features can then be represented along with
                                     boolean features in the same map.*/
};



#endif // FEATURE_H
