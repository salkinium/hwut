/*
 * harness.hpp
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

#ifndef	HWUT_TESTSUITE_HPP
#define	HWUT_TESTSUITE_HPP

#include "harness.hpp"

namespace hwut
{
	/**
	 * \brief	Base class for every test suite
	 * 
	 * \author	Fabian Greif
	 * \ingroup	hwut
	 */
	class TestSuite
	{
	public:
		virtual
		~TestSuite();

		virtual void
		setUp();

		virtual void
		tearDown();
	};
}

#endif	// HWUT_TESTSUITE_HPP
