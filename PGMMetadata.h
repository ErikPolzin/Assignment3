#ifndef PGM_H
#define PGM_H

#include <iostream>

namespace PLZERI001 {
struct PGMMetadata {

    int width, height, data_offset;
    PGMMetadata() = default;
    PGMMetadata(int w, int h);

    /// @brief Write PGM file metadata to a stream
    /// @param stream Output stream
    /// @param md PGM metadata
    /// @return Same stream
    friend std::ostream& operator<<(std::ostream& stream, const PGMMetadata& md);
    
    /// @brief Parse PGM metadata from the first lines of a file
    /// @param stream Input file
    /// @param md PGM metadata
    /// @return Same stream
    friend std::ifstream& operator>>(std::ifstream& stream, PGMMetadata& md);
};
}
#endif