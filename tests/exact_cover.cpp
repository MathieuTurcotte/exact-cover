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

#include <algorithm>
#include <iostream>
#include <cassert>
#include <vector>

#include "mapped_matrix.hpp"
#include "exact_cover.hpp"

using namespace std;

int main() {
    vector<size_t> cover;

    MappedMatrix<int> m1(2, 2);
    m1(0, 0) = 1;
    m1(1, 1) = 1;
    cover = exact_cover::solve(m1);
    assert(find(cover.begin(), cover.end(), 0) != cover.end());
    assert(find(cover.begin(), cover.end(), 1) != cover.end());

    MappedMatrix<int> m2(2, 2);
    m2(0, 1) = 1;
    m2(1, 1) = 1;
    cover = exact_cover::solve(m2);
    assert(cover.empty());

    return 0;
}
