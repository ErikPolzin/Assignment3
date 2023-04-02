#ifndef PGMImageProcessor_H
#define PGMImageProcessor_H

#include <string>
#include <set>
#include <tuple>
#include <stdio.h>
#include "ConnectedComponent.h"
#include "PGMMetadata.h"

namespace PLZERI001{

class PGMImageProcessor {

    private:
        std::multiset<ConnectedComponent> components;
        std::tuple<unsigned char, bool> ** data;
        PGMMetadata mdata;

    public:
        PGMImageProcessor(const PGMMetadata &md);
        ~PGMImageProcessor();
        PGMImageProcessor(const PGMImageProcessor &p);
        PGMImageProcessor(PGMImageProcessor &&p);
        PGMImageProcessor& operator=(const PGMImageProcessor& rhs);
        PGMImageProcessor& operator=(PGMImageProcessor&& rhs);
        int extractComponents(int threshold, int minValidSize);
        int filterComponentsBySize(int minSize, int maxSize);
        bool writeComponents(const std::string & outFileName);
        int getComponentCount(void) const;
        int getLargestSize(void) const;
        int getSmallestSize(void) const;
        void printComponentData(const ConnectedComponent & theComponent) const;
        void printComponents(void) const;
        friend std::istream& operator>>(std::istream& stream, PGMImageProcessor& processor);
};
}
#endif