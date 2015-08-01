/* Copyright (c) 2015, Roboterclub Aachen e.V.
 * This file is part of roboterclubaachen/hwut.
 *
 * roboterclubaachen/hwut is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * roboterclubaachen/hwut is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with roboterclubaachen/hwut.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdint.h>

class Awesome
{
public:
	inline
	Awesome() :
		value(0) {}

	inline
	void
	setValue(int32_t value)
	{ this->value = value; }

	inline
	int32_t
	getValue()
	{ return value; }

private:
	int32_t value;
};
