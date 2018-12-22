#include "DataReader.h"

#include <fstream>
#include <iostream>

using namespace std;

vector<short> DataReader::read(string fileName)
{
    ifstream input(fileName, ifstream::binary);
    static const unsigned SIZE = 1201 * 1201 * 2;
    char *buffer = new char[SIZE];
    input.read(buffer, SIZE);
    vector<short> values(SIZE/2);
    for (unsigned i = 0; i < SIZE/2; i++)
    {
        short val = (swapBites(buffer[2*i]) << 8) | swapBites(buffer[2*i+1]);
        cout << val << " ";
        values[i] = val;
    }

    delete[] buffer;
    input.close();
    return values;
}

short DataReader::swapBites(char val)
{
    return
        ((1<<7) & val) >> 7 |
        ((1<<6) & val) >> 5 |
        ((1<<5) & val) >> 3 |
        ((1<<4) & val) >> 1 |
        ((1<<3) & val) << 1 |
        ((1<<2) & val) << 3 |
        ((1<<1) & val) << 5 |
        ((1<<0) & val) << 7;
}