#include <iostream>
#include <string.h>
#include <fstream>
#include "PGMMetadata.h"

namespace PLZERI001 {

PGMMetadata::PGMMetadata(int w, int h): width(w), height(h), data_offset(0) {};

std::ifstream &operator>>(std::ifstream& stream, PGMMetadata& md)
{
    std::string line;
    std::getline(stream, line);
    while (stream.peek() == '#') std::getline(stream, line);
    stream >> md.width;
    stream >> md.height >> std::ws;
    std::getline(stream, line);
    md.data_offset = stream.tellg();
    return stream;
}
std::ostream &operator<<(std::ostream &stream, const PGMMetadata& md)
{
    stream << "P5" << std::endl << md.width << ' ' << md.height 
        << std::endl << "255" << std::endl;
    return stream;
}
}