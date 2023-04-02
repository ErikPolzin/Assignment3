#ifndef ConnectedComponent_H
#define ConnectedComponent_H

#include <string>
#include <tuple>

namespace PLZERI001 {

class ConnectedComponent {
    private:
        short id;
        int size, boundarysize, width, height;
        std::tuple<unsigned char, bool> ** data;
    public:
        static short currentid;
        /// @brief Construct a connected component with references to the image data and dimensions
        /// @param w Image width
        /// @param h Image height
        /// @param d Image data
        ConnectedComponent(int w, int h, std::tuple<unsigned char, bool> ** &d);
        /// @brief Start seeding this component at x, y with a given threshold
        /// @param x X coordinate to seed at
        /// @param y Y coordinate to seed at
        /// @param threshold Pixel threshold
        void seedAt(int x, int y, int threshold);
        /// @brief Get the number of connected pixels in this component
        /// @return number of interior pixels
        int getSize() const;
        /// @brief Get the number of boundary pixels in this component
        /// @return number of boundary pixels
        int getBoundarySize() const;
        /// @brief Compare this component to another
        /// @param other other component
        /// @return Comparison based on the number of interior pixels
        bool operator<(const ConnectedComponent &other) const;
        /// @brief Get a string representation of this region
        /// @return string component
        std::string to_string() const;
};
}
#endif