#include <stdio.h>
#include <fstream>
#include <algorithm>
#include <limits>
#include "PGMMetadata.h"
#include "PGMImageProcessor.h"


namespace PLZERI001{
PGMImageProcessor::PGMImageProcessor(const PGMMetadata &md): mdata(md)
{
    // Dynamically allocate memory from the heap
    data = new std::tuple<unsigned char, bool>*[mdata.height];
    for (int i = 0; i < mdata.height; ++i) {
        data[i] = new std::tuple<unsigned char, bool>[mdata.width];
    }
}

PGMImageProcessor::~PGMImageProcessor()
{
    if (data == nullptr) return;
    for (int i = 0; i < mdata.height; ++i) delete [] data[i];
    delete [] data;
}

PGMImageProcessor::PGMImageProcessor(const PGMImageProcessor &p)
{
    mdata = PGMMetadata(p.mdata);
    // Copy new data on the heap
    data = new std::tuple<unsigned char, bool>*[mdata.height];
    for (int i = 0; i < mdata.height; ++i) {
        data[i] = new std::tuple<unsigned char, bool>(*p.data[i]);
    }
}

PGMImageProcessor::PGMImageProcessor(PGMImageProcessor &&p)
{
    mdata = PGMMetadata(p.mdata);
    data = p.data;  // Steal p's pixel data
    p.data = nullptr;
    components = std::move(p.components);
}

PGMImageProcessor &PGMImageProcessor::operator=(const PGMImageProcessor &rhs)
{
    if (this != &rhs) {
        mdata = PGMMetadata(rhs.mdata);
        // Clear the previously set data, if applicable
        if (data != nullptr)
        {
            for (int i = 0; i < mdata.height; ++i) delete [] data[i];
            delete [] data;
        }
        // Copy new data
        if (rhs.data != nullptr) {
            data = new std::tuple<unsigned char, bool>*[mdata.height];
            for (int i = 0; i < mdata.height; ++i) {
                data[i] = new std::tuple<unsigned char, bool>(*rhs.data[i]);
            }
        };
    }
    return *this;
}

PGMImageProcessor &PGMImageProcessor::operator=(PGMImageProcessor &&rhs)
{
    if (this != &rhs) {
        mdata = PGMMetadata(rhs.mdata);
        // Clear the previously set data, if applicable
        if (data != nullptr) {
            for (int i = 0; i < mdata.height; ++i) delete [] data[i];
            delete [] data;
        }
        // Steal moveable processor's pixels
        if (rhs.data != nullptr) {
            data = rhs.data;
            rhs.data = nullptr;
        };
        components = std::move(rhs.components);
    }
    return *this;
}

int PGMImageProcessor::extractComponents(int threshold)
{
    extractComponents(threshold, -1);
}

int PGMImageProcessor::extractComponents(int threshold, int minValidSize)
{
    extractComponents(threshold, minValidSize, std::numeric_limits<int>::max());
}

int PGMImageProcessor::extractComponents(int threshold, int minValidSize, int maxValidSize)
{
    if (data == nullptr) return -1;
    int width = mdata.width; int height = mdata.height;
    for (int y = 0; y < mdata.height; ++y) {
        for (int x = 0; x < mdata.width; ++x) {
            // Continue if visited
            if (std::get<1>(data[y][x])) continue;
            if (std::get<0>(data[y][x]) > threshold) {
                ConnectedComponent component(mdata.width, mdata.height, data);
                component.seedAt(x, y, threshold);
                if (component.getSize() > minValidSize && component.getSize() < maxValidSize) {
                    components.insert(component);
                }
            // else if not VISITED
            } else {
                // Set to background
                std::get<0>(data[y][x]) = 0;
            }
        }
    }
    return components.size();
}

int PGMImageProcessor::filterComponentsBySize(int m, int M)
{
    // Couldn't get this to work using the STL :(
    // std::remove_if(components.begin(), components.end(),
    //     [m, M](ConnectedComponent& c)->bool{return c.getSize() < m || c.getSize() > M;});
    std::multiset<ConnectedComponent> componentsCopy;
    for (const ConnectedComponent &c: components) {
        if (c.getSize() > m && c.getSize() < M) componentsCopy.insert(c);
    }
    components = componentsCopy;
    return components.size();
}
bool PGMImageProcessor::writeComponents(const std::string &outFileName)
{
    std::ofstream outFile;
    outFile.open(outFileName, std::ios::binary);
    if (!outFile) {
        std::cout << "Unable to open output file." << std::endl;
        return false;
    }
    outFile << mdata;
    int i = 0;
    for (int y = 0; y < mdata.height; ++y) {
        for (int x = 0; x < mdata.width; ++x) {
            outFile << std::get<0>(data[y][x]);
        }
    }
    std::cout << "Wrote components to file at " << outFileName << std::endl;
    return true;
}
int PGMImageProcessor::getComponentCount(void) const { return components.size(); }

int PGMImageProcessor::getLargestSize(void) const {
    int largest = -1;
    for (const ConnectedComponent &c: components) {
        if (c.getSize() > largest) largest = c.getSize();
    }
    return largest;
}

int PGMImageProcessor::getSmallestSize(void) const {
    int smallest = std::numeric_limits<int>::max();
    for (const ConnectedComponent &c: components) {
        if (c.getSize() < smallest) smallest = c.getSize();
    }
    return smallest;
}

void PGMImageProcessor::printComponentData(const ConnectedComponent &c) const
{
    std::cout << c.to_string() << std::endl;
}

void PGMImageProcessor::printComponents(void) const
{
    for (ConnectedComponent c: components) printComponentData(c);
}

std::multiset<ConnectedComponent>::const_iterator PGMImageProcessor::begin()
{
    return components.begin();
}

std::multiset<ConnectedComponent>::const_iterator PGMImageProcessor::end()
{
    return components.end();
}

std::istream &operator>>(std::istream &stream, PGMImageProcessor &processor)
{
    for (int i = 0; i < processor.mdata.height; ++i) {
        for (int j = 0; j < processor.mdata.width; ++j) {
            processor.data[i][j] = std::make_tuple(stream.get(), false);
        }
    }
    return stream;
}
}