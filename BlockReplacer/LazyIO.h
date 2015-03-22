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

typedef struct tagBuffer {
	unsigned char *buf;
	unsigned int size;
} Buffer, *PBuffer;

void appendToBuffer(PBuffer src, PBuffer dst);
void allocBuffer(PBuffer buf, unsigned int sz);
void makeBlankBuffer(PBuffer buf);
void freeBuffer(PBuffer buf);
void readBuffer(PBuffer buf, void *out, int sz);
void writeBuffer(PBuffer buf, const void *in, int sz);
