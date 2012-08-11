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

#ifndef SUDOKU_H_
#define SUDOKU_H_

#include <cctype>
#include <vector>
#include <string>
#include <ostream>
#include <algorithm>
#include <stdexcept>

#include "types.hpp"

namespace sudoku {

// The obvious thing, a grid cell. Can be in two states:
// set and not set. In the set state, the cell has a value
// within its domain. Otherwise, the cell value is undefined
// and trying to access it is an error.
template <uint16 Domain = 9>
class Cell {
public:
    static const uint16 domain = Domain;

    Cell() : is_set(false) {}

    Cell(uint16 value) : value(value), is_set(true) {
        if (value >= Domain) {
            throw std::domain_error("Value out of cell's domain!");
        }
    }

    const Cell& operator=(const Cell& rhs) {
        is_set = rhs.is_set;
        value = rhs.value;
        return *this;
    }

    const Cell& operator=(uint16 value) {
        set(value);
        return *this;
    }

    uint16 get() const {
        return is_set ? value :
            throw std::logic_error("Cell's value isn't set.");
    }

    void set(uint16 value) {
        if (value >= Domain) {
            throw std::domain_error("Value out of cell's domain!");
        }

        is_set = true;
        this->value = value;
    }

    operator uint16() const { return get(); }
    bool setted() const { return is_set; }
    void reset() { is_set = false; }

private:
    uint16 value;   // Current cell value.
    bool is_set;    // Boolean to mark if the cell is set.
};

template <uint16 Domain>
bool operator==(const Cell<Domain>& a, const Cell<Domain>& b) {
    return a.setted() && b.setted()
         ? a.get() == b.get()
         : a.setted() == b.setted();
}

template <uint16 Domain>
bool operator!=(const Cell<Domain>& a, const Cell<Domain>& b) {
    return !operator==(a, b);
}

template <uint16 Domain>
std::ostream& operator<<(std::ostream& out, const Cell<Domain>& cell) {
    if (!cell.setted()) {
        return out << "-";
    }

    uint16 value = cell.get();
    return value < 10
         ? out << char(value + 48)
         : out << char(value + 55);
}

// Again, the obvious thing, a Sudoku grid. At its core,
// this is just a matrix of cells. No checks for validity
// are performed on the cells, i.e. two cells could have
// the same value in a row, a column, a region, etc. A
// grid is defined by its regions size (Row, Col).
template <uint16 Row = 3, uint16 Col = 3>
class Grid {
public:
    // Expose some static informations about the grid.
    static const uint16 size = Row * Col;
    static const uint16 rows = Row;
    static const uint16 columns = Col;
    static const uint16 num_cells = size * size;

    const Grid& operator=(const Grid& rhs) {
        for (uint16 i = 0; i < size; ++i) {
            for (uint16 j = 0; j < size; ++j) {
                cells[i][j] = rhs.cells[i][j];
            }
        }
        return *this;
    }

    void operator<<(std::string grid) {
        if (grid.size() != num_cells) {
            throw std::logic_error("Representation error !");
        }

        std::transform(grid.begin(), grid.end(), grid.begin(), ::tolower);

        for (uint16 i = 0; i < num_cells; i++) {
            uint16 row = i / size;
            uint16 col = i % size;

            if (grid[i] == 'x' || grid[i] == ' ') {
                cells[row][col].reset();
            } else if ('a' <= grid[i] && grid[i] <= 'w') {
                cells[row][col] = uint16(grid[i] - 87);
            } else if ('0' <= grid[i] && grid[i] <= '9') {
                cells[row][col] = uint16(grid[i] - 48);
            } else {
                throw std::logic_error("Value out of range!");
            }
        }
    }

    Cell<size>& operator()(uint16 row, uint16 col) {
        if (row >= size || col >= size) {
            throw std::out_of_range("Bad subscripts!");
        }
        return cells[row][col];
    }

    const Cell<size>& operator()(uint16 row, uint16 col) const {
        if (row >= size || col >= size) {
            throw std::out_of_range("Bad subscripts!");
        }
        return cells[row][col];
    }

protected:
    Cell<size> cells[size][size];   // A grid is just a matrix of cells.
};

template <uint16 Row, uint16 Col>
bool operator==(const Grid<Row, Col>& a, const Grid<Row, Col>& b) {
    for (uint16 i = 0; i < Grid<Row, Col>::size; ++i) {
        for (uint16 j = 0; j < Grid<Row, Col>::size; ++j) {
            if (a(i, j) != b(i, j)) {
                return false;
            }
        }
    }
    return true;
}

template <uint16 Row, uint16 Col>
bool operator!=(const Grid<Row, Col>& a, const Grid<Row, Col>& b) {
    return !operator==(a, b);
}

template <uint16 Row, uint16 Col>
std::ostream& operator<<(std::ostream& out, const Grid<Row, Col>& sudoku) {
    for (uint32 i = 0; i < Grid<Row, Col>::size; i++) {
        for (uint32 j = 0; j < Grid<Row, Col>::size; j++) {
            out << sudoku(i, j) << " ";
        }
        out << "\n";
    }
    return out;
}

} // namespace sudoku

#endif // SUDOKU_H_
