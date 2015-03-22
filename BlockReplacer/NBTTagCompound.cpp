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

#include "NBTTagCompound.h"

NBTTagCompound::NBTTagCompound()
{
}

NBTTagCompound::NBTTagCompound(const std::string &name)
{
	setName(name);
}

NBTTagCompound::~NBTTagCompound()
{
	clearTags(true);
}

void NBTTagCompound::print(int ident)
{
	printTabs(ident);
	std::cout << (getId() == 10 ? "Compound " : "List ") << getName() << " {" << std::endl;

	if(ident >= 7) {
		printTabs(ident + 1);
		std::cout << "TOO BIG" << std::endl;
	} else {
		for(int i = 0; i < m_data.size(); i++)
			m_data[i]->print(ident + 1);
	}

	printTabs(ident);
	std::cout << "}" << std::endl;
}

int NBTTagCompound::getTagCount() const
{
	return m_data.size();
}

NBTBase *NBTTagCompound::getTag(int id)
{
	return m_data[id];
}

NBTBase *NBTTagCompound::getTag(const std::string &name)
{
	for(int i = 0; i < m_data.size(); i++) {
		if(m_data[i]->getName() == name)
			return m_data[i];
	}

	return NULL;
}

void NBTTagCompound::clearTags(bool del)
{
	if(del) {
		for(int i = 0; i < m_data.size(); i++)
			delete m_data[i];
	}

	m_data.clear();
}

void NBTTagCompound::addTag(NBTBase *tag)
{
	if(tag == this)
		std::cout << "NBTTagCompound is trying to add itself!" << std::endl;
	else
		m_data.push_back(tag);
}

void NBTTagCompound::removeTag(NBTBase *tag, bool del)
{
	for(int i = 0; i < m_data.size(); i++) {
		if(m_data[i] == tag) {
			m_data.erase(m_data.begin() + i);
			break;
		}
	}

	if(del)
		delete tag;
}

void NBTTagCompound::removeTag(const std::string &name, bool del)
{
	for(int i = 0; i < m_data.size(); i++) {
		if(m_data[i]->getName() == name) {
			NBTBase *b = m_data[i];
			m_data.erase(m_data.begin() + i);

			if(del)
				delete b;

			break;
		}
	}
}

void NBTTagCompound::removeTag(int id, bool del)
{
	NBTBase *b = m_data[id];
	m_data.erase(m_data.begin() + id);

	if(del)
		delete b;
}

void NBTTagCompound::write(PBuffer fle)
{
	for(int i = 0; i < m_data.size(); i++)
		NBTBase::writeNamedTag(m_data[i], fle);

	writeByte(fle, 0); //Append end tag
}

void NBTTagCompound::load(PBuffer fle)
{
	clearTags(true);

	while(true) {
		NBTBase *b = NBTBase::readNamedTag(fle);
		if(b == NULL)
			continue;

		if(b->getId() == 0) {
			delete b; //Don't keep the end tag
			break;
		}

		addTag(b);
	}
}
