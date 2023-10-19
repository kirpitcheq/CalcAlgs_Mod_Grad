#pragma once
#include <KPEq/newtpolyroots.hpp>

namespace KPEq{
namespace Interpoll{

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
        int polypowres);
    ~SysEquasRoot() {}
    void calcAuxTable();
    void calcResTable();

    void findRoot();

};
}
}
