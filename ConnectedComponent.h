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
        ConnectedComponent(int w, int h, std::tuple<unsigned char, bool> ** &d);
        void seedAt(int x, int y, int threshold);
        int getSize() const;
        int getBoundarySize();
        bool operator<(const ConnectedComponent &other) const;
        std::string to_string() const;
};
}
#endif