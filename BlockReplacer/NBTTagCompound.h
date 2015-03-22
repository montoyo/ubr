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
#include <vector>

class NBTTagCompound : public NBTBase
{
	DECLARE_NBT_CLASS(10)

public:
	NBTTagCompound();
	NBTTagCompound(const std::string &name);
	virtual ~NBTTagCompound();

	virtual void print(int ident = 0);
	virtual void write(PBuffer fle);
	virtual void load(PBuffer fle);

	int getTagCount() const;
	NBTBase *getTag(int id);
	NBTBase *getTag(const std::string &name);
	
	template<class T> T *getTagEx(const std::string &name, int type)
	{
		NBTBase *ret = getTag(name);
		if(ret == NULL)
			return NULL;

		if(ret->getId() != type)
			return NULL;

		return static_cast<T*>(ret);
	}

	void clearTags(bool del = true);
	void addTag(NBTBase *tag);
	void removeTag(NBTBase *tag, bool del = false);
	void removeTag(const std::string &name, bool del = true);
	void removeTag(int id, bool del = true);

protected:
	std::vector<NBTBase*> m_data;
};
