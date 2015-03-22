/**
 ** Copyright (C) 2014 BARBOTIN Nicolas
 ** 
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 ** 
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 ** 
 ** You should have received a copy of the GNU General Public License
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ** 
 **/

#include "NBTTagDouble.h"

NBTTagDouble::NBTTagDouble()
{
	data = 0;
}

NBTTagDouble::NBTTagDouble(const std::string &name)
{
	setName(name);
	data = 0;
}

NBTTagDouble::NBTTagDouble(const std::string &name, double value)
{
	setName(name);
	data = value;
}

NBTTagDouble::~NBTTagDouble()
{
}

void NBTTagDouble::print(int ident)
{
	printTabs(ident);
	std::cout << "Double " << getName() << " = " << data << std::endl;
}

void NBTTagDouble::write(PBuffer fle)
{
	writeDouble(fle, data);
}

void NBTTagDouble::load(PBuffer fle)
{
	data = readDouble(fle);
}
