#include <QDebug>
#include <vector>
#include <math.h>
#include "myimage.h"
#include "feature.h"
#include "decisiontree.h"



/*
 * Implementation of class DecisionTree.
 */
/*Initialization of static member variables.*/
int DecisionTree::Node::nextID = 0;

/*Constructors/destructor*/
DecisionTree::DecisionTree()
{
    mRoot = new Node();
}

DecisionTree::~DecisionTree()
{
    if(mRoot)
    {
        delete mRoot; /*FIX: Only deletes root node; memory leak.*/
    }
}

/*Accessors*/
DecisionTree::ClassificationType DecisionTree::TestImage(const MyImage& testImage) const
{
    Node* movePtr = mRoot;

    while(movePtr->mLeft != nullptr && movePtr->mRight != nullptr)
    {
        int decisionVal = testImage.GetFeatureValue(movePtr->mDecisionKey);
        //qDebug() << "Feature " << movePtr->mDecisionKey << " = " << decisionVal;

        if(movePtr->mClassification == DecisionTree::ctWaldo ||
           movePtr->mClassification == DecisionTree::ctNonWaldo)
        {
            qDebug() << "Error: DecisionTree::TestImage(): Should not enter this if statement.";
            break;
        }

        /* Always move left in the tree if the feature value of the testImage is zero (boolean false)
         * for the current node's decision key.*/
        if(decisionVal == 0)
        {
            movePtr = movePtr->mLeft;
        }

        /* Always move right othewise.*/
        else
        {
            movePtr = movePtr->mRight;
        }
    }

    return movePtr->mClassification;
}

float DecisionTree::GetPercentErrorOfTestingSet(const std::vector<MyImage*>& waldoImageVec,
                                                const std::vector<MyImage*>& nonWaldoImageVec)
{
    /*Init variables.*/
    int numFailedDetections_Waldo = 0;
    int numFailedDetections_NonWaldo = 0;
    float waldoImagesPercentError = 0.0f;
    float nonWaldoImagesPercentError = 0.0f;
    float totalPercentError = 0.0f;

    /*Calculate percent error for the Waldo testing images.*/
    for(int waldoImageIndex = 0; waldoImageIndex < waldoImageVec.size(); waldoImageIndex++)
    {
        numFailedDetections_Waldo += (TestImage(*(waldoImageVec[waldoImageIndex])) != ctWaldo) ? 1.0f : 0.0f;
    }
    waldoImagesPercentError = (float)numFailedDetections_Waldo / (float)waldoImageVec.size();
    qDebug() << "numFailedDetection_Waldo = " << numFailedDetections_Waldo;
    qDebug() << "numSucceededDetection_Waldo = " << (int)waldoImageVec.size() - numFailedDetections_Waldo;
    qDebug() << "Waldo images %error = " << waldoImagesPercentError * 100.0f;

    /*Calculate percent error for the non-Waldo testing images.*/
    for(int nonWaldoImageIndex = 0; nonWaldoImageIndex < nonWaldoImageVec.size(); nonWaldoImageIndex++)
    {
        numFailedDetections_NonWaldo += (TestImage(*(nonWaldoImageVec[nonWaldoImageIndex])) != ctNonWaldo) ? 1.0f : 0.0f;
    }
    nonWaldoImagesPercentError = (float)numFailedDetections_NonWaldo / (float)nonWaldoImageVec.size();
    qDebug() << "numFailedDetection_NonWaldo = " << numFailedDetections_NonWaldo;
    qDebug() << "numSucceededDetection_NonWaldo = " << (int)nonWaldoImageVec.size() - numFailedDetections_NonWaldo;
    qDebug() << "Non-Waldo images %error = " << nonWaldoImagesPercentError * 100.0f;

    /*Return total percent error.*/
    totalPercentError = (float)(numFailedDetections_Waldo + numFailedDetections_NonWaldo) /
                        (float)(waldoImageVec.size() + nonWaldoImageVec.size());
    qDebug() << "Total %error = " << totalPercentError * 100.0f;
    return totalPercentError;
}

void DecisionTree::DisplayTree(Node* movePtr) const
{
    if(movePtr->mLeft == nullptr && movePtr->mRight == nullptr)
    {
        qDebug() << "Leaf node: classification = " << movePtr->mClassification << " ID = " << movePtr->mID;
        return;
    }
    else
    {
        DisplayTree(movePtr->mLeft);
        qDebug() << "Inner node: decision key = " << movePtr->mDecisionKey << " ID = " << movePtr->mID;
        DisplayTree(movePtr->mRight);
    }
}

/*Returns -1 if not found.*/
int DecisionTree::GetLevelOfNode(int searchID, Node* movePtr, int currentLevel) const
{
    if(movePtr == nullptr)
    {
        return -1;
    }
    if(movePtr->mID == searchID)
    {
        return currentLevel;
    }

    int retVal_Left = GetLevelOfNode(searchID, movePtr->mLeft, currentLevel + 1);
    int retVal_Right = GetLevelOfNode(searchID, movePtr->mRight, currentLevel + 1);
    if(retVal_Left >= 0)
    {
        return retVal_Left;
    }
    else if(retVal_Right >= 0)
    {
        return retVal_Right;
    }

    return -1;
}

DecisionTree::Node* DecisionTree::GetRootNode()
{
    return mRoot;
}

/*Mutators*/
void DecisionTree::ConstructTree(std::vector<MyImage*>* waldoImageVec,
                                 std::vector<MyImage*>* nonWaldoImageVec,
                                 const int numImagesAtStart_Waldo,
                                 const int numImagesAtStart_NonWaldo,
                                 std::vector<int*>* featureVec,
                                 Node* treeNode)
{
    qDebug() << "Entered ConstructTree()";

    /*Init variables.*/
    float entropyBeforeSplit = 0.0f;
    float minEntropyAfterSplit = 2.0f; /*Entropy should be between 0 and 1, so a starting value
                                         greater than 1 ensures that a minimum entropy after split
                                         will be found.*/
    int featureToSplitOn;
    float informationGain = 0.0;
    int numTotalImages = (int)waldoImageVec->size() + (int)nonWaldoImageVec->size();

    /*If the current node has all instances of one class (all Waldo or all non-Waldo), then
      no need to split anymore at this node.*/
    if((int)waldoImageVec->size() == 0 || (int)nonWaldoImageVec->size() == 0)
    {
        qDebug() << "Purity = 100%. Creating leaf node.";
        treeNode->mID = DecisionTree::Node::nextID;
        DecisionTree::Node::nextID++;
        treeNode->mClassification = ((int)waldoImageVec->size() == 0) ? ctNonWaldo : ctWaldo;
        qDebug() << "Setting classification to " << treeNode->mClassification;
        treeNode->mLeft = nullptr;
        treeNode->mRight = nullptr;
        return;
    }

    /*If there are no more features that can be used to split at the current node, return.*/
    if(featureVec->size() == 0)
    {
        qDebug() << "No more features to split on. Creating leaf node.";
        treeNode->mID = DecisionTree::Node::nextID;
        DecisionTree::Node::nextID++;
        float percentageWaldo = (float)waldoImageVec->size() / (float)numImagesAtStart_Waldo;
        float percentageNonWaldo = (float)nonWaldoImageVec->size() / (float)numImagesAtStart_NonWaldo;
        treeNode->mClassification = (percentageNonWaldo > percentageWaldo) ?
                                     ctNonWaldo : ctWaldo;
        qDebug() << "%Waldo = " << percentageWaldo << "%Non-Waldo = " << percentageNonWaldo;
        qDebug() << "Setting classification to " << treeNode->mClassification;
        treeNode->mLeft = nullptr;
        treeNode->mRight = nullptr;
        return;
    }

    /*Calculate entropy at the current node (pre-split).*/
    qDebug() << "Calculating current entropy.";
    float probWaldo = (float)waldoImageVec->size() / (float)numTotalImages;
    float probNonWaldo = 1.0f - probWaldo;
    entropyBeforeSplit = -1.0f * probWaldo * (float)log2((double)probWaldo) -
                                 probNonWaldo * (float)log2((double)probNonWaldo);


    qDebug() << "Calculating min new entropy.";
    for(int featureIndex = 0; featureIndex < featureVec->size(); featureIndex++)
    {
        /*For each feature that could be used to split at the current node, calculate the entropy
          at the current node after the split on that feature. Take the minimum entropy (post-split).*/
        float entropy_Left = 0.0f;
        float entropy_Right = 0.0f;
        int numWaldoImagesWithFeature = 0;
        int numNonWaldoImagesWithFeature = 0;

        /*Calculate the number of Waldo images that have the current feature.*/
        for(int waldoImageIndex = 0; waldoImageIndex < waldoImageVec->size(); waldoImageIndex++)
        {
            if((*waldoImageVec)[waldoImageIndex]->GetFeatureValue((FeatureSet::FeatureType)*((*featureVec)[featureIndex])) == 1)
            {
                numWaldoImagesWithFeature++;
            }
        }

        /*Calculate the number of non-Waldo images that have the current feature.*/
        for(int nonWaldoImageIndex = 0; nonWaldoImageIndex < nonWaldoImageVec->size(); nonWaldoImageIndex++)
        {
            if((*nonWaldoImageVec)[nonWaldoImageIndex]->GetFeatureValue((FeatureSet::FeatureType)*((*featureVec)[featureIndex])) == 1)
            {
                numNonWaldoImagesWithFeature++;
            }
        }

        /*Calculate the entropy of the left node post-split.*/
        int numTotalImages_Left = ((int)waldoImageVec->size() - numWaldoImagesWithFeature) +
                                    ((int)nonWaldoImageVec->size() - numNonWaldoImagesWithFeature);
        float probWaldo_Left = (float)((int)waldoImageVec->size() - numWaldoImagesWithFeature) / (float)numTotalImages_Left;
        float probNonWaldo_Left = 1.0f - probWaldo_Left;
        entropy_Left = -1.0f * probWaldo_Left * (float)log2((float)probWaldo_Left) -
                               probNonWaldo_Left * (float)log2((float)probNonWaldo_Left);

        /*Calculate the entropy of the right node post-split.*/
        int numTotalImages_Right = numWaldoImagesWithFeature + numNonWaldoImagesWithFeature;
        float probWaldo_Right = (float)numWaldoImagesWithFeature / (float)numTotalImages_Right;
        float probNonWaldo_Right = 1.0f - probWaldo_Right;
        entropy_Right = -1.0f * probWaldo_Right * (float)log2((float)probWaldo_Right) -
                                probNonWaldo_Right * (float)log2((float)probNonWaldo_Right);

        /*Calculate entropy at current node post-split.*/
        float entropyAfterSplit = ((float)numTotalImages_Left / (float)numTotalImages) * entropy_Left +
                                  ((float)numTotalImages_Right / (float)numTotalImages) * entropy_Right;

        /*If the entropyAfterSplit is less than the current min entropy post-split, update it
          and also update the feature to split on.*/
        if(entropyAfterSplit < minEntropyAfterSplit)
        {
            minEntropyAfterSplit = entropyAfterSplit;
            featureToSplitOn = *((*featureVec)[featureIndex]);
        }
    }

    /*Calculate the information gain. If the information gain is zero, then no more
      splitting will occur at the current node.*/
    informationGain = entropyBeforeSplit - minEntropyAfterSplit;
    const float MIN_THRESHOLD = 0.000005f;
    if(informationGain <= MIN_THRESHOLD)
    {
        qDebug() << "Info gain too low. Creating leaf node.";
        qDebug() << "entropyBeforeSplit = " << entropyBeforeSplit;
        qDebug() << "minEntropyAfterSplit = " << minEntropyAfterSplit;
        treeNode->mID = DecisionTree::Node::nextID;
        DecisionTree::Node::nextID++;
        float percentageWaldo = (float)waldoImageVec->size() / (float)numImagesAtStart_Waldo;
        float percentageNonWaldo = (float)nonWaldoImageVec->size() / (float)numImagesAtStart_NonWaldo;
        treeNode->mClassification = (percentageNonWaldo > percentageWaldo) ?
                                     ctNonWaldo : ctWaldo;
        qDebug() << "Setting classification to " << treeNode->mClassification;
        treeNode->mLeft = nullptr;
        treeNode->mRight = nullptr;
        return;
    }

    /*Split the current node on the featureToSplitOn. Update the arguments for each
      recursive call (left and right subnodes).*/
    qDebug() << "Splitting arguments for split on node.";
    std::vector<MyImage*>* waldoImageVec_Left = new std::vector<MyImage*>;
    std::vector<MyImage*>* waldoImageVec_Right = new std::vector<MyImage*>;
    std::vector<MyImage*>* nonWaldoImageVec_Left = new std::vector<MyImage*>;
    std::vector<MyImage*>* nonWaldoImageVec_Right = new std::vector<MyImage*>;
    for(int waldoImageIndex = 0; waldoImageIndex < waldoImageVec->size(); waldoImageIndex++)
    {
        if((*waldoImageVec)[waldoImageIndex]->GetFeatureValue((FeatureSet::FeatureType)featureToSplitOn) == 0)
        {
            waldoImageVec_Left->push_back((*waldoImageVec)[waldoImageIndex]);
        }
        else /*if((*waldoImageVec)[waldoImageIndex]->GetFeatureValue(featureToSplitOn) == 1)*/
        {
            waldoImageVec_Right->push_back((*waldoImageVec)[waldoImageIndex]);
        }
    }
    for(int nonWaldoImageIndex = 0; nonWaldoImageIndex < nonWaldoImageVec->size(); nonWaldoImageIndex++)
    {
        if((*nonWaldoImageVec)[nonWaldoImageIndex]->GetFeatureValue((FeatureSet::FeatureType)featureToSplitOn) == 0)
        {
            nonWaldoImageVec_Left->push_back((*nonWaldoImageVec)[nonWaldoImageIndex]);
        }
        else /*if((*nonWaldoImageVec)[nonWaldoImageIndex]->GetFeatureValue(featureToSplitOn) == 1)*/
        {
            nonWaldoImageVec_Right->push_back((*nonWaldoImageVec)[nonWaldoImageIndex]);
        }
    }

    /*Get the position of the feature to remove from the featureVec by searching through the vec
      for the featureToSplitOn. Then remove that feature from the featureVec.*/
    qDebug() << "Removing feature from featureVec.";
    int removeIndex = 0;
    for(; removeIndex < featureVec->size(); removeIndex++)
    {
        if(*((*featureVec)[removeIndex]) == featureToSplitOn)
        {
            break;
        }
    }
    featureVec->erase(featureVec->begin() + removeIndex);

    /*Create deep copies of featureVec to pass to each child node.*/
    qDebug() << "Creating deep copies of featureVec.";
    std::vector<int*>* featureVec_Left = new std::vector<int*>;
    std::vector<int*>* featureVec_Right = new std::vector<int*>;
    qDebug() << "Allocated memory for new feature vecs.";
    int maxSize = (int)featureVec->size();
    qDebug() << "maxSize = " << maxSize;
    for(int featureIndex = 0; featureIndex < maxSize; featureIndex++)
    {
        qDebug() << "featureVec->size() = " << featureVec->size() << " and featureVec = " << featureVec;
        for(int i = 0; i < (int)FeatureSet::ftNum__FeatureTypes; i++)
        {
            qDebug() << "About to calculate tempFeature. featureIndex = " << featureIndex;
            qDebug() << "featureVec = " << featureVec;
            int tempFeature = *((*featureVec)[featureIndex]);
            qDebug() << "tempFeature = " << tempFeature;
            if(i == tempFeature)
            {
                int* newFeature_Left = new int;
                int* newFeature_Right = new int;
                *newFeature_Left = i;
                *newFeature_Right = i;
                featureVec_Left->push_back(newFeature_Left);
                featureVec_Right->push_back(newFeature_Right);
                newFeature_Left = nullptr;
                newFeature_Right = nullptr;
                qDebug() << "Pushed back feature " << i << " and featureVec->size() = " << featureVec->size();
                break;
            }
        }
        qDebug() << "Pushed back feature in deep copy vecs. And featureVec->size() = " << (int)featureVec->size();
    }

    /*Make a recursive call to the new left and right nodes of the current node.*/
    qDebug() << "Preparing for recursive call to children nodes.";
    treeNode->mID = DecisionTree::Node::nextID;
    DecisionTree::Node::nextID++;
    treeNode->mDecisionKey = (FeatureSet::FeatureType)featureToSplitOn;
    treeNode->mLeft = new Node();
    treeNode->mRight = new Node();
    qDebug() << "Creating left node.";
    ConstructTree(waldoImageVec_Left, nonWaldoImageVec_Left, numImagesAtStart_Waldo, numImagesAtStart_NonWaldo, featureVec_Left, treeNode->mLeft);
    qDebug() << "Creating right node.";
    ConstructTree(waldoImageVec_Right, nonWaldoImageVec_Right, numImagesAtStart_Waldo, numImagesAtStart_NonWaldo, featureVec_Right, treeNode->mRight);
}
