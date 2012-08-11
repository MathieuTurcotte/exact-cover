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

#ifndef SPARSE_MATRIX_HPP_
#define SPARSE_MATRIX_HPP_

#include <stdexcept>
#include <utility>
#include <ostream>
#include <limits>
#include <vector>
#include <map>

#include "types.hpp"
#include "subscripts.hpp"

template <class Matrix>
class Proxy {
public:
    typedef typename Matrix::value_type value_type;

    Proxy(Matrix& matrix, const Subscript<uint32>& subscript) :
        matrix(matrix), subscript(subscript) {
    }

    Proxy& operator=(const Proxy& rhs) {
        matrix.set_value(subscript, value_type(rhs));
        return *this;
    }

    template <typename A>
    Proxy& operator=(const A& value) {
        matrix.set_value(subscript, value);
        return *this;
    }

    template <typename A>
    bool operator==(const A& rhs) const {
        return matrix.value_at(subscript) == value_type(rhs);
    }

    template <typename A>
    bool operator!=(const A& rhs) const {
        return matrix.value_at(subscript) != value_type(rhs);
    }

    /* ... */

    operator const value_type&() const {
        return matrix.value_at(subscript);
    }

private:
    Matrix& matrix;
    Subscript<uint32> subscript;
};

template <typename T>
class MappedMatrix {
public:
    typedef T value_type;

    MappedMatrix(size_t row, size_t col) :
        size(make_subscript(row, col)), zero() {
        data.resize(row);
    }

    const MappedMatrix& operator=(MappedMatrix rhs) {
        std::swap(data, rhs.data);
        size = rhs.size;
        return *this;
    }

    const T& operator()(uint32 row, uint32 col) const {
        if (row >= size.row || col >= size.col) {
            throw std::invalid_argument("Invalid subscripts.");
        }
        return value_at(make_subscript(row, col));
    }

    Proxy<MappedMatrix> operator()(uint32 row, uint32 col) {
        if (row >= size.row || col >= size.col) {
            throw std::invalid_argument("Invalid subscripts.");
        }
        return Proxy<MappedMatrix>(*this, make_subscript(row, col));
    }

    uint32 rows() const { return size.row; }
    uint32 cols() const { return size.col; }

private:
    template <class M>
    friend class Proxy;

    //! \brief Set the value of an element at a given subscript.
    void set_value(const Subscript<uint32>& subscript, const T& value) {
        std::map<uint32, T>& row = data[subscript.row];

        typedef typename std::map<uint32, T>::iterator iterator;
        iterator none = row.end();
        iterator elem = row.find(subscript.col);

        // Inserting a non-zero value over a zero value.
        // Inserting a non-zero value over a non-zero value.
        // Inserting a zero value over a non-zero value.
        // Inserting a zero value over a zero value: NOP.
        if (value != zero && elem == none) {
            row.insert(std::make_pair(subscript.col, value));
        } else if (value != zero && elem != none) {
            elem->second = value;
        } else if (value == zero && elem != none) {
            row.erase(elem);
        }
    }

    const T& value_at(const Subscript<uint32>& subscript) const {
        typedef typename std::map<uint32, T>::const_iterator iterator;
        const std::map<uint32, T>& row = data[subscript.row];
        iterator iter = row.find(subscript.col);
        return iter != row.end() ? iter->second : zero;
    }

    Subscript<uint32> size;                 // Matrix size (MxN)
    std::vector<std::map<uint32, T> > data; // A map associating subscripts to elements.
    T zero;                                 // The zero value for the current value type.
};

template <typename M, template <typename M> class Matrix>
std::ostream& operator<<(std::ostream& out, const Matrix<M>& m) {
    for (size_t i = 0; i < m.rows(); i++) {
        for (size_t j = 0; j < m.cols(); j++) {
            out << m(i, j) << " ";
        }
        out << std::endl;
    }
    return out;
}

#endif
