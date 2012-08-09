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

#include "types.hpp"
#include "mapped_matrix.hpp"

using namespace std;

int main() {
    MappedMatrix<int> m(2, 2);

    // Matrix should initially be empty.
    assert(m(0, 0) == int());
    assert(m(0, 1) == int());
    assert(m(1, 0) == int());
    assert(m(1, 1) == int());

    // Assignments should be retained.
    m(0, 0) = 1;
    m(0, 1) = 2;
    m(1, 0) = 3;
    m(1, 1) = 4;
    assert(m(0, 0) == 1);
    assert(m(0, 1) == 2);
    assert(m(1, 0) == 3);
    assert(m(1, 1) == 4);

    // Copy construction.
    MappedMatrix<int> copy(m);
    assert(copy(0, 0) == 1);
    assert(copy(0, 1) == 2);
    assert(copy(1, 0) == 3);
    assert(copy(1, 1) == 4);

    cout << m << endl;

    // Assignment construction.
    MappedMatrix<int> assigned = m;
    assert(assigned(0, 0) == 1);
    assert(assigned(0, 1) == 2);
    assert(assigned(1, 0) == 3);
    assert(assigned(1, 1) == 4);

    cout << assigned << endl;

    // Zero-value assignments should
    // reduce the number of elements
    // actually stored.
    m(0, 0) = int();
    m(0, 1) = int();
    m(1, 0) = int();
    m(1, 1) = int();
    assert(m(0, 0) == int());
    assert(m(0, 1) == int());
    assert(m(1, 0) == int());
    assert(m(1, 1) == int());

    // Direct copy between matrices.
    copy(0, 0) = m(0, 0);
    copy(0, 1) = m(0, 1);
    copy(1, 0) = m(1, 0);
    copy(1, 1) = m(1, 1);
    assert(copy(0, 0) == m(0, 0));
    assert(copy(0, 1) == m(0, 1));
    assert(copy(1, 0) == m(1, 0));
    assert(copy(1, 1) == m(1, 1));
    assert(copy(0, 0) == int());
    assert(copy(0, 1) == int());
    assert(copy(1, 0) == int());
    assert(copy(1, 1) == int());

    cout << copy;

    MappedMatrix<uint16> m5(2, 2);

    // Matrix should initially be empty.
    assert(m5(0, 0) == uint16());
    assert(m5(0, 1) == uint16());
    assert(m5(1, 0) == uint16());
    assert(m5(1, 1) == uint16());
    m5(0, 0) = 1;
    m5(0, 1) = 2;
    m5(1, 0) = 3;
    m5(1, 1) = 4;
    assert(m5(0, 0) == 1);
    assert(m5(0, 1) == 2);
    assert(m5(1, 0) == 3);
    assert(m5(1, 1) == 4);

    return 0;
}
