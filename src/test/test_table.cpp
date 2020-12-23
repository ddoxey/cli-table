#include <boost/filesystem.hpp>
#include "gtest/gtest.h"
#include "table.hpp"

namespace fs = boost::filesystem;


class TableTest : public ::testing::Test
{
protected:
    TableTest() { }
    virtual ~TableTest() { }
    virtual void TearDown() { }
};


TEST_F(TableTest, EmptyConstructor)
{
    Table table;
    std::stringstream result;
    result << table;
    EXPECT_EQ(result.str(), "");
}

TEST_F(TableTest, WidthConstructor)
{
    Table table(27);
    std::stringstream result;
    result << table;
    EXPECT_EQ(result.str(), "");
}

TEST_F(TableTest, TitleWidthConstructor)
{
    Table table("title", 27);
    std::stringstream result;
    result << table;
    std::stringstream expect;
    expect << "┌─────────────────────────┐\n"
           << "│          title          │\n"
           << "└─────────────────────────┘\n";
    EXPECT_EQ(result.str(), expect.str());
}

TEST_F(TableTest, TitleConstructor)
{
    Table table("title");
    std::stringstream result;
    result << table;
    std::stringstream expect;
    expect << "┌───────┐\n"
           << "│ title │\n"
           << "└───────┘\n";
    EXPECT_EQ(result.str(), expect.str());
}

TEST_F(TableTest, TitleOneTwoThree)
{
    Table table("title");
    table << "\n";
    table << "a";
    table << "\n";
    table << "b";
    table << "c";
    table << "\n";
    table << "d";
    table << "e";
    table << "f";
    std::stringstream result;
    result << table;
    std::stringstream expect;
    expect << "┌───────────┐\n"
           << "│   title   │\n"
           << "│     a     │\n"
           << "├─────┬─────┤\n"
           << "│ b   │ c   │\n"
           << "├───┬─┴─┬───┤\n"
           << "│ d │ e │ f │\n"
           << "└───┴───┴───┘\n";
    EXPECT_EQ(result.str(), expect.str());
}

TEST_F(TableTest, TitleOneTwoThreeIfstream)
{
    std::stringstream data;
    data << "title";
    data << "\n";
    data << "a";
    data << "\n";
    data << "b,c";
    data << "\n";
    data << "d,e,f";

    std::istringstream input(data.str().c_str());

    Table table(input);

    std::stringstream result;
    result << table;

    std::stringstream expect;
    expect << "┌───────────┐\n"
           << "│   title   │\n"
           << "│     a     │\n"
           << "├─────┬─────┤\n"
           << "│ b   │ c   │\n"
           << "├───┬─┴─┬───┤\n"
           << "│ d │ e │ f │\n"
           << "└───┴───┴───┘\n";
    EXPECT_EQ(result.str(), expect.str());
}


int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
