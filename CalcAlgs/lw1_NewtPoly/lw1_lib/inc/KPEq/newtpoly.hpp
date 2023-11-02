#pragma once
// #include "newtpoly.hpp"
#include <variant>
#include <vector>
#include <optional>
#include <algorithm>
#include <memory>
#include <iostream>

#define PRINT_VAL(VAL) std::cout<<#VAL<<"="<<VAL<<std::endl;
/*
 * Required:
 * - min 2 nodes!
 * - nodes must be difference
 * - first arg of node must be unique
 */
namespace KPEq{
namespace Interpoll{

using T = double;
using XYNodes = std::pair<T, T>;
using SrcNodesType = std::vector<XYNodes>;

class NewtPoly{
private:
    std::size_t workTblRows = 0;
    std::size_t workTblCols = 0;

    SrcNodesType srcnodes;
    std::size_t polypow;
    int err_code = 0; //-1 by def
    T targetval;
    int targetvalidx = -1;
    bool hasCache = false;

    std::unique_ptr<T*> uptr_worktable;

protected:
    auto& getSrcNodesProtect(){return srcnodes;}
public:
    std::size_t getWorkTblRows() const{return this->workTblRows;}
    std::size_t getWorkTblCols() const{return this->workTblCols;}

    NewtPoly(const SrcNodesType& nodesin, bool hasCache = false);
#ifdef ALL_IN_ONE_CTOR
    NewtPoly(const SrcNodesType& nodesin, std::size_t pow, T value)  //here with throw
        : NewtPoly(nodesin)
    {
        this->polypow = pow;
        calc();
    }
#endif
    ~NewtPoly(){ }

    bool isWrong() { return err_code != 0; }
    int setConfig(T value, std::size_t pow) ;
    std::optional<T> calc();
#define X_COL 0
    T calcValueWithPoly(T **worktable, size_t pow);

    enum class IntExtTypeDef {INTERPOLATION, EXTRAPOLATION};
    using IndexWithOpt = std::pair<T,IntExtTypeDef>;
    IndexWithOpt findIndexFrom(const std::vector<std::pair<T,T>>& nodes, T value);
    using IndxRngType = std::pair<size_t,size_t>;
    T** createWorkTable(const SrcNodesType& srcnodes, std::size_t pow, size_t idx); //return nullptr if wrong
    IndxRngType findIndxRange(int range, int len, int ref);
    //table need return with arg for printing, need value return
    void calcWorkTable(T** &worktable, size_t tablesize);
    T** getLastWorkTable() const;
private:
    SrcNodesType& sortSrcNodes(SrcNodesType & srcnodes){
        std::sort(std::begin(srcnodes), std::end(srcnodes), [](auto A, auto B){return A.first <= B.second;});
//        auto unigue_end = std::unigue(std::begin(srcnodes), std::end(srcnodes), [](auto A, auto B){return A.first == B.first;});
        return srcnodes;
    }

};
//NewtPolynomical only find polynom and than when it calculate, we can find any X value with this polynome, but need use Interpolation
}
}
