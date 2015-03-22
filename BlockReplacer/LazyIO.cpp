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

#include "LazyIO.h"
#include <cstdlib>
#include <cstring>

void appendToBuffer(PBuffer src, PBuffer dst)
{
	if(src->size <= 0 || src->buf == NULL)
		return;

	if(dst->buf == NULL) {
		dst->buf = new unsigned char[src->size];
		dst->size = src->size;
		memcpy(dst->buf, src->buf, src->size);
	} else {
		unsigned char *nb = new unsigned char[dst->size + src->size];
		memcpy(nb, dst->buf, dst->size);
		memcpy(nb + dst->size, src->buf, src->size);

		delete[] dst->buf;
		dst->buf = nb;
		dst->size += src->size;
	}
}

void allocBuffer(PBuffer buf, unsigned int sz)
{
	buf->size = sz;
	buf->buf = new unsigned char[sz];
}

void makeBlankBuffer(PBuffer buf)
{
	buf->size = 0;
	buf->buf = NULL;
}

void freeBuffer(PBuffer buf)
{
	if(buf->buf != NULL)
		delete[] buf->buf;

	buf->buf = NULL;
	buf->size = 0;
}

void readBuffer(PBuffer buf, void *out, int sz)
{
	memcpy(out, buf->buf + buf->size, sz);
	buf->size += sz;
}

void writeBuffer(PBuffer buf, const void *in, int sz)
{
	if(buf->buf != NULL) //Used to check size
		memcpy(buf->buf + buf->size, in, sz);

	buf->size += sz;
}
