#ifndef DATA_READER_H
#define DATA_READER_H

#include <vector>
#include <string>

class DataReader{
public:
    static std::vector<short> read(std::string fileName); 
private:
    static short swapBytes(short val);
};

#endif