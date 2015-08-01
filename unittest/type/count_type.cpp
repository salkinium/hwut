/*
 * count_type.cpp
 *
 * Copyright (c) 2009-2015, Roboterclub Aachen e.V.
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

#include "count_type.hpp"

std::size_t hwut::CountType::numberOfDefaultConstructorCalls = 0;
std::size_t hwut::CountType::numberOfCopyConstructorCalls = 0;
std::size_t hwut::CountType::numberOfAssignments = 0;
std::size_t hwut::CountType::numberOfDestructorCalls = 0;
std::size_t hwut::CountType::numberOfReallocs = 0;

std::size_t hwut::CountType::numberOfOperations = 0;

hwut::CountType::CountType()
{
	++numberOfDefaultConstructorCalls;
	++numberOfOperations;
}

hwut::CountType::CountType(const CountType&)
{
	++numberOfCopyConstructorCalls;
	++numberOfOperations;
}

hwut::CountType::~CountType()
{
	++numberOfDestructorCalls;
	if (numberOfDestructorCalls == (numberOfCopyConstructorCalls - numberOfDefaultConstructorCalls)) {
		++numberOfReallocs;
	}
	++numberOfOperations;
}

hwut::CountType&
hwut::CountType::operator = (const CountType&)
{
	++numberOfAssignments;
	++numberOfOperations;
	
	return *this;
}

void
hwut::CountType::reset()
{
	numberOfDefaultConstructorCalls = 0;
	numberOfCopyConstructorCalls = 0;
	numberOfAssignments = 0;
	numberOfDestructorCalls = 0;
	numberOfReallocs = 0;
	
	numberOfOperations = 0;
}
