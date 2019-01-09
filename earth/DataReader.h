#ifndef DATA_READER_H
#define DATA_READER_H

#include <vector>
#include <string>

class DataReader{
public:
    static std::vector<short> read(std::string fileName); 
private:
    static void fixData(std::vector<short>& data);
    static short swapBytes(short val);
    static int findGood(const std::vector<short>& data, int i, int j, int dir);
};

#endif