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

#ifndef SUDOKU_VALIDATION_H_
#define SUDOKU_VALIDATION_H_

#include <bitset>

#include "sudoku.hpp"
#include "types.hpp"

namespace sudoku {

namespace details {

template <size_t N>
bool set(std::bitset<N>& bset, uint32 pos) {
    bool old = bset.test(pos);
    bset.set(pos);
    return old;
}

} // details

// A grid is valid if each unit is a permutation of its digits.
// To check this condition, we simply use the std::set insert
// behavior to make sure that every element in a unit is unique.
template <uint16 Row, uint16 Col>
bool valid(const Grid<Row, Col>& sudoku) {
    std::bitset<Grid<Row, Col>::size> rows[Grid<Row, Col>::size];
    std::bitset<Grid<Row, Col>::size> cols[Grid<Row, Col>::size];

    for (uint32 i = 0; i < Grid<Row, Col>::size; i++) {
        for (uint32 j = 0; j < Grid<Row, Col>::size; j++) {
            if (sudoku(i, j).setted() &&
                details::set(rows[i], sudoku(i, j).get())) {
                return false;
            }

            if (sudoku(j, i).setted() &&
                details::set(cols[i], sudoku(j, i).get())) {
                return false;
            }
        }
    }

    std::bitset<Grid<Row, Col>::size> region;
    for (uint32 i = 0; i < Grid<Row, Col>::size; i++) {
        uint16 rowstart = Row * (i / Row);
        uint16 colstart = Col * (i % Col);

        for (uint32 row = rowstart; row < (rowstart + Row); row++) {
            for (uint32 col = colstart; col < (colstart + Col); col++) {
                if (sudoku(row, col).setted() &&
                    details::set(region, sudoku(row, col).get())) {
                    return false;
                }
            }
        }

        region.reset();
    }

    return true;
}

} // namespace sudoku

#endif // SUDOKU_VALIDATION_H_
