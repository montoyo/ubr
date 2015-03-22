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

#include "NBTTagShort.h"

NBTTagShort::NBTTagShort()
{
	data = 0;
}

NBTTagShort::NBTTagShort(const std::string &name)
{
	setName(name);
	data = 0;
}

NBTTagShort::NBTTagShort(const std::string &name, short value)
{
	setName(name);
	data = value;
}

NBTTagShort::~NBTTagShort()
{
}

void NBTTagShort::print(int ident)
{
	printTabs(ident);
	std::cout << "Short " << getName() << " = " << data << std::endl;
}

void NBTTagShort::write(PBuffer fle)
{
	writeShort(fle, data);
}

void NBTTagShort::load(PBuffer fle)
{
	data = readShort(fle);
}
