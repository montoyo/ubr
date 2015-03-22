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

#include "NBTTagByteArray.h"
#include <cstring>

NBTTagByteArray::NBTTagByteArray()
{
	length = 0;
	data = NULL;
}

NBTTagByteArray::NBTTagByteArray(const std::string &name)
{
	setName(name);
	length = 0;
	data = NULL;
}

NBTTagByteArray::NBTTagByteArray(const std::string &name, int len, unsigned char *buf)
{
	setName(name);
	length = len;

	if(buf == NULL) {
		if(len > 0)
			data = new unsigned char[len];
		else
			data = NULL;
	} else {
		data = new unsigned char[len];
		memcpy(data, buf, len);
	}
}

NBTTagByteArray::~NBTTagByteArray()
{
	if(data != NULL)
		delete[] data;
}

void NBTTagByteArray::print(int ident)
{
	printTabs(ident);
	std::cout << "ByteArray " << getName() << "[" << length << "]" << std::endl;
}

void NBTTagByteArray::write(PBuffer fle)
{
	writeInt(fle, length);
	if(length > 0 && data != NULL)
		writeBuffer(fle, data, length);
}

void NBTTagByteArray::load(PBuffer fle)
{
	length = readInt(fle);

	if(length > 0) {
		if(data != NULL)
			delete[] data;

		data = new unsigned char[length];
		readBuffer(fle, data, length);
	}
}
