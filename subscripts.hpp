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

#ifndef SUBSCRIPT_HPP_
#define SUBSCRIPT_HPP_

// A Subscript is very much like an stl::pair, but it's
// targeted specifically to be used as an index in any matrix
// like data structure. It also provides a Strict Weak Ordering
// guaranty so a Subscript can be used as a key in the STL
// associative containers (map, multimap, etc.).
template <typename T>
struct Subscript {
    Subscript() :
        row(), col() {}

    Subscript(T row, T col) :
        row(row), col(col) {}

    T row, col;
};

// The standard menagerie of operator overloading.
template <typename T>
bool operator==(const Subscript<T>& a, const Subscript<T>& b) {
    return a.row == b.row && a.col == b.col;
}

template <typename T>
bool operator!=(const Subscript<T>& a, const Subscript<T>& b) {
    return a.row != b.row || a.col != b.col;
}

// Provide Strict Weak Ordering so a Subscript can be used as a
// key in the STL associative containers (map, multimap, etc.).
template <typename T>
bool operator<(const Subscript<T>& a, const Subscript<T>& b) {
    return a.row < b.row || (a.row == b.row && a.col < b.col);
}

template <typename T>
bool operator>(const Subscript<T>& a, const Subscript<T>& b) {
    return b < a;
}

template <typename T>
bool operator<=(const Subscript<T>& a, const Subscript<T>& b) {
    return !(b < a);
}

template <typename T>
bool operator>=(const Subscript<T>& a, const Subscript<T>& b) {
    return !(a < b);
}

// Convenience wrapper for creating a Subscript
// without explicitly having to specify the type.
template <typename T>
inline Subscript<T> make_subscript(T row, T col) {
    return Subscript<T>(row, col);
}

#endif /* SUBSCRIPT_HPP_ */
