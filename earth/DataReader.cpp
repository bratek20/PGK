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
        //cout << "Read failed for file " << fileName << endl;
        return values;
    }
    //cout << "Reading: " << fileName << endl;

    values.reserve(SIZE);
    while(input.read(reinterpret_cast<char *>(&value), sizeof(short)) )
    {
        values.push_back(swapBytes(value));
    }
    input.close();
    fixData(values);
    return values;
}

static const int LEN = 1201;
static const int NO_DATA_OFF = -30000;

void DataReader::fixData(std::vector<short>& data){
    for(int i=0;i<LEN;i++){
        for(int j=0;j<LEN;j++){
            if(data[i*LEN + j] < NO_DATA_OFF){
                int lj = findGood(data, i, j, -1);
                int rj = findGood(data, i, j, 1);
                int off = data[i*LEN + rj] - data[i*LEN + lj]; 
                for(int mj = lj-1; mj<=rj-1; mj++){
                    data[i*LEN + j] = data[i*LEN + lj] + off * (mj - lj) / (rj - lj);
                }
            }
        }
    }
}

int DataReader::findGood(const std::vector<short>& data, int i, int j, int dir){
    while(data[i*LEN + j] < NO_DATA_OFF){
        j += dir;
    }
    return j;
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