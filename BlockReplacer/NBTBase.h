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
#include "LazyIO.h"
#include <string>
#include <iostream>
#include <cstdlib>
#include <zlib.h>

#define DECLARE_NBT_CLASS(id) public: virtual unsigned char getId() const { return id; }

typedef struct tagRBuffer
{
	unsigned char *buf;
	unsigned int pos;
} *RBuffer;

class NBTBase
{
public:
	NBTBase();
	virtual ~NBTBase();

	void setName(const std::string &name);
	std::string getName() const;

	virtual unsigned char getId() const = 0;
	virtual void print(int ident = 0);
	virtual void write(PBuffer fle) = 0;
	virtual void load(PBuffer fle) = 0;

	static NBTBase *readNamedTag(PBuffer fle);
	static void writeNamedTag(NBTBase *b, PBuffer fle);
	static NBTBase *newTag(unsigned char type);

protected:
	//Reading funcs
	static unsigned char readByte(PBuffer fle);
	static int readInt(PBuffer fle);
	static float readFloat(PBuffer fle);
	static std::string readString(PBuffer fle);
	static double readDouble(PBuffer fle);
	static long long readLong(PBuffer fle);
	static short readShort(PBuffer fle);

	//Writing funcs
	static void writeByte(PBuffer fle, unsigned char b);
	static void writeInt(PBuffer fle, int i);
	static void writeFloat(PBuffer fle, float f);
	static void writeString(PBuffer fle, const std::string &str);
	static void writeDouble(PBuffer fle, double d);
	static void writeLong(PBuffer fle, long long l);
	static void writeShort(PBuffer fle, short s);

	//Print util
	static void printTabs(int num);

private:
	std::string m_name;
};
