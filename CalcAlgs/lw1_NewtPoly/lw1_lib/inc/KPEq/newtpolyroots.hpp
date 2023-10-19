#pragma once
#include <KPEq/newtpoly.hpp>

namespace KPEq{
namespace Interpoll{

class NewtPolyRoots : public NewtPoly{
    #define YROOTZERO 0
    std::optional<T> root = 0;

public:
//    friend std::ostream& operator<<(std::ostream & out, const SrcNodesType& nodes);
    NewtPolyRoots(const SrcNodesType &srcnodes, size_t polypow);

    auto getRoot() { return root; }
};


}
}
