#include <KPEq/sysequasroot.hpp>

KPEq::Interpoll::SysEquasRoot::SysEquasRoot(const SrcNodesType &src1, const SrcNodesType &src2, int polypowaux, int polypowres) {
    // InterpKPEq::validTable(src1); //unique
    // InterpKPEq::validTable(src2); //and sort
    this->table1 = src1;
    this->table2 = src2;
    this->polypowaux = polypowaux;
    this->polypowres = polypowres;
}

void KPEq::Interpoll::SysEquasRoot::calcAuxTable(){
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

void KPEq::Interpoll::SysEquasRoot::calcResTable(){
    restable.clear();
    auto tablesize = this->auxtable.size();
    for (size_t i = 0; i < tablesize; i++)
    {
        restable.push_back(XYPair(this->table2[i].first,
                                  this->auxtable[i].second - this->table2[i].second));
    }

}

void KPEq::Interpoll::SysEquasRoot::findRoot(){
    NewtPolyRoots newtpolroot(this->restable,
                              this->polypowres);
    if(newtpolroot.isWrong())
        throw "wrong restable for find roots";
    this->rootval = newtpolroot.getRoot().value();
}

#include <KPEq/sysequasroot.hpp>
