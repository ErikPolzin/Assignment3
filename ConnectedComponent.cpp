#include <stack>
#include <array>
#include <string>
#include <sstream>
#include "ConnectedComponent.h"

#define SET(x, y, v) std::get<0>(data[y][x]) = (v); std::get<1>(data[y][x]) = true
#define INBOUNDS(x, y) (0 <= (x) && (x) < width && 0 <= (y) && (y) < height)
#define VISITED(x, y) bool(std::get<1>(data[y][x]))
#define INSIDE(x, y) ((std::get<0>(data[y][x]) > threshold) && (!VISITED(x, y)))

namespace PLZERI001 {

ConnectedComponent::ConnectedComponent(int w, int h, std::tuple<unsigned char, bool> ** &d)
: width(w), height(h), data(d), size(0), boundarysize(0), id(++currentid) {}

void ConnectedComponent::seedAt(int x, int y, int threshold) {
    // Stack holds (x, y) pixel positions that haven't been visited yet
    std::stack<std::array<int, 2>> s;
    s.push({x, y});  // Start at the seed position
    std::array<int, 2> coords;
    while (!s.empty()) {
        coords = s.top();
        x = coords[0]; y = coords[1];
        s.pop();
        if (!INBOUNDS(x, y)) continue;
        if (INSIDE(x, y)) {
            SET(x, y, 255);
            s.push({x-1, y});
            s.push({x, y-1});
            s.push({x+1, y});
            s.push({x, y+1});
            ++size;
        } else if (!VISITED(x, y)) {
            SET(x, y, 100);
            ++boundarysize;
        }
    }
}

int ConnectedComponent::getSize() const {return size;}
int ConnectedComponent::getBoundarySize() {return boundarysize;}
bool ConnectedComponent::operator<(const ConnectedComponent &other) const
{
    if (size < other.size) return true;
    return false;
}
std::string ConnectedComponent::to_string() const
{
    std::ostringstream os;
    os << "ConnectedComponent " << id << ": " << size << " pixels (" << boundarysize << " boundary)";
    return os.str();
};

short ConnectedComponent::currentid = 0;
}