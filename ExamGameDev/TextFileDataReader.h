#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

template<typename inputStruct>
class TextFileDataReader final
{
public:
    TextFileDataReader(const std::string& filePath);
    //Read the text file and return an array of the inputStruct objects
    const std::vector<inputStruct> ReadTextFile();

private:
    const std::string m_TextFileName;
};

template<typename inputStruct>
TextFileDataReader<inputStruct>::TextFileDataReader(const std::string& filePath) :
    m_TextFileName{ filePath }
{

}

template<typename inputStruct>
const std::vector<inputStruct> TextFileDataReader<inputStruct>::ReadTextFile()
{
    std::ifstream inputFile(m_TextFileName);

    if (inputFile.fail())
    {
        std::cerr << "Could not load " << m_TextFileName << "\n";
        return {};
    }
    else
    {
        std::vector<inputStruct> objectsInfoArray{};
        std::string currentTextFileLine{};

        if (std::getline(inputFile, currentTextFileLine) && currentTextFileLine[0] == '/')
        {
            //Skip comment line in txt file
        }

        while (std::getline(inputFile, currentTextFileLine)) //Read file line by line and add to the inputStructArray using the overloaded operater
        {
            std::istringstream inputString{ currentTextFileLine };
            inputStruct objectInfo{};

            inputString >> objectInfo;

            objectsInfoArray.emplace_back(objectInfo);
        }

        inputFile.close();

        return objectsInfoArray;
    }
}