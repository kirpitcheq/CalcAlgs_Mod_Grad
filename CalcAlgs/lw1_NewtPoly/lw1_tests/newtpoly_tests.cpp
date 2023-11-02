#include <gtest/gtest.h>
#include <KPEq/newtpoly.hpp>
//#include "../lw1_lib/modules/newtpoly.hpp"

using namespace KPEq::Interpoll;
TEST(HelloTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}

TEST(UniquesTable, SameNodesY){
    SrcNodesType srctable =
    {
        XYNodes{0, 0},
        XYNodes{1, 0},
        XYNodes{2, 0},
        XYNodes{3, 0},
        XYNodes{4, 0},
    };
    NewtPoly newtpoly(srctable);
    EXPECT_FALSE(newtpoly.isWrong());
}

TEST(UniquesTable, SameNodesXFromBegin){
    SrcNodesType srctable =
    {
        XYNodes{0, 1}, //this
        XYNodes{0, 2}, //this
        XYNodes{2, 3},
        XYNodes{3, 4},
        XYNodes{4, 5},
    };
    NewtPoly newtpoly(srctable);
    EXPECT_TRUE(newtpoly.isWrong());
}

TEST(UniquesTable, SameNodesXFromMiddle){
    SrcNodesType srctable =
    {
        XYNodes{0, 1},
        XYNodes{1, 2},
        XYNodes{2, 3}, //here
        XYNodes{2, 4}, //and here
        XYNodes{4, 5},
    };
    NewtPoly newtpoly(srctable);
    EXPECT_TRUE(newtpoly.isWrong());
}


SrcNodesType srctable =
{
    XYNodes{-0.5, 0.707},
    XYNodes{-0.25, 0.924},
    XYNodes{0.0, 1.0},
    XYNodes{0.25, 0.924},
    XYNodes{0.5, 0.707},
    XYNodes{0.75, 0.383},
    XYNodes{1.0, 0},
};

TEST(ExtrapollationTest, ExampleGood){
    auto LessValueX = 0.6;
    auto polypow = 4;

    NewtPoly newtpoly(srctable);

    EXPECT_FALSE(newtpoly.isWrong());

    auto configerr = newtpoly.setConfig(LessValueX, polypow);

    EXPECT_EQ(configerr, 1); // good

    auto result = newtpoly.calc();

    EXPECT_EQ(newtpoly.getPolyPow(), 4); // must be same!
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), 0.58786559999999999);
}
TEST(ExtrapollationTest, ExampleGoodButAutoSmallPolyPow){
    auto LessValueX = 0.6;
    auto polypow = 6; // look at this

    NewtPoly newtpoly(srctable);

    EXPECT_FALSE(newtpoly.isWrong());

    auto configerr = newtpoly.setConfig(LessValueX, polypow);

    EXPECT_EQ(configerr, 1); // good

    auto result = newtpoly.calc();

    EXPECT_EQ(newtpoly.getPolyPow(), 4); // must be because decreasing after x=0
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), 0.58786559999999999);
}
TEST(ExtrapollationTest, LessEP){
    auto LessValueX = -0.7;
    auto polypow = 4;

    NewtPoly newtpoly(srctable);

    EXPECT_FALSE(newtpoly.isWrong());
    
    auto configerr = newtpoly.setConfig(LessValueX, polypow);
    
    EXPECT_EQ(configerr, 2); //extrapollation less
}
TEST(ExtrapollationTest, MoreEP){
    auto LessValueX = 1.3;
    auto polypow = 4;

    NewtPoly newtpoly(srctable);

    EXPECT_FALSE(newtpoly.isWrong());

    auto configerr = newtpoly.setConfig(LessValueX, polypow);

    EXPECT_EQ(configerr, 2); //extrapollation less

    auto result = newtpoly.calc();

    EXPECT_EQ(newtpoly.getPolyPow(), 4); // must be because decreasing after x=0
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), -0.44919840000000089); // -0.4490346 by excel
}

TEST(ExtrapollationTest, TooMoreEP){
    auto LessValueX = 21231.3;
    auto polypow = 4;

    NewtPoly newtpoly(srctable);

    EXPECT_FALSE(newtpoly.isWrong());

    auto configerr = newtpoly.setConfig(LessValueX, polypow);

    EXPECT_EQ(configerr, 2); //extrapollation less

    auto result = newtpoly.calc();

    EXPECT_EQ(newtpoly.getPolyPow(), 4); // must be because decreasing after x=0
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), 30344647211252780); // 3.0276924559799E+016 by excel
}
