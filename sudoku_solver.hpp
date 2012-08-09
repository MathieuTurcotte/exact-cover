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

#ifndef SUDOKU_SOLVER_HPP_
#define SUDOKU_SOLVER_HPP_

#include <utility>
#include <vector>

#include "types.hpp"
#include "power.hpp"
#include "sudoku.hpp"
#include "subscripts.hpp"
#include "exact_cover.hpp"
#include "mapped_matrix.hpp"

namespace sudoku {

template <uint16 Row, uint16 Col>
Grid<Row, Col> solve(const Grid<Row, Col>& sudoku) {
    // The solution to an exact cover problem is a list of row
    // indexes. We need a way to map those indexes to a value and
    // the corresponding cell in the Sudoku's grid. Thus the need
    // for this vector which, given a row index, allow us to find
    // the associated cell and its value in the solved grid.
    std::vector<std::pair<Subscript<uint16>, uint16> > index;

    // A sparse matrix large enough to old the exact cover instance.
    // Since it's a sparse matrix, we don't have to pay for this
    // excess storage.
    MappedMatrix<bool> bmatrix(power<Grid<Row, Col>::size, 3>::value,
                               power<Grid<Row, Col>::size, 2>::value * 4);

    uint32 irow = 0; // Instance matrix row index.

    // In order to fill the cover matrix,
    // go over each cell of the grid.
    for (uint16 row = 0; row < Grid<Row, Col>::size; row++) {
        for (uint16 col = 0; col < Grid<Row, Col>::size; col++) {

            // Current region index for this cell.
            uint16 region = row / Grid<Row, Col>::rows +
                            col / Grid<Row, Col>::columns * Grid<Row, Col>::columns;

            // Cell's domain start and stop values.
            uint16 start = 0;
            uint16 stop = Grid<Row, Col>::size;

            // If the cell's value is known, its
            // domain boils down this single value.
            if (sudoku(row, col).setted()) {
                start = sudoku(row, col).get();
                stop = start + 1;
            }

            // For each value in the cell domain, fill a row in the
            // sparse matrix holding the exact cover problem instance.
            for (uint16 value = start; value < stop; value++) {
                uint16 col0 = Grid<Row, Col>::num_cells * 0 +
                              Grid<Row, Col>::size * row + col;
                uint16 col1 = Grid<Row, Col>::num_cells * 1 +
                              Grid<Row, Col>::size * row + value;
                uint16 col2 = Grid<Row, Col>::num_cells * 2 +
                              Grid<Row, Col>::size * col + value;
                uint16 col3 = Grid<Row, Col>::num_cells * 3 +
                     region * Grid<Row, Col>::size + value;

                bmatrix(irow, col0) = true;
                bmatrix(irow, col1) = true;
                bmatrix(irow, col2) = true;
                bmatrix(irow, col3) = true;

                irow++; // Increment the instance matrix row index.

                // We've just inserted a new line in the instance matrix,
                // note in the index vector which cell is associated to this
                // row and what is the value associated to it for this entry.
                index.push_back(std::make_pair(make_subscript(row, col), value));
            }
        }
    }

    Grid<Row, Col> solution;
    std::vector<uint32> cover(exact_cover::solve(bmatrix));
    for (size_t i = 0; i < cover.size(); i++) {
        Subscript<uint16> subscript = index[cover[i]].first;
        uint16 value = index[cover[i]].second;
        solution(subscript.row, subscript.col) = value;
    }

    return solution;
}

} // namespace sudoku

#endif /* SUDOKU_SOLVER_HPP_ */
