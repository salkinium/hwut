/*
 * controller.hpp
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

#ifndef	HWUT_CONTROLLER_HPP
#define	HWUT_CONTROLLER_HPP

#include <xpcc/io/iostream.hpp>
#include <xpcc/architecture/driver/accessor/flash.hpp>

#include "reporter.hpp"

namespace hwut
{
	/**
	 * \brief	%Controller singleton
	 *
	 * Used to forward information from the test suites to the reporters.
	 * All macros form 'harness.hpp' use this class to determine the
	 * active reporter.
	 *
	 * \author	Fabian Greif 
	 * \ingroup	unittest
	 */
	class Controller
	{
	public:
		/// Get instance
		static inline Controller&
		instance() {
			static Controller controller;
			return controller;
		}

		/// Set a new reporter
		void
		setReporter(Reporter& reporter);

		/// Get currently active reporter
		Reporter&
		getReporter() const;

		/**
		 * \brief	Switch to the next test suite
		 *
		 * \param	name	Name of the test suite, used by the reporter
		 *					to generate messages
		 */
		void
		nextTestSuite(xpcc::accessor::Flash<char> name) const;

	private:
		Controller();

		Controller(const Controller&);

		Controller&
		operator =(const Controller&);

		Reporter *reporter;		//!< active reporter
	};
}

#endif	// HWUT_CONTROLLER_HPP
