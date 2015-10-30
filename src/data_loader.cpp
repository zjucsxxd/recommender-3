/* Coding logic: 

Read text file

Arrange data into floating number array format. 

 */ 
DataLoader::DataLoader()
{
}

DataLoader::~DataLoader()
{
}

float * DataLoader::LoadData(string fileName)
{
    f = fopen(fileName.c_str(), 'r');
    fread(f, )

}
