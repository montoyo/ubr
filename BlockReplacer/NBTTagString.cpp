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

#include "NBTTagString.h"

NBTTagString::NBTTagString()
{
}

NBTTagString::NBTTagString(const std::string &name)
{
	setName(name);
}

NBTTagString::NBTTagString(const std::string &name, std::string value)
{
	setName(name);
	data = value;
}

NBTTagString::~NBTTagString()
{
}

void NBTTagString::print(int ident)
{
	printTabs(ident);
	std::cout << "String " << getName() << " = \"" << data << "\"" << std::endl;
}

void NBTTagString::write(PBuffer fle)
{
	writeString(fle, data);
}

void NBTTagString::load(PBuffer fle)
{
	data = readString(fle);
}
