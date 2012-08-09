/*
 * Copyright (C) 2011 Mathieu Turcotte (mathieuturcotte.ca)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/.
 */

#include <iostream>
#include <cassert>

#include "sudoku.hpp"
#include "sudoku_solver_ng.hpp"

using namespace std;
using namespace sudoku;

void test_2x2() {
    Grid<2, 2> instance;
    Grid<2, 2> solution;

    instance << "xx3x"
                "0xx1"
                "xxx2"
                "x0xx";

    solution << "1230"
                "0321"
                "3102"
                "2013";

    assert(solve(instance) == solution);

    instance << "1xxx"
                "xxx0"
                "2xxx"
                "xxx2";

    solution << "1023"
                "3210"
                "2301"
                "0132";

    assert(solve(instance) == solution);

    instance << "x33x"
                "xxxx"
                "xxxx"
                "xxxx";

    solution << "xxxx"
                "xxxx"
                "xxxx"
                "xxxx";

    assert(solve(instance) == solution);
}

void test_3x3() {
    Grid<3, 3> instance;
    Grid<3, 3> solution;

    instance << "x0x25xx4x"
                "xx1xxxxxx"
                "x4xx803xx"
                "76xxxxxxx"
                "4xx5x7xx6"
                "xxxxxxx80"
                "xx803xx5x"
                "xxxxxx6xx"
                "x7xx64x2x";

    solution << "307256841"
                "851473062"
                "246180375"
                "762308514"
                "480517236"
                "513642780"
                "628031457"
                "134725608"
                "075864123";

    assert(solve(instance) == solution);

    instance << "xxx75xx3x"
                "xxx140x7x"
                "0xxxx28xx"
                "1xxxxx3x8"
                "xxx2x3xxx"
                "6x8xxxxx5"
                "xx08xxxx1"
                "x6x315xxx"
                "x5xx64xxx";

    solution << "416758032"
                "283140576"
                "075632814"
                "127506348"
                "504283167"
                "638471205"
                "340827651"
                "762315480"
                "851064723";

    assert(solve(instance) == solution);

    instance << "74xxx13xx"
                "61xxxxxx8"
                "xx3xxxxxx"
                "xxx0x6xx1"
                "2x4xxx8xx"
                "x3xxxxxxx"
                "xxxx7xx6x"
                "x06xxxxxx"
                "xxxx25x3x";

    solution << "748501326"
                "612743058"
                "053268417"
                "875036241"
                "264157803"
                "130482675"
                "321870564"
                "506314782"
                "487625130";

    assert(solve(instance) == solution);
}

int test_4x4() {
    Grid<4, 4> instance;
    Grid<4, 4> solution;

    instance << "x43xCxxxAxxx19xx"
                "xx7xx9xA1xxxxxx4"
                "810xDBxxxCxxxx5x"
                "xF5Axx4xx63xx70x"
                "x0xx9x26x874xDxB"
                "xxxxA8xxxxEDxxx3"
                "xxx4xxxxxxA6817x"
                "xxx8xxxD20xxxxA5"
                "2Exxxx784xxxAxxx"
                "xA452CxxxxxxBxxx"
                "0xxxBExxxxxxxxxx"
                "1xCx43DxE5xAxx2x"
                "x5Axx2Exx9xx7BDx"
                "x7xxxx1xxx2Fx43E"
                "3xxxxxx5Cx6xx8xx"
                "xx8ExxxFxxxBxxxx";

    solution << "e436c057adf219b8"
                "bc7df98a1e053264"
                "8102db639c47ea5f"
                "9f5ae142b638c70d"
                "a01c95263874fdeb"
                "5967a8b4f1ed20c3"
                "d3240fce5ba68179"
                "fbe8173d209c46a5"
                "2edb56784fc0a391"
                "7a452cf163d9be80"
                "06f3bea97281d54c"
                "18c943d0e5ba6f27"
                "45af82ec09137bd6"
                "c7906d1b8a2f543e"
                "3db17a05c46e98f2"
                "628e349fd75b0c1a";

    assert(solve(instance) == solution);
}

void test_5x5() {
    Grid<5, 5> sudoku;
    solve(sudoku);
}

void test_6x6() {
    Grid<6, 6> sudoku;
    solve(sudoku);
}

int main() {
    test_2x2();
    test_3x3();
    test_4x4();
    test_5x5();
    test_6x6();
    return 0;
}

