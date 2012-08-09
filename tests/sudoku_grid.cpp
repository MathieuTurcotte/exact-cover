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

using namespace std;
using namespace sudoku;

int main() {
    Grid<2, 2> sudoku;

    sudoku << "xxxx"
              "xxxx"
              "xx01"
              "xx23";

    assert(sudoku(2, 2) == 0);
    assert(sudoku(2, 3) == 1);
    assert(sudoku(3, 2) == 2);
    assert(sudoku(3, 3) == 3);

    Grid<2,2> copy(sudoku);
    assert(copy == sudoku);

    return 0;
}
