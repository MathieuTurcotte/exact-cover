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
    Cell<4> c1;
    assert(c1.setted() == false);

    Cell<4> c2(3);
    assert(c2.setted() == true);
    assert(c2.get() == 3);

    assert(c1 != c2);
    c1.set(3);
    assert(c1 == c2);

    cout << c1 << endl;
    cout << c2 << endl;

    return 0;
}
