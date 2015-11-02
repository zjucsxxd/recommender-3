//#include 'data_loader.h'
#include <iostream>
#include <cstdlib>
#include "data_loader.hpp"
#include "caffe/caffelib.hpp"

int main()
{
/* Coding logic: 

Run the data loader 

Initialize CaffeLib

Initialize the parameters 
    According to the Glorot & Bengio paper. 

SGD Iterate while iter < max_iters: 
    Compute gradient 
    Perform momentum descend udpate 

Save parameters 

*/
    DataLoader * loader = new DataLoader;
    std::string trainFileName("/home/youzhi.zou/juhuasuan/recommender/data/test.txt"); //20151016_train.txt");
    std::string valFileName("/home/youzhi.zou/juhuasuan/recommender/data/test.txt"); //20151016_val.txt");

    float * pTrainData;
    float * pTrainLabel;
    float * pValData;
    float * pValLabel;

    unsigned int featureLen, trainDataCount, trainNumFloats, valDataCount, valNumFloats;

    loader->LoadData(trainFileName, pTrainData, pTrainLabel);
    loader->ReturnStats(featureLen, trainDataCount, trainNumFloats);

    loader->LoadData(valFileName, pValData, pValLabel);
    loader->ReturnStats(featureLen, valDataCount, valNumFloats);

    std::cout << std::endl << "testing loaded data: " << std::endl;
    for (int i = 0; i < 50; i++)
    {
        std::cout << pTrainData[i] << " ";
    }
    std::cout << std::endl;
    for (int i = 0; i < 50; i++)
    {
        std::cout << pTrainLabel[i] << " ";
    }
    std::cout << std::endl;

    // ---- Initialize CaffeLib -----
    std::string modelPrototxt = "prototxt/dnn_memdata.prototxt";
    CaffeLib<float> * caffeLib = new CaffeLib<float>(modelPrototxt);

    // ---- the model parameters are automatically initialized
    long paramLen = caffeLib->ReturnParamLen();
    float * pParam = new float[paramLen];
    caffeLib->ReturnParam(pParam);
    float * pGrad = new float[paramLen];
    float * pStochData;
    float * pStochLabel;
    float * pStochValData;
    float * pStochValLabel;

    // ---- track training loss and validation acc
    float traLoss;
    float valLoss;
    float valAcc;

    // ---- optimization algorithm parameters
    float learningRate = 0.001;
    unsigned int miniBatchSize = 100;
    unsigned int validateEvery = 500;
    unsigned int maxIters = 100000;
    unsigned int printEvery = 100;

    for (int iter = 0; iter < maxIters; iter ++)
    {
        unsigned int ref = static_cast<unsigned int>(rand() % (trainDataCount - miniBatchSize + 1));
        pStochData = pTrainData + (ref) * featureLen;
        pStochLabel = pTrainLabel + ref;

        traLoss = caffeLib->ComputeGradientWithDatum(pParam, pGrad, pStochData, pStochLabel, miniBatchSize, 1, featureLen, 1);
        if (iter % printEvery == 0)
        {
            std::cout << "SGD Iter: " << iter << " trainLoss: " << traLoss << std::endl;
        }
        for (int i = 0; i < paramLen; i ++)
        {
            pParam[i] = pParam[i] - learningRate * pGrad[i];
        }

        if (iter % validateEvery == 0)
        {
            //[TODO] change this to validation accuracy
            caffeLib->SetParamsDense(pParam);

            unsigned int ref = static_cast<unsigned int>(rand() % (valDataCount - miniBatchSize + 1));
            pStochData = pValData + (ref) * featureLen;
            pStochLabel = pValLabel + ref;

            caffeLib->Validate(pStochData, pStochLabel, miniBatchSize, 1, featureLen, 1, valLoss, valAcc);
            std::cout << "Validating ... valloss: " << valLoss << "valacc: " << valAcc << std::endl;
        }
    }

    return 1;
}
