#ifndef DECISIONTREE_H
#define DECISIONTREE_H

#include <vector>
#include "myimage.h"
#include "feature.h"


/*NOTE: Leaf nodes point to nullptr.*/
class DecisionTree
{
public:
    enum ClassificationType
    {
        ctNoClassification,
        ctWaldo,
        ctNonWaldo,

        ctNum__ClassificationTypes,
    };

public:
    class Node
    {
    public:
        Node()
        {
            mLeft = nullptr;
            mRight = nullptr;
            mID = -1;
        }

    public:
        Node* mLeft;
        Node* mRight;
        FeatureSet::FeatureType mDecisionKey;
        ClassificationType mClassification;
        int mID;
    public:
        static int nextID;
    };

public:
    /*Constructors/destructor*/
    DecisionTree();
    ~DecisionTree();

    /*Accessors*/
    ClassificationType TestImage(const MyImage& testImage) const;
    float GetPercentErrorOfTestingSet(const std::vector<MyImage*>& waldoImageVec,
                                      const std::vector<MyImage*>& nonWaldoImageVec);
    Node* GetRootNode();
    void DisplayTree(Node *movePtr) const;
    int GetLevelOfNode(int searchID, Node *movePtr, int currentLevel) const; /*Returns -1 if not found.*/

    /*Mutators*/
    void ConstructTree(std::vector<MyImage*>* waldoImageVec,
                       std::vector<MyImage*>* nonWaldoImageVec,
                       const int numTotalImages_Waldo,
                       const int numTotalImages_NonWaldo,
                       std::vector<int*> *featureVec,
                       Node* treeNode);

    /*Overloaded operators*/

private:
    Node* mRoot;
};



#endif // DECISIONTREE_H
