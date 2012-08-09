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

#ifndef MPL_POWER_HPP_
#define MPL_POWER_HPP_

// Compile time power function.
template <int m, int n>
struct power {
    enum { value = m * power<m, n-1>::value };
};

template <int n>
struct power<n, 0> {
    enum { value = 1 };
};

#endif /* MPL_POWER_HPP_ */
