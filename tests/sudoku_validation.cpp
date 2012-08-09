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
#include "sudoku_validation.hpp"

using namespace std;
using namespace sudoku;

int main() {
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

    assert(valid(instance) == true);
    assert(valid(solution) == true);

    instance << "x0x25xx4x"
                "xx1xxxxxx"
                "x4xx803xx"
                "76xxxxxxx"
                "4xx5x7xx6"
                "xxxxxxx80"
                "xx803xx5x"
                "xxxxxx6xx"
                "x7xx66x2x"; // Row error.

    assert(valid(instance) == false);

    instance << "x0x25xx4x"
                "xx1xxxxxx"
                "x4xx803xx"
                "76xxxxxxx"
                "4xx5x7xx6"
                "xxxxxxx80"
                "xx8036x5x"
                "xxxxxx6xx"
                "x7xx64x2x"; // Region error.

    assert(valid(instance) == false);

    return 0;
}
