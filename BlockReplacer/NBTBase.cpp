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

#include "NBTBase.h"

NBTBase::NBTBase()
{
}

NBTBase::~NBTBase()
{
}

void NBTBase::setName(const std::string &name)
{
	m_name = name;
}

std::string NBTBase::getName() const
{
	return m_name;
}

void NBTBase::print(int ident)
{
	printTabs(ident);
	std::cout << "Base " << m_name << std::endl;
}

void NBTBase::printTabs(int num)
{
	for(int i = 0; i < num; i++)
		std::cout << "\t";
}

NBTBase *NBTBase::readNamedTag(PBuffer fle)
{
	unsigned char type = readByte(fle);
	NBTBase *base = newTag(type);

	if(base == NULL || type == 0)
		return base;

	base->setName(readString(fle));
	base->load(fle);
	return base;
}

void NBTBase::writeNamedTag(NBTBase *b, PBuffer fle)
{
	writeByte(fle, b->getId());

	if(b->getId() != 0) {
		writeString(fle, b->getName());
		b->write(fle);
	}
}

/************************************************ READING FUNCS ************************************************/

unsigned char NBTBase::readByte(PBuffer fle)
{
	unsigned char ret;
	readBuffer(fle, &ret, 1);
	return ret;
}

int NBTBase::readInt(PBuffer fle)
{
	int ret;
	readBuffer(fle, &ret, sizeof(int));
	return ((ret << 24) | ((ret <<  8) & 0x00FF0000) | ((ret >>  8) & 0x0000FF00) | ((ret >> 24) & 0x000000FF));
}

float NBTBase::readFloat(PBuffer fle)
{
	char ain[4];
	readBuffer(fle, ain, 4);

	float out;
	char *aout = (char*) &out;

	aout[0] = ain[3];
	aout[1] = ain[2];
	aout[2] = ain[1];
	aout[3] = ain[0];

	return out;
}

std::string NBTBase::readString(PBuffer fle)
{
	short sz = readShort(fle);
	char *str = new char[sz + 1];
	readBuffer(fle, str, sz);

	str[sz] = 0;
	std::string ret(str);
	delete[] str;

	return ret;
}

double NBTBase::readDouble(PBuffer fle)
{
	char ain[8];
	readBuffer(fle, ain, 8);

	double out;
	char *aout = (char*) &out;

	aout[0] = ain[7];
	aout[1] = ain[6];
	aout[2] = ain[5];
	aout[3] = ain[4];
	aout[4] = ain[3];
	aout[5] = ain[2];
	aout[6] = ain[1];
	aout[7] = ain[0];

	return out;
}

long long NBTBase::readLong(PBuffer fle)
{
	char ain[8];
	readBuffer(fle, ain, 8);

	long long out;
	char *aout = (char*) &out;

	aout[0] = ain[7];
	aout[1] = ain[6];
	aout[2] = ain[5];
	aout[3] = ain[4];
	aout[4] = ain[3];
	aout[5] = ain[2];
	aout[6] = ain[1];
	aout[7] = ain[0];

	return out;
}

short NBTBase::readShort(PBuffer fle)
{
	short ret;
	readBuffer(fle, &ret, sizeof(short));

	return (ret << 8) | ((ret >> 8) & 0x00FF);
}

/************************************************ WRITING FUNCS ************************************************/

void NBTBase::writeByte(PBuffer fle, unsigned char b)
{
	writeBuffer(fle, &b, 1);
}

void NBTBase::writeInt(PBuffer fle, int i)
{
	i = ((i << 24) | ((i <<  8) & 0x00FF0000) | ((i >>  8) & 0x0000FF00) | ((i >> 24) & 0x000000FF));
	writeBuffer(fle, &i, sizeof(int));
}

void NBTBase::writeFloat(PBuffer fle, float f)
{
	char *ain = (char*) &f;
	char aout[4];

	aout[0] = ain[3];
	aout[1] = ain[2];
	aout[2] = ain[1];
	aout[3] = ain[0];

	writeBuffer(fle, aout, 4);
}

void NBTBase::writeString(PBuffer fle, const std::string &str)
{
	short len = (short) str.size();
	writeShort(fle, len);
	writeBuffer(fle, str.c_str(), len);
}

void NBTBase::writeDouble(PBuffer fle, double d)
{
	char *ain = (char*) &d;
	char aout[8];

	aout[0] = ain[7];
	aout[1] = ain[6];
	aout[2] = ain[5];
	aout[3] = ain[4];
	aout[4] = ain[3];
	aout[5] = ain[2];
	aout[6] = ain[1];
	aout[7] = ain[0];

	writeBuffer(fle, aout, 8);
}

void NBTBase::writeLong(PBuffer fle, long long l)
{
	char *ain = (char*) &l;
	char aout[8];

	aout[0] = ain[7];
	aout[1] = ain[6];
	aout[2] = ain[5];
	aout[3] = ain[4];
	aout[4] = ain[3];
	aout[5] = ain[2];
	aout[6] = ain[1];
	aout[7] = ain[0];

	writeBuffer(fle, aout, 8);
}

void NBTBase::writeShort(PBuffer fle, short s)
{
	s = (s << 8) | ((s >> 8) & 0x00FF);
	writeBuffer(fle, &s, sizeof(short));
}

/************************************************ CLASS MAPPING ************************************************/

#include "NBTTagEnd.h"
#include "NBTTagByte.h"
#include "NBTTagShort.h"
#include "NBTTagInt.h"
#include "NBTTagLong.h"
#include "NBTTagFloat.h"
#include "NBTTagDouble.h"
#include "NBTTagByteArray.h"
#include "NBTTagString.h"
#include "NBTTagList.h"
#include "NBTTagCompound.h"
#include "NBTTagIntArray.h"

#define BEGIN_TAG_MAPPING() NBTBase *NBTBase::newTag(unsigned char type) {
#define REGISTER_TAG(id, cls) if(type == id) return new cls;
#define END_TAG_MAPPING() return NULL; }

BEGIN_TAG_MAPPING()
	REGISTER_TAG(0, NBTTagEnd)
	REGISTER_TAG(1, NBTTagByte)
	REGISTER_TAG(2, NBTTagShort)
	REGISTER_TAG(3, NBTTagInt)
	REGISTER_TAG(4, NBTTagLong)
	REGISTER_TAG(5, NBTTagFloat)
	REGISTER_TAG(6, NBTTagDouble)
	REGISTER_TAG(7, NBTTagByteArray)
	REGISTER_TAG(8, NBTTagString)
	REGISTER_TAG(9, NBTTagList)
	REGISTER_TAG(10, NBTTagCompound)
	REGISTER_TAG(11, NBTTagIntArray)
END_TAG_MAPPING()
