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

#include "NBTTagList.h"

NBTTagList::NBTTagList()
{
}

NBTTagList::~NBTTagList()
{
	clearTags(true);
}

void NBTTagList::write(PBuffer fle)
{
	writeByte(fle, getContentType());
	writeInt(fle, m_data.size());

	for(int i = 0; i < m_data.size(); i++)
		m_data[i]->write(fle);
}

void NBTTagList::load(PBuffer fle)
{
	unsigned char type = readByte(fle);
	int len = readInt(fle);

	clearTags(true);
	for(int i = 0; i < len; i++) {
		NBTBase *b = NBTBase::newTag(type);
		b->load(fle);
		addTag(b);
	}
}

unsigned char NBTTagList::getContentType() const
{
	return ((unsigned char) (m_data.empty() ? 1 : m_data[0]->getId()));
}
