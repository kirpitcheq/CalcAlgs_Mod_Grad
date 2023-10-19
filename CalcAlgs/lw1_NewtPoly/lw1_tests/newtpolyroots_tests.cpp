#include <gtest/gtest.h>
#include "../lw1_lib/modules/newtpolyroots.hpp"

TEST(NPRootsCtorTests, MustBeRoots){
    NewtPoly::SrcNodesType srcnodes = {
        NewtPoly::XYNodes(0,-3),
        // NewtPoly::XYNodes(3,0), //must be this root
        NewtPoly::XYNodes(3,6),
    };

    NewtPolyRoots nproots(srcnodes, 1);

    EXPECT_FALSE(nproots.isWrong());

    auto result = nproots.getRoot();
    EXPECT_EQ(result.value(), 3);

}