#include <iostream>
#include <fstream>
#include <string.h>
#include <chrono>
#include <limits>
#include "PGMImageProcessor.h"

#ifndef CATCH_CONFIG_MAIN

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cout << "Specify path to PGM file" << std::endl;
        return 1;
    }
    // Default values when not specifying min/max
    int minValidSize = 3;
    int maxValidSize = std::numeric_limits<int>::max();
    int threshold = 128;
    bool printComponents = false;
    std::ifstream inputFile;
    std::string outFileName;
    
    inputFile.open(argv[argc-1], std::ios::in | std::ios::binary);
    if (!inputFile) {
        std::cout << "There was an error opening the input file" << std::endl;
        return 1;
    }

    for (int i = 1; i < argc; i++)
	{
        if (strcmp("-s", argv[i]) == 0) {
            minValidSize = std::abs(std::stoi(argv[++i]));
            maxValidSize = std::abs(std::stoi(argv[++i]));
        }
		else if (strcmp("-t", argv[i]) == 0) {
            threshold = std::abs(std::stoi(argv[++i]));
            if (!(0 <= threshold && threshold <= 255)) {
                std::cout << "Invalid boundary threshold" << std::endl;
                return 1;
            }
        }
        else if (strcmp("-p", argv[i]) == 0) printComponents = true;
        else if (strcmp("-w", argv[i]) == 0) outFileName = argv[++i];
	}
    PLZERI001::PGMMetadata md;
    inputFile >> md;
    PLZERI001::PGMImageProcessor processor(md);
    inputFile >> processor;
    auto t1 = std::chrono::high_resolution_clock::now();
    int ncomponents = processor.extractComponents(threshold, minValidSize, maxValidSize);
    auto t2 = std::chrono::high_resolution_clock::now();
    auto ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    std::cout << "Extracted completed in " << ms_int.count() << "ms\n";
    if (!outFileName.empty()) processor.writeComponents(outFileName);
    if (printComponents) processor.printComponents();
}
#endif