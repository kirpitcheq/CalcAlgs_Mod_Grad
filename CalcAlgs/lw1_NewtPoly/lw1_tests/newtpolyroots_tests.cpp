#include <gtest/gtest.h>
#include <KPEq/newtpolyroots.hpp>

using namespace KPEq::Interpoll;
TEST(NPRootsCtorTests, MustBeRoots){
    SrcNodesType srcnodes = {
        XYNodes(0,-3),
        // NewtPoly::XYNodes(3,0), //must be this root
        XYNodes(3,6),
    };

    NewtPolyRoots nproots(srcnodes, 1);

    EXPECT_FALSE(nproots.isWrong());

    auto result = nproots.getRoot();
    EXPECT_EQ(result.value(), 3);

}
