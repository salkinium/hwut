/*
 * awesome_test.cpp
 *
 * Copyright (c) 2015, Roboterclub Aachen e.V.
 *
 * This file is part of hwut.
 *
 * hwut is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * hwut is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with hwut.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "awesome_test.hpp"
#include "../awesome.hpp"

void
AwesomeTest::testGetterSetter()
{
	Awesome awesome;
	TEST_ASSERT_EQUALS(awesome.getValue(), 0);

	awesome.setValue(42);
	TEST_ASSERT_EQUALS(awesome.getValue(), 42);
}
