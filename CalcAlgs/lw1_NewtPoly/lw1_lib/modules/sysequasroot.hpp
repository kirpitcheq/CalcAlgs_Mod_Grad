#pragma once
#include "newtpolyroots.hpp"

namespace InterpKPEq{


class SysEquasRoot
{
private:
    SrcNodesType table1, table2;
    SrcNodesType auxtable, restable;
    using XYPair = std::pair<T, T>;
    T rootval;
    size_t polypowaux, polypowres;
public:
    auto getAuxTable() {return auxtable;}
    auto getResTable() {return restable;}
    auto getRootVal() {return rootval;}
    SysEquasRoot(const SrcNodesType &src1,
        const SrcNodesType &src2, 
        int polypowaux, 
        int polypowres) {
            // InterpKPEq::validTable(src1); //unique
            // InterpKPEq::validTable(src2); //and sort
            this->table1 = src1;
            this->table2 = src2;
            this->polypowaux = polypowaux;
            this->polypowres = polypowres;
        }
    ~SysEquasRoot() {}
    void calcAuxTable(){
        this->auxtable.clear();
        for(auto pair : this->table2){
            NewtPoly newtpol(this->table1);
            if(newtpol.isWrong())
                throw "Wrong 1 table";
            auto confans = newtpol.setConfig(pair.first, polypowaux);
            if(confans < 0)
                throw "Wrong polypowaux";
            auto calculated = newtpol.calc(); // here each node calculated
            if(calculated != std::nullopt)
                auxtable.push_back(XYPair(pair.first, calculated.value()));
            else
                throw "Can't calc";
        }
    }
    void calcResTable(){
        restable.clear();
        auto tablesize = this->auxtable.size();
        for (size_t i = 0; i < tablesize; i++)
        {
            restable.push_back(XYPair(this->table2[i].first,
                this->auxtable[i].second - this->table2[i].second));
        }
        
    }

    void findRoot(){
        NewtPolyRoots newtpolroot(this->restable, 
        this->polypowres);
        if(newtpolroot.isWrong())
            throw "wrong restable for find roots";
        this->rootval = newtpolroot.getRoot().value();
    }

};
}
