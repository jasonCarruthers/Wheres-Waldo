#include <QDir>
#include <QStringList>
#include <QDebug>
#include "mainwindow.h"
#include "decisiontree.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mDrawWindow = new DrawWindow();
    ui->CenterView->setScene(mDrawWindow);
    ui->centralWidget->setGeometry(0, 0, 2000, 1000);
    ui->CenterView->setGeometry(0, 0, 1900, 1000);

    InitWaldoImages();
    InitNonWaldoImages();


    DecisionTree* decisionTree = new DecisionTree();
    std::vector<int*>* featureVec = new std::vector<int*>;
    for(int featureIndex = 0; featureIndex < FeatureSet::ftNum__FeatureTypes; featureIndex++)
    {
        int* newFeature = new int;
        *newFeature = featureIndex;
        featureVec->push_back(newFeature);
        newFeature = nullptr;
    }

    const float constructionRatio = 0.7f;
    std::vector<MyImage*>* waldoConstructionVec = new std::vector<MyImage*>;
    std::vector<MyImage*>* nonWaldoConstructionVec = new std::vector<MyImage*>;
    std::vector<MyImage*>* waldoTestingVec = new std::vector<MyImage*>;
    std::vector<MyImage*>* nonWaldoTestingVec = new std::vector<MyImage*>;
    int numWaldoImages = (int)(*mWaldoImageVec)[MyImage::ftNoFilter]->size();
    int numNonWaldoImages = (int)(*mNonWaldoImageVec)[MyImage::ftNoFilter]->size();
    for(int imageIndex = 0; imageIndex < numWaldoImages; imageIndex++)
    {
        if((int)(constructionRatio * (float)numWaldoImages) <= imageIndex)
        {
            waldoConstructionVec->push_back((*((*mWaldoImageVec)[MyImage::ftNoFilter]))[imageIndex]);
        }
        else
        {
            waldoTestingVec->push_back((*((*mWaldoImageVec)[MyImage::ftNoFilter]))[imageIndex]);
        }
    }
    for(int imageIndex = 0; imageIndex < numNonWaldoImages; imageIndex++)
    {
        if((int)(constructionRatio * (float)numNonWaldoImages) <= imageIndex)
        {
            nonWaldoConstructionVec->push_back((*((*mNonWaldoImageVec)[MyImage::ftNoFilter]))[imageIndex]);
        }
        else
        {
            nonWaldoTestingVec->push_back((*((*mNonWaldoImageVec)[MyImage::ftNoFilter]))[imageIndex]);
        }
    }

    decisionTree->ConstructTree(waldoConstructionVec,
                                nonWaldoConstructionVec,
                                (int)waldoConstructionVec->size(),
                                (int)nonWaldoConstructionVec->size(),
                                featureVec,
                                decisionTree->GetRootNode());
    qDebug() << "Finished constructing tree.";

    DecisionTree::ClassificationType classification = decisionTree->TestImage(*((*(*mWaldoImageVec)[MyImage::ftNoFilter])[0]));
    qDebug() << "Image classified as " << classification;

    decisionTree->DisplayTree(decisionTree->GetRootNode());

    decisionTree->GetPercentErrorOfTestingSet(*waldoTestingVec, *nonWaldoTestingVec);
    qDebug() << "Finished getting %error.";

    DrawImages();
}

MainWindow::~MainWindow()
{
    delete ui;

    /*Delete all pointers to Waldo images.*/
    if(mWaldoImageVec)
    {
        for(int filterIndex = 0; filterIndex < mWaldoImageVec->size(); filterIndex++)
        {
            if((*mWaldoImageVec)[filterIndex])
            {
                for(int imageIndex = 0; imageIndex < (*mWaldoImageVec)[filterIndex]->size(); imageIndex++)
                {
                    if((*(*mWaldoImageVec)[filterIndex])[imageIndex])
                    {
                        delete (*(*mWaldoImageVec)[filterIndex])[imageIndex];
                    }
                }
            }
        }
    }

    /*Delete all pointers to non-Waldo images.*/
    if(mNonWaldoImageVec)
    {
        for(int filterIndex = 0; filterIndex < mNonWaldoImageVec->size(); filterIndex++)
        {
            if((*mNonWaldoImageVec)[filterIndex])
            {
                for(int imageIndex = 0; imageIndex < (*mNonWaldoImageVec)[filterIndex]->size(); imageIndex++)
                {
                    if((*(*mNonWaldoImageVec)[filterIndex])[imageIndex])
                    {
                        delete (*(*mNonWaldoImageVec)[filterIndex])[imageIndex];
                    }
                }
            }
        }
    }
}

void MainWindow::InitWaldoImages()
{
    /*Init temp variables.*/
    QDir tempDir = QDir("Hey-Waldo-master/64/waldo/");
    QStringList tempStrList = tempDir.entryList(QDir::Files, QDir::LocaleAware);

    /*Store each non-filtered and filtered image of Waldo.*/
    mWaldoImageVec = new std::vector<std::vector<MyImage*>*>;

    //qDebug() << "Num filter types = " << (int)(MyImage::ftNum__FilterTypes);
    for(int filterIndex = 0; filterIndex < (int)(MyImage::ftNum__FilterTypes); filterIndex++)
    {
        //qDebug() << "filterIndex = " << filterIndex;
        mWaldoImageVec->push_back(new std::vector<MyImage*>);

        for(int imageIndex = 0; imageIndex < tempStrList.size(); imageIndex++)
        {
            std::string tempStr = "Hey-Waldo-master/64/waldo/" + tempStrList[imageIndex].toStdString();
            if(filterIndex == MyImage::ftNoFilter)
            {
                (*mWaldoImageVec)[filterIndex]->push_back(new MyImage(tempStr));
            }
            else
            {
                MyImage *newImage = new MyImage(*((*((*mWaldoImageVec)[MyImage::ftNoFilter]))[imageIndex]));

                newImage->ApplyFilter((MyImage::FilterType)filterIndex);
                (*mWaldoImageVec)[filterIndex]->push_back(newImage);

                /*Set the no-filter image's feature's value based on the feature
                  just set by the filtered image. The feature that has just been set
                  in the filtered image has the same index as the current filter index
                  minus one (to account for the ftNoFilter).*/
                FeatureSet::FeatureType featureType = (FeatureSet::FeatureType)(filterIndex - 1);
                int value = (*((*mWaldoImageVec)[filterIndex]))[imageIndex]->GetFeatureValue(featureType);
                (*((*mWaldoImageVec)[MyImage::ftNoFilter]))[imageIndex]->SetFeatureValue(featureType, value);
            }
        }
    }
}

void MainWindow::InitNonWaldoImages()
{
    /*Init temp variables.*/
    QDir tempDir = QDir("Hey-Waldo-master/64/notwaldo/");
    QStringList tempStrList = tempDir.entryList(QDir::Files, QDir::LocaleAware);

    /*Store each non-filtered and filtered image of Waldo.*/
    mNonWaldoImageVec = new std::vector<std::vector<MyImage*>*>;

    //qDebug() << "Num filter types = " << (int)(MyImage::ftNum__FilterTypes);
    for(int filterIndex = 0; filterIndex < (int)(MyImage::ftNum__FilterTypes); filterIndex++)
    {
        //qDebug() << "filterIndex = " << filterIndex;
        mNonWaldoImageVec->push_back(new std::vector<MyImage*>);

        for(int imageIndex = 0; imageIndex < tempStrList.size(); imageIndex++)
        {
            std::string tempStr = "Hey-Waldo-master/64/notwaldo/" + tempStrList[imageIndex].toStdString();
            if(filterIndex == MyImage::ftNoFilter)
            {
                (*mNonWaldoImageVec)[filterIndex]->push_back(new MyImage(tempStr));
            }
            else
            {
                MyImage *newImage = new MyImage(*((*((*mNonWaldoImageVec)[MyImage::ftNoFilter]))[imageIndex]));
                //qDebug() << "\t\t\tNo filter image address = " << ((*((*mNonWaldoImageVec)[MyImage::ftNoFilter]))[imageIndex]);
                newImage->ApplyFilter((MyImage::FilterType)filterIndex);
                //qDebug() << "Filter applied";
                (*mNonWaldoImageVec)[filterIndex]->push_back(newImage);

                /*Set the no-filter image's feature's value based on the feature
                  just set by the filtered image. The feature that has just been set
                  in the filtered image has the same index as the current filter index
                  minus one (to account for the ftNoFilter).*/
                FeatureSet::FeatureType featureType = (FeatureSet::FeatureType)(filterIndex - 1);
                int value = (*((*mNonWaldoImageVec)[filterIndex]))[imageIndex]->GetFeatureValue(featureType);
                (*((*mNonWaldoImageVec)[MyImage::ftNoFilter]))[imageIndex]->SetFeatureValue(featureType, value);
            }
        }
    }
}

void MainWindow::DrawImages() const
{
    /*Init variables.*/
    const int NUM_PER_ROW = 4;
    //const int OFFSET_X = MyImage::IMAGE_WIDTH; //Measured in pixels
    //const int OFFSET_Y = MyImage::IMAGE_HEIGHT; //Measured in pixels
    QRectF sceneRect = mDrawWindow->sceneRect();
    float centerX = (float)sceneRect.center().x();
    float centerY = (float)sceneRect.center().y();
    QGraphicsPixmapItem *TileGraphic;

    /*Draw Waldo images.*/
    for(int imageIndex = 0; imageIndex < (*mWaldoImageVec)[MyImage::ftNoFilter]->size(); imageIndex++)
    {
        TileGraphic = mDrawWindow->addPixmap(QPixmap::fromImage(*((*(*mWaldoImageVec)[MyImage::ftBlueOther])[imageIndex]->GetQImage())));
        TileGraphic->setOffset(centerX - MyImage::IMAGE_WIDTH * (NUM_PER_ROW / 2) + MyImage::IMAGE_WIDTH * (imageIndex % NUM_PER_ROW), centerY + (imageIndex / NUM_PER_ROW) * MyImage::IMAGE_HEIGHT); //sets Pixmap position
    }
}
