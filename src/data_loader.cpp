#include "data_loader.hpp"

float * DataLoader::LoadData(string fileName, float * & pData, float * & pLabel)
{
    /* Coding logic:

    Read text file

    Arrange data into floating number array format.

     */
    cout << "Loading data from: " << fileName << endl;

    // Hard code mFeatureLen
    mFeatureLen = 55;

    FILE * f = fopen(fileName.c_str(), "r");
    fpos_t * startPos = new fpos_t;
    fgetpos(f, startPos);

    try
    {
        if (f==NULL)
        {
            throw fileName;
        }
    }
    catch(string fileName)
    {
        cout << "Cannot open file: " << fileName << endl;
    }

    // go through the file once
    // to count the number of floats
    // and return some important parameters

    RunThroughFile(f);

    float * pNewData = new float[(mNumFloats/mNumFeatures-1) * mNumFeatures];
    //pData = new float[mNumFloats];
    float * pNewLabel = new float[mNumFeatures];

    pData = pNewData;
    pLabel = pNewLabel;

    fsetpos(f, startPos);

    AllocateData(f, pData, pLabel);

}

void DataLoader::RunThroughFile(FILE * f)
{
    cout << "Running through file ..." << endl;
    cout << "Feature #: ";
    mNumFloats = 0;
    mNumFeatures = 0;

    while(Peek(f) != EOF)
    {
        // read a single line of features
        do
        {
            //[floatCount] = ReadFloat(f);
            ReadFloat(f, false);
            mNumFloats ++ ;
            //floatCount ++;
        }
        while(Peek(f) != '\n');

        while (Peek(f) == ' ' || Peek(f) == '\n' || Peek(f) == '\t')
        {
            fgetc(f);
        }

        mNumFeatures ++ ;
        if (mNumFeatures % 5000 == 0)
        {
            cout << mNumFeatures << "  " << flush;
        }
    }

    cout << endl;
    cout << "mNumFloats: " << mNumFloats << endl;
    cout << "mNumFeatures: " << mNumFeatures << endl;
    cout << "mFeatureLen: " << mFeatureLen << endl;
}

void DataLoader::AllocateData(FILE * f, float * pData, float * pLabel)
{
    cout << "Allocating data ..." << endl;
    cout << "featureCount #: ";

    unsigned int dataFloatCount = 0;
    unsigned int featureCount = 0;

    while(Peek(f) != EOF)
    {
        float num = ReadFloat(f, true);
        pLabel[featureCount] = num;

        // read a single line of features
        do
        {
            pData[dataFloatCount] = ReadFloat(f, true);
            dataFloatCount ++;
        }
        while(Peek(f) != '\n');

        while (Peek(f) == ' ' || Peek(f) == '\n' || Peek(f) == '\t')
        {
            fgetc(f);
        }

        featureCount ++ ;
        if (featureCount % 5000 == 0)
        {
            cout << featureCount << "  " << flush;
        }
    }
    assert ((dataFloatCount + mNumFeatures) == mNumFloats);
    //assert (dataFloatCount == mNumFloats);
    assert (featureCount == mNumFeatures);
}

float DataLoader::ReadFloat(FILE * f, bool returnFloat)
{
    char cur;
    string curStr;
    do
    {
        cur = fgetc(f);
        if (returnFloat)
        {
            curStr.push_back(cur);
        }
    }
    while(cur != ',');

    try
    {
        if (curStr.size() == 0 && returnFloat)
        {
            throw 0;
        }
    }
    catch (int e)
    {
        cout << "Trying to Read a float failed with " << e << "StrLen." << endl;
    }

    if (returnFloat)
    {
        string::size_type sz;
        return ::atof(curStr.c_str());
    }
    else
    {
        return 0;
    }
}

char DataLoader::Peek(FILE *f)
{
    fpos_t * pos = new fpos_t;
    fgetpos(f, pos);
    char c = fgetc(f);
    fsetpos(f, pos);
    return c;
}

void DataLoader::ReturnStats(unsigned int & featureLen, unsigned int & numFeatures, unsigned int & numFloats)
{
    featureLen = mFeatureLen;
    numFeatures = mNumFeatures;
    numFloats = mNumFloats;
}
