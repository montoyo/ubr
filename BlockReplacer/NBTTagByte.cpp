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

#include "NBTTagByte.h"

NBTTagByte::NBTTagByte()
{
	data = 0;
}

NBTTagByte::NBTTagByte(const std::string &name)
{
	setName(name);
	data = 0;
}

NBTTagByte::NBTTagByte(const std::string &name, unsigned char value)
{
	setName(name);
	data = value;
}

NBTTagByte::~NBTTagByte()
{
}

void NBTTagByte::print(int ident)
{
	printTabs(ident);
	std::cout << "Byte " << getName() << " = " << (int) data << std::endl;
}

void NBTTagByte::write(PBuffer fle)
{
	writeByte(fle, data);
}

void NBTTagByte::load(PBuffer fle)
{
	data = readByte(fle);
}
