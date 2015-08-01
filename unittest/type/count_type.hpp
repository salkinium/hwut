/*
 * count_type.hpp
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

#ifndef HWUT_COUNT_TYPE_HPP
#define HWUT_COUNT_TYPE_HPP

#include <cstddef>
#include <stdint.h>

namespace hwut
{
	/**
	 * \brief	Data type to count the number of constructor etc. calls
	 * 
	 * \ingroup	hwut
	 */
	class CountType
	{
	public:
		CountType();

		CountType(const CountType& other);

		~CountType();

		CountType&
		operator = (const CountType& other);

		static void
		reset();

		static std::size_t numberOfDefaultConstructorCalls;
		static std::size_t numberOfCopyConstructorCalls;
		static std::size_t numberOfAssignments;
		static std::size_t numberOfDestructorCalls;
		static std::size_t numberOfReallocs;

		static std::size_t numberOfOperations;
	};
}

#endif // HWUT_COUNT_TYPE_HPP
