#include <string>
#include <iostream>
#include <cassert>

using namespace std;

class DataLoader
{
    public:
        // main load data function
        float * LoadData(string fileName, float * & pData, float * & pLabel);

        // Code logic components
        void RunThroughFile(FILE * file);
        void AllocateData(FILE * file, float * pData, float * pLabel);

        // file read routines
        float ReadFloat(FILE * file, bool returnFloat);
        char Peek(FILE * file);

        // misc
        void ReturnStats(unsigned int & featureLen, unsigned int & numFeatures, unsigned int & numFloats);

    private:
        unsigned int mNumFloats;
        unsigned int mNumFeatures;
        unsigned int mFeatureLen;
};
