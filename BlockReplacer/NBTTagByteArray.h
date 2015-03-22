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

#pragma once
#include "NBTBase.h"

class NBTTagByteArray : public NBTBase
{
	DECLARE_NBT_CLASS(7)

public:
	NBTTagByteArray();
	NBTTagByteArray(const std::string &name);
	NBTTagByteArray(const std::string &name, int len, unsigned char *buf);
	virtual ~NBTTagByteArray();

	int length;
	unsigned char *data;

	virtual void print(int ident = 0);
	virtual void write(PBuffer fle);
	virtual void load(PBuffer fle);
};
