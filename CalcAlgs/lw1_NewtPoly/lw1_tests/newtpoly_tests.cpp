#include <gtest/gtest.h>
#include "../lw1_lib/modules/newtpoly.hpp"

TEST(HelloTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}

TEST(UniquesTable, SameNodesY){
    NewtPoly::SrcNodesType srctable = 
    {
        NewtPoly::XYNodes{0, 0},
        NewtPoly::XYNodes{1, 0},
        NewtPoly::XYNodes{2, 0},
        NewtPoly::XYNodes{3, 0},
        NewtPoly::XYNodes{4, 0},
    };
    NewtPoly newtpoly(srctable);
    EXPECT_FALSE(newtpoly.isWrong());
}

TEST(UniquesTable, SameNodesXFromBegin){
    NewtPoly::SrcNodesType srctable = 
    {
        NewtPoly::XYNodes{0, 1}, //this
        NewtPoly::XYNodes{0, 2}, //this
        NewtPoly::XYNodes{2, 3},
        NewtPoly::XYNodes{3, 4},
        NewtPoly::XYNodes{4, 5},
    };
    NewtPoly newtpoly(srctable);
    EXPECT_TRUE(newtpoly.isWrong());
}

TEST(UniquesTable, SameNodesXFromMiddle){
    NewtPoly::SrcNodesType srctable = 
    {
        NewtPoly::XYNodes{0, 1},
        NewtPoly::XYNodes{1, 2},
        NewtPoly::XYNodes{2, 3}, //here
        NewtPoly::XYNodes{2, 4}, //and here
        NewtPoly::XYNodes{4, 5},
    };
    NewtPoly newtpoly(srctable);
    EXPECT_TRUE(newtpoly.isWrong());
}

TEST(ExtrapollationTest, LessEP){
    auto LessValueX = -0.7;
    auto polypow = 4;

    NewtPoly::SrcNodesType srctable = 
    {
        NewtPoly::XYNodes{-0.5, 0.707},
        NewtPoly::XYNodes{-0.25, 0.924},
        NewtPoly::XYNodes{0.0, 1.0}, 
        NewtPoly::XYNodes{0.25, 0.924}, 
        NewtPoly::XYNodes{0.5, 0.707},
        NewtPoly::XYNodes{0.75, 0.383},
        NewtPoly::XYNodes{1.0, 0},
    };

    NewtPoly newtpoly(srctable);

    EXPECT_FALSE(newtpoly.isWrong());
    
    auto configerr = newtpoly.setConfig(LessValueX, polypow);
    
    EXPECT_TRUE(configerr == 2); //extrapollation

    auto result = newtpoly.calc();

    EXPECT_EQ(result.value(), 0.45331679999999946);
}