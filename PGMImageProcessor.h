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
        PGMImageProcessor& operator=(const PGMImageProcessor& rhs); // Copy Assignment Operator
        PGMImageProcessor& operator=(PGMImageProcessor&& rhs); // Move Assignment Operator
        /* Process the input image to extract all the connected components,
        based on the supplied threshold (0...255) and excluding any components
        of less than the minValidSize. The final number of components that
        you store in your container (after discarding undersized one)
        must be returned.
        */
        int extractComponents(int threshold, int minValidSize);
        /* Iterate - with an iterator - though a container of connected
        components and filter out (remove) all the components which do not
        obey the size criteria pass as arguments. The number remaining
        after this operation is returned.
        */
        int filterComponentsBySize(int minSize, int maxSize);
        /* Create a new PGM file which contains all current components
        (255=component pixel, 0 otherwise) and write this to outFileName as a
        valid PGM. the return value indicates success of operation
        */
        bool writeComponents(const std::string & outFileName);
        // Return number of components
        int getComponentCount(void) const;
        // Return number of pixels in largest component
        int getLargestSize(void) const;
        // Return number of pixels in smallest component
        int getSmallestSize(void) const;
        /* Print the data for a component to std::cout
        see ConnectedComponent class;
        print out to std::cout: component ID, number of pixels
        */
        void printComponentData(const ConnectedComponent & theComponent) const;
        /* Print all components */
        void printComponents(void) const;
        /* Read image data from a file stream */
        friend std::istream& operator>>(std::istream& stream, PGMImageProcessor& processor);
};
}
#endif