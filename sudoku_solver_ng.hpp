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

namespace sudoku {

template <uint16 Row, uint16 Col>
class SudokuBinaryMatrix {
public:
    struct RowDescriptor {
        RowDescriptor(uint16 row, uint16 col, uint16 value,
            uint32 col0, uint32 col1, uint32 col2, uint32 col3) :
            cell(Subscript<uint16>(row, col)), value(value) {
            cols[0] = col0;
            cols[1] = col1;
            cols[2] = col2;
            cols[3] = col3;
        }

        Subscript<uint16> cell;
        uint16 value;
        uint32 cols[4];
    };

    void operator<<(const Grid<Row, Col>& grid) {
        mrows.clear();

        for (uint16 row = 0; row < Grid<Row, Col>::size; row++) {
            for (uint16 col = 0; col < Grid<Row, Col>::size; col++) {
                // Current region index for this cell.
                uint16 region = row / Grid<Row, Col>::rows +
                                col / Grid<Row, Col>::columns *
                                      Grid<Row, Col>::columns;

                // Cell's domain start and stop values.
                uint16 start = 0;
                uint16 stop = Grid<Row, Col>::size;

                // If the cell's value is known, its
                // domain boils down this single value.
                if (grid(row, col).setted()) {
                    start = grid(row, col).get();
                    stop = start + 1;
                }

                // For each value in the cell domain, fill a row in
                // the sparse matrix representing the exact cover
                // problem instance.
                for (uint16 value = start; value < stop; value++) {
                    uint16 col0 = Grid<Row, Col>::num_cells * 0 +
                                  Grid<Row, Col>::size * row + col;
                    uint16 col1 = Grid<Row, Col>::num_cells * 1 +
                                  Grid<Row, Col>::size * row + value;
                    uint16 col2 = Grid<Row, Col>::num_cells * 2 +
                                  Grid<Row, Col>::size * col + value;
                    uint16 col3 = Grid<Row, Col>::num_cells * 3 +
                         region * Grid<Row, Col>::size + value;

                    mrows.push_back(RowDescriptor(row, col, value, col0,
                                                  col1, col2, col3));
                }
            }
        }
    }

    const SudokuBinaryMatrix& operator=(SudokuBinaryMatrix rhs) {
        std::swap(mrows, rhs.mrows);
        return *this;
    }

    bool operator()(uint32 row, uint32 col) const {
        uint16 quarter = col / power<Grid<Row, Col>::size, 2>::value;
        return mrows[row].cols[quarter] == col;
    }

    const RowDescriptor& operator[](uint32 row) const {
        return mrows[row];
    }

    uint32 rows() const { return mrows.size(); }
    uint32 cols() const { return power<Grid<Row, Col>::size, 2>::value * 4; }

private:
    std::vector<RowDescriptor> mrows;
};

template <uint16 Row, uint16 Col>
Grid<Row, Col> solve(const Grid<Row, Col>& sudoku) {
    Grid<Row, Col> solution;
    std::vector<uint32> cover;
    SudokuBinaryMatrix<Row, Col> matrix;

    matrix << sudoku;
    cover = exact_cover::solve(matrix);
    for (size_t i = 0; i < cover.size(); i++) {
        uint32 row = cover[i];
        uint16 value = matrix[row].value;
        Subscript<uint16> cell = matrix[row].cell;
        solution(cell.row, cell.col) = value;
    }

    return solution;
}

} // namespace sudoku

#endif /* SUDOKU_SOLVER_HPP_ */
