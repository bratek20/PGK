#include "DataReader.h"

#include <fstream>
#include <iostream>
using namespace std;

vector<short> DataReader::read(string fileName)
{
    ifstream input(fileName, ifstream::binary);
    static const unsigned SIZE = 1201 * 1201;
    short value;
    vector<short> values;
    if(input.fail()){
        cout << "Read failed for file " << fileName << endl;
        return values;
    }
    //cout << "Reading: " << fileName << endl;

    values.reserve(SIZE);
    while(input.read(reinterpret_cast<char *>(&value), sizeof(short)) )
    {
        values.push_back(swapBytes(value));
    }
    input.close();
    return values;
}

short DataReader::swapBytes(short val)
{
    char* cVal = reinterpret_cast<char*>(&val);
    short newVal;
    char* cNewVal = reinterpret_cast<char*>(&newVal);
    cNewVal[0] = cVal[1];
    cNewVal[1] = cVal[0];
    return newVal;
}