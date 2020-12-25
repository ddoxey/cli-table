#include "gtest/gtest.h"
#include "table.hpp"


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

TEST_F(TableTest, VariousTypes)
{
    short                  aa = 1;
    short int              ab = 2;
    signed short           ac = 3;
    signed short int       ad = 4;
    unsigned short         ae = 5;
    unsigned short int     af = 6;
    int                    ag = 7;
    signed                 ah = 8;
    signed int             ai = 9;
    unsigned               aj = 10;
    unsigned int           ak = 11;
    long                   al = 12;
    long int               am = 13;
    signed long            an = 14;
    signed long int        ao = 15;
    unsigned long          ap = 16;
    unsigned long int      aq = 17;
    unsigned long int      ar = 18;
    long long              as = 19;
    long long int          at = 20;
    signed long long       au = 21;
    signed long long int   av = 22;
    unsigned long long     aw = 23;
    unsigned long long int ax = 24;
    float                  ay = 25.1;
    double                 az = 26.2;
    long double            ba = 27.3;
    std::string            bb("bb");
    char                   bc[3] = "bc";
    char                   bd = '.';

    Table table("title");
    table << "\n";
    table << aa << ab << ac << ad << ae << af << "\n";
    table << ag << ah << ai << aj << ak << al << "\n";
    table << am << an << ao << ap << aq << ar << "\n";
    table << as << at << au << av << aw << ax << "\n";
    table << ay << az << ba << bb << bc << bd;

    std::stringstream result;
    result << table;
    std::stringstream expect;
    expect << "┌───────────────────────────────────┐\n"
           << "│               title               │\n"
           << "├──────┬──────┬──────┬────┬────┬────┤\n"
           << "│ 1    │ 2    │ 3    │ 4  │ 5  │ 6  │\n"
           << "│ 7    │ 8    │ 9    │ 10 │ 11 │ 12 │\n"
           << "│ 13   │ 14   │ 15   │ 16 │ 17 │ 18 │\n"
           << "│ 19   │ 20   │ 21   │ 22 │ 23 │ 24 │\n"
           << "│ 25.1 │ 26.2 │ 27.3 │ bb │ bc │ .  │\n"
           << "└──────┴──────┴──────┴────┴────┴────┘\n";
    EXPECT_EQ(result.str(), expect.str());
}

TEST_F(TableTest, StressColumnAutosizing)
{
    Table table("title");
    table << "\n";
    table << '.' << '.' << "\n";
    table << '.' << '.' << '.' << "\n";
    table << '.' << '.' << '.' << '.' << "\n";
    table << '.' << '.' << '.' << '.' << '.' << "\n";
    table << '.' << '.' << '.' << '.' << '.' << '.' << '.' << "\n";
    table << '.' << "\n";
    table << '.' << '.' << '.' << '.' << "\n";
    table << '.' << '.' << "\n";
    table << '.' << '.' << '.' << '.' << "\n";
    table << '.' << '.' << '.' << '.' << '.' << '.' << '.' << "\n";
    table << '.';

    std::stringstream result;
    result << table;
    std::stringstream expect;
    expect << "┌───────────────────────────┐\n"
           << "│           title           │\n"
           << "├─────────────┬─────────────┤\n"
           << "│ .           │ .           │\n"
           << "├─────────┬───┴────┬────────┤\n"
           << "│ .       │ .      │ .      │\n"
           << "├──────┬──┴───┬────┴─┬──────┤\n"
           << "│ .    │ .    │ .    │ .    │\n"
           << "├─────┬┴────┬─┴───┬──┴─┬────┤\n"
           << "│ .   │ .   │ .   │ .  │ .  │\n"
           << "├───┬─┴─┬───┼───┬─┴─┬──┴┬───┤\n"
           << "│ . │ . │ . │ . │ . │ . │ . │\n"
           << "├───┴───┴───┴───┴───┴───┴───┤\n"
           << "│             .             │\n"
           << "├──────┬──────┬──────┬──────┤\n"
           << "│ .    │ .    │ .    │ .    │\n"
           << "├──────┴──────┼──────┴──────┤\n"
           << "│ .           │ .           │\n"
           << "├──────┬──────┼──────┬──────┤\n"
           << "│ .    │ .    │ .    │ .    │\n"
           << "├───┬──┴┬───┬─┴─┬───┬┴──┬───┤\n"
           << "│ . │ . │ . │ . │ . │ . │ . │\n"
           << "├───┴───┴───┴───┴───┴───┴───┤\n"
           << "│             .             │\n"
           << "└───────────────────────────┘\n";
    EXPECT_EQ(result.str(), expect.str());
}



int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
