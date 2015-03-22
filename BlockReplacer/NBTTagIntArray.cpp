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

#include "NBTTagIntArray.h"
#include <cstring>

NBTTagIntArray::NBTTagIntArray()
{
	length = 0;
	data = NULL;
}

NBTTagIntArray::NBTTagIntArray(const std::string &name)
{
	setName(name);
	length = 0;
	data = NULL;
}

NBTTagIntArray::NBTTagIntArray(const std::string &name, int len, int *buf)
{
	setName(name);
	length = len;

	if(buf == NULL) {
		if(len > 0)
			data = new int[len];
		else
			data = NULL;
	} else {
		data = new int[len];
		memcpy(data, buf, len * sizeof(int));
	}
}

NBTTagIntArray::~NBTTagIntArray()
{
	if(data != NULL)
		delete[] data;
}

void NBTTagIntArray::print(int ident)
{
	printTabs(ident);
	std::cout << "IntArray " << getName() << "[" << length << "]" << std::endl;
}

void NBTTagIntArray::write(PBuffer fle)
{
	writeInt(fle, length);
	if(length > 0 && data != NULL)
		writeBuffer(fle, data, length * sizeof(int));
}

void NBTTagIntArray::load(PBuffer fle)
{
	length = readInt(fle);

	if(length > 0) {
		if(data != NULL)
			delete[] data;

		data = new int[length];
		readBuffer(fle, data, length * sizeof(int));
	}
}
