#ifndef __TABLE_HPP_INCLUDED__
#define __TABLE_HPP_INCLUDED__

#include <string>
#include <memory>
#include <vector>
#include <sstream>
#include <iostream>
#include <nlohmann/json.hpp>

#define LIGHT_HORIZONTAL "\u2500"
#define HEAVY_HORIZONTAL "\u2501"
#define LIGHT_VERTICAL "\u2502"
#define HEAVY_VERTICAL "\u2503"
#define LIGHT_TRIPLE_DASH_HORIZONTAL "\u2504"
#define HEAVY_TRIPLE_DASH_HORIZONTAL "\u2505"
#define LIGHT_TRIPLE_DASH_VERTICAL "\u2506"
#define HEAVY_TRIPLE_DASH_VERTICAL "\u2507"
#define LIGHT_QUADRUPLE_DASH_HORIZONTAL "\u2508"
#define HEAVY_QUADRUPLE_DASH_HORIZONTAL "\u2509"
#define LIGHT_QUADRUPLE_DASH_VERTICAL "\u250a"
#define HEAVY_QUADRUPLE_DASH_VERTICAL "\u250b"
#define LIGHT_DOWN_AND_RIGHT "\u250c"
#define DOWN_LIGHT_AND_RIGHT_HEAVY "\u250d"
#define DOWN_HEAVY_AND_RIGHT_LIGHT "\u250e"
#define HEAVY_DOWN_AND_RIGHT "\u250f"
#define LIGHT_DOWN_AND_LEFT "\u2510"
#define DOWN_LIGHT_AND_LEFT_HEAVY "\u2511"
#define DOWN_HEAVY_AND_LEFT_LIGHT "\u2512"
#define HEAVY_DOWN_AND_LEFT "\u2513"
#define LIGHT_UP_AND_RIGHT "\u2514"
#define UP_LIGHT_AND_RIGHT_HEAVY "\u2515"
#define UP_HEAVY_AND_RIGHT_LIGHT "\u2516"
#define HEAVY_UP_AND_RIGHT "\u2517"
#define LIGHT_UP_AND_LEFT "\u2518"
#define UP_LIGHT_AND_LEFT_HEAVY "\u2519"
#define UP_HEAVY_AND_LEFT_LIGHT "\u251a"
#define HEAVY_UP_AND_LEFT "\u251b"
#define LIGHT_VERTICAL_AND_RIGHT "\u251c"
#define VERTICAL_LIGHT_AND_RIGHT_HEAVY "\u251d"
#define UP_HEAVY_AND_RIGHT_DOWN_LIGHT "\u251e"
#define DOWN_HEAVY_AND_RIGHT_UP_LIGHT "\u251f"
#define VERTICAL_HEAVY_AND_RIGHT_LIGHT "\u2520"
#define DOWN_LIGHT_AND_RIGHT_UP_HEAVY "\u2521"
#define UP_LIGHT_AND_RIGHT_DOWN_HEAVY "\u2522"
#define HEAVY_VERTICAL_AND_RIGHT "\u2523"
#define LIGHT_VERTICAL_AND_LEFT "\u2524"
#define VERTICAL_LIGHT_AND_LEFT_HEAVY "\u2525"
#define UP_HEAVY_AND_LEFT_DOWN_LIGHT "\u2526"
#define DOWN_HEAVY_AND_LEFT_UP_LIGHT "\u2527"
#define VERTICAL_HEAVY_AND_LEFT_LIGHT "\u2528"
#define DOWN_LIGHT_AND_LEFT_UP_HEAVY "\u2529"
#define UP_LIGHT_AND_LEFT_DOWN_HEAVY "\u252a"
#define HEAVY_VERTICAL_AND_LEFT "\u252b"
#define LIGHT_DOWN_AND_HORIZONTAL "\u252c"
#define LEFT_HEAVY_AND_RIGHT_DOWN_LIGHT "\u252d"
#define RIGHT_HEAVY_AND_LEFT_DOWN_LIGHT "\u252e"
#define DOWN_LIGHT_AND_HORIZONTAL_HEAVY "\u252f"
#define DOWN_HEAVY_AND_HORIZONTAL_LIGHT "\u2530"
#define RIGHT_LIGHT_AND_LEFT_DOWN_HEAVY "\u2531"
#define LEFT_LIGHT_AND_RIGHT_DOWN_HEAVY "\u2532"
#define HEAVY_DOWN_AND_HORIZONTAL "\u2533"
#define LIGHT_UP_AND_HORIZONTAL "\u2534"
#define LEFT_HEAVY_AND_RIGHT_UP_LIGHT "\u2535"
#define RIGHT_HEAVY_AND_LEFT_UP_LIGHT "\u2536"
#define UP_LIGHT_AND_HORIZONTAL_HEAVY "\u2537"
#define UP_HEAVY_AND_HORIZONTAL_LIGHT "\u2538"
#define RIGHT_LIGHT_AND_LEFT_UP_HEAVY "\u2539"
#define LEFT_LIGHT_AND_RIGHT_UP_HEAVY "\u253a"
#define HEAVY_UP_AND_HORIZONTAL "\u253b"
#define LIGHT_VERTICAL_AND_HORIZONTAL "\u253c"
#define LEFT_HEAVY_AND_RIGHT_VERTICAL_LIGHT "\u253d"
#define RIGHT_HEAVY_AND_LEFT_VERTICAL_LIGHT "\u253e"
#define VERTICAL_LIGHT_AND_HORIZONTAL_HEAVY "\u253f"
#define UP_HEAVY_AND_DOWN_HORIZONTAL_LIGHT "\u2540"
#define DOWN_HEAVY_AND_UP_HORIZONTAL_LIGHT "\u2541"
#define VERTICAL_HEAVY_AND_HORIZONTAL_LIGHT "\u2542"
#define LEFT_UP_HEAVY_AND_RIGHT_DOWN_LIGHT "\u2543"
#define RIGHT_UP_HEAVY_AND_LEFT_DOWN_LIGHT "\u2544"
#define LEFT_DOWN_HEAVY_AND_RIGHT_UP_LIGHT "\u2545"
#define RIGHT_DOWN_HEAVY_AND_LEFT_UP_LIGHT "\u2546"
#define DOWN_LIGHT_AND_UP_HORIZONTAL_HEAVY "\u2547"
#define UP_LIGHT_AND_DOWN_HORIZONTAL_HEAVY "\u2548"
#define RIGHT_LIGHT_AND_LEFT_VERTICAL_HEAVY "\u2549"
#define LEFT_LIGHT_AND_RIGHT_VERTICAL_HEAVY "\u254a"
#define HEAVY_VERTICAL_AND_HORIZONTAL "\u254b"
#define LIGHT_DOUBLE_DASH_HORIZONTAL "\u254c"
#define HEAVY_DOUBLE_DASH_HORIZONTAL "\u254d"
#define LIGHT_DOUBLE_DASH_VERTICAL "\u254e"
#define HEAVY_DOUBLE_DASH_VERTICAL "\u254f"
#define DOUBLE_HORIZONTAL "\u2550"
#define DOUBLE_VERTICAL "\u2551"
#define DOWN_SINGLE_AND_RIGHT_DOUBLE "\u2552"
#define DOWN_DOUBLE_AND_RIGHT_SINGLE "\u2553"
#define DOUBLE_DOWN_AND_RIGHT "\u2554"
#define DOWN_SINGLE_AND_LEFT_DOUBLE "\u2555"
#define DOWN_DOUBLE_AND_LEFT_SINGLE "\u2556"
#define DOUBLE_DOWN_AND_LEFT "\u2557"
#define UP_SINGLE_AND_RIGHT_DOUBLE "\u2558"
#define UP_DOUBLE_AND_RIGHT_SINGLE "\u2559"
#define DOUBLE_UP_AND_RIGHT "\u255a"
#define UP_SINGLE_AND_LEFT_DOUBLE "\u255b"
#define UP_DOUBLE_AND_LEFT_SINGLE "\u255c"
#define DOUBLE_UP_AND_LEFT "\u255d"
#define VERTICAL_SINGLE_AND_RIGHT_DOUBLE "\u255e"
#define VERTICAL_DOUBLE_AND_RIGHT_SINGLE "\u255f"
#define DOUBLE_VERTICAL_AND_RIGHT "\u2560"
#define VERTICAL_SINGLE_AND_LEFT_DOUBLE "\u2561"
#define VERTICAL_DOUBLE_AND_LEFT_SINGLE "\u2562"
#define DOUBLE_VERTICAL_AND_LEFT "\u2563"
#define DOWN_SINGLE_AND_HORIZONTAL_DOUBLE "\u2564"
#define DOWN_DOUBLE_AND_HORIZONTAL_SINGLE "\u2565"
#define DOUBLE_DOWN_AND_HORIZONTAL "\u2566"
#define UP_SINGLE_AND_HORIZONTAL_DOUBLE "\u2567"
#define UP_DOUBLE_AND_HORIZONTAL_SINGLE "\u2568"
#define DOUBLE_UP_AND_HORIZONTAL "\u2569"
#define VERTICAL_SINGLE_AND_HORIZONTAL_DOUBLE "\u256a"
#define VERTICAL_DOUBLE_AND_HORIZONTAL_SINGLE "\u256b"
#define DOUBLE_VERTICAL_AND_HORIZONTAL "\u256c"
#define LIGHT_ARC_DOWN_AND_RIGHT "\u256d"
#define LIGHT_ARC_DOWN_AND_LEFT "\u256e"
#define LIGHT_ARC_UP_AND_LEFT "\u256f"
#define LIGHT_ARC_UP_AND_RIGHT "\u2570"
#define LIGHT_DIAGONAL_UPPER_RIGHT_TO_LOWER_LEFT "\u2571"
#define LIGHT_DIAGONAL_UPPER_LEFT_TO_LOWER_RIGHT "\u2572"
#define LIGHT_DIAGONAL_CROSS "\u2573"
#define LIGHT_LEFT "\u2574"
#define LIGHT_UP "\u2575"
#define LIGHT_RIGHT "\u2576"
#define LIGHT_DOWN "\u2577"
#define HEAVY_LEFT "\u2578"
#define HEAVY_UP "\u2579"
#define HEAVY_RIGHT "\u257a"
#define HEAVY_DOWN "\u257b"
#define LIGHT_LEFT_AND_HEAVY_RIGHT "\u257c"
#define LIGHT_UP_AND_HEAVY_DOWN "\u257d"
#define HEAVY_LEFT_AND_LIGHT_RIGHT "\u257e"
#define HEAVY_UP_AND_LIGHT_DOWN "\u257f"
#define LIGHT_DIAGONAL_UPPER_CENTRE_TO_MIDDLE_LEFT "\u1fba0"
#define LIGHT_DIAGONAL_UPPER_CENTRE_TO_MIDDLE_RIGHT "\u1fba1"
#define LIGHT_DIAGONAL_MIDDLE_LEFT_TO_LOWER_CENTRE "\u1fba2"
#define LIGHT_DIAGONAL_MIDDLE_RIGHT_TO_LOWER_CENTRE "\u1fba3"
#define LIGHT_DIAGONAL_UPPER_CENTRE_TO_MIDDLE_LEFT_TO_LOWER_CENTRE "\u1fba4"
#define LIGHT_DIAGONAL_UPPER_CENTRE_TO_MIDDLE_RIGHT_TO_LOWER_CENTRE "\u1fba5"
#define LIGHT_DIAGONAL_MIDDLE_LEFT_TO_LOWER_CENTRE_TO_MIDDLE_RIGHT "\u1fba6"
#define LIGHT_DIAGONAL_MIDDLE_LEFT_TO_UPPER_CENTRE_TO_MIDDLE_RIGHT "\u1fba7"
#define LIGHT_DIAGONAL_UPPER_CENTRE_TO_MIDDLE_LEFT_AND_MIDDLE_RIGHT_TO_LOWER_CENTRE "\u1fba8"
#define LIGHT_DIAGONAL_UPPER_CENTRE_TO_MIDDLE_RIGHT_AND_MIDDLE_LEFT_TO_LOWER_CENTRE "\u1fba9"
#define LIGHT_DIAGONAL_UPPER_CENTRE_TO_MIDDLE_RIGHT_TO_LOWER_CENTRE_TO_MIDDLE_LEFT "\u1fbaa"
#define LIGHT_DIAGONAL_UPPER_CENTRE_TO_MIDDLE_LEFT_TO_LOWER_CENTRE_TO_MIDDLE_RIGHT "\u1fbab"
#define LIGHT_DIAGONAL_MIDDLE_LEFT_TO_UPPER_CENTRE_TO_MIDDLE_RIGHT_TO_LOWER_CENTRE "\u1fbac"
#define LIGHT_DIAGONAL_MIDDLE_RIGHT_TO_UPPER_CENTRE_TO_MIDDLE_LEFT_TO_LOWER_CENTRE "\u1fbad"
#define LIGHT_DIAGONAL_DIAMOND "\u1fbae"
#define LIGHT_HORIZONTAL_WITH_VERTICAL_STROKE "\u1fbaf"

using json = nlohmann::json;

enum Align { left, center, right, automatic };


class Table
{
public:
    class Tokenizer
    {
    public:
        Tokenizer() = delete;
        Tokenizer(const std::string &text);

        std::vector<std::string>::iterator begin() { return tokens_.begin(); }
        std::vector<std::string>::iterator end() { return tokens_.end(); }

    private:
        std::vector<std::string> tokens_{};
    };

    class Cell
    {
    public:
        Cell() = delete;
        Cell(const std::string &text);

        size_t length() const;
        void set_style(
            const std::pair<std::vector<size_t>, Align> &row_style,
            const std::pair<std::vector<size_t>, Align> &col_style,
            const std::pair<std::vector<size_t>, Align> &position_style);
        Align alignment() const;
        std::pair<std::string, std::string> sgr_codes() const;
        std::ostream& str(std::ostream &out) const;
        bool cmp(const std::string &subtext) const;

    private:
        std::vector<size_t> sgr;
        Align align = automatic;
        std::string text;
    };

    class Style
    {
    public:
        Style() {};

        void update(const json &config);

        std::pair<std::vector<size_t>, Align> row(
            size_t row_n,
            const std::shared_ptr<std::vector<std::shared_ptr<Table::Cell>>> &cols) const;

        std::pair<std::vector<size_t>, Align> col(
            size_t col_n,
            const std::shared_ptr<Table::Cell> &col) const;

        std::pair<std::vector<size_t>, Align> position(
            size_t row_n,
            size_t col_n,
            const std::shared_ptr<Table::Cell> &col) const;

    private:

        struct rule {
            int row_n = -1;
            int col_n = -1;
            int mod = -1;
            std::string text;
            Align align;
            std::vector<size_t> sgr;

            rule(int r_n, int c_n, int m, std::string &t, Align &a, std::vector<size_t> &s):
                row_n(r_n),
                col_n(c_n),
                mod(m),
                text(t),
                align(a),
                sgr(s) {}
        };

        std::vector<rule> row_rules;
        std::vector<rule> col_rules;
        std::vector<rule> pos_rules;
    };

    Table();
    Table(const size_t width);
    Table(const std::string &title);
    Table(const std::string &title, const size_t width);
    Table(std::istream &ifs);

    void set_style(const json &config);
    void add_header(const std::string &title);
    void add_row(const std::shared_ptr<std::vector<std::string>> &cols);
    void add_col(const std::string &col);
    std::ostream& render(std::ostream &out) const;

    template<typename T>
    Table& operator << (T const &value)
    {
        std::stringstream text;
        text << value;
        add_col(text.str());
        return *this;
    }

private:
    size_t minimum_table_width = 0;
    std::vector<std::shared_ptr<std::vector<std::shared_ptr<Cell>>>> rows;
    Style style;

    void horizontal(std::ostream &out, std::vector<std::shared_ptr<std::vector<size_t>>> &width_for, size_t index) const;
    void intermediate(std::ostream &out, std::vector<std::shared_ptr<std::vector<size_t>>> &width_for, size_t index) const;
    std::vector<std::shared_ptr<std::vector<size_t>>> compute_widths_() const;
};

std::ostream& operator << (std::ostream &out, const Table &t);
std::ostream& operator << (std::ostream &out, const Table::Cell &c);

#endif
