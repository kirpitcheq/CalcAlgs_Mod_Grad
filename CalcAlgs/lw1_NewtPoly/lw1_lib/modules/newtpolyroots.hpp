#pragma once
#include "newtpoly.hpp"

namespace InterpKPEq{

class NewtPolyRoots : public NewtPoly{
    #define YROOTZERO 0
    std::optional<T> root = 0;

public:
//    friend std::ostream& operator<<(std::ostream & out, const SrcNodesType& nodes);
    NewtPolyRoots(const SrcNodesType &srcnodes, size_t polypow) : NewtPoly(srcnodes){
        using namespace std;
        if(isWrong())
            return;
        auto& lastsrcnodes = NewtPoly::getSrcNodesProtect();
        for(auto i: lastsrcnodes){
            cout << i.first << ", " << i.second << endl;
        }
        //reverse x values and y values
        for(auto & i : lastsrcnodes){
            std::swap(i.first,i.second);
        }

        for(auto i: lastsrcnodes){
            cout << i.first << ", " << i.second << endl;
        }

        std::sort(begin(lastsrcnodes), end(lastsrcnodes), [](auto A, auto B){return A.first <= B.first;});//need sort by x nodesin
        auto end_of_unique = std::unique(begin(lastsrcnodes),end(lastsrcnodes), [](auto A, auto B){return A.first == B.first;});
        if(std::distance(begin(lastsrcnodes), end_of_unique) != srcnodes.size()) {
            lastsrcnodes.erase(end_of_unique, end(srcnodes));
        }

        if(setConfig(YROOTZERO, polypow) < 0){
            return; //or throw ?
        }

        this->root = calc(); //but here maybe several roots? And here maybe no roots on INTERPOLLATION
    }

    auto getRoot() { return root; }
    };


}
