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
#include "NBTTagList.h"
#include "NBTTagByteArray.h"
#include "LazyIO.h"
#include "Mutex.h"
#include "OSDep.h"
#include <fstream>
#include <cstring>

static int sourceId = 3;
static int sourceMeta = 0;
static int targetId = 57;
static int targetMeta = 0;
static long long rpCount = 0;

static Mutex mutexA;
static Mutex mutexB;

typedef struct tagThreadData {
	std::string file;
	volatile bool *isRunning;
} ThreadData;

/************************************************ ANVIL ************************************************/

int readInt(std::fstream &in)
{
	int o;
	in.read((char*) &o, 4);

	return ((o << 24) | ((o <<  8) & 0x00FF0000) | ((o >>  8) & 0x0000FF00) | ((o >> 24) & 0x000000FF));
}

void writeInt(std::fstream &out, int o)
{
	o = ((o << 24) | ((o <<  8) & 0x00FF0000) | ((o >>  8) & 0x0000FF00) | ((o >> 24) & 0x000000FF));
	out.write((char*) &o, 4);
}

int getNibble(unsigned char *data, int x, int y, int z)
{
	int pos = y << 8 | z << 4 | x;
	int num = pos >> 1;
	int bit = pos & 1;

	if(bit == 0)
		return ((int) data[num]) & 15;
	else
		return ((int) data[num]) >> 4 & 15;
}

void setNibble(unsigned char *data, int x, int y, int z, int nb)
{
	int pos = y << 8 | z << 4 | x;
	int num = pos >> 1;
	int bit = pos & 1;

	if(bit == 0)
		data[num] = (unsigned char) (data[num] & 240 | nb & 15);
	else
		data[num] = (unsigned char) (data[num] & 15 | (nb & 16) << 4);
}

void processSection(NBTTagCompound *tag)
{
	NBTTagByteArray *lsbIds = tag->getTagEx<NBTTagByteArray>("Blocks", 7);
	NBTTagByteArray *msbIds = tag->getTagEx<NBTTagByteArray>("Add", 7);
	NBTTagByteArray *metas  = tag->getTagEx<NBTTagByteArray>("Data", 7);

	if(lsbIds == NULL || metas == NULL)
		return;

	if(msbIds == NULL) {
		if(sourceId > 255)
			return; //Nothing here!

		if(targetId > 255) { //Uhm... we need it!
			msbIds = new NBTTagByteArray("Add", 2048, NULL);
			memset(msbIds->data, 0, 2048);
			tag->addTag(msbIds);
		}
	}

	for(int x = 0; x < 16; x++) {
		for(int y = 0; y < 16; y++) {
			for(int z = 0; z < 16; z++) {
				int bid = lsbIds->data[y << 8 | z << 4 | x] & 0xFF;

				if(msbIds != NULL)
					bid |= getNibble(msbIds->data, x, y, z) << 8;

				if(bid == sourceId) {
					int meta = getNibble(metas->data, x, y, z);

					if(meta == sourceMeta) { //Target spotted! Replacing...
						lsbIds->data[y << 8 | z << 4 | x] = targetId & 0xFF;

						if(targetId > 255)
							setNibble(msbIds->data, x, y, z, (targetId & 3840) >> 8);
						else if(msbIds != NULL)
							setNibble(msbIds->data, x, y, z, 0);

						setNibble(metas->data, x, y, z, targetMeta);

						mutexB.lock();
						rpCount++;
						mutexB.unlock();
					}
				}
			}
		}
	}
}

void processChunk(std::fstream &in, int &csz)
{
	unsigned char ctype;
	in.read((char*) &ctype, 1);

	if(ctype != 1 && ctype != 2)
		return;

	int begPos = in.tellg();

	z_stream stream;
	stream.zalloc   = Z_NULL;
	stream.zfree    = Z_NULL;
	stream.opaque   = Z_NULL;
	stream.avail_in = 0;
	stream.next_in  = Z_NULL;

	if(inflateInit(&stream) != Z_OK) {
		std::cout << "inflateInit() failed!" << std::endl;
		return;
	}

	Buffer chunk, output;
	allocBuffer(&chunk, 1024);
	makeBlankBuffer(&output);

	unsigned char *buf = new unsigned char[csz];
	in.read((char*) buf, csz);

	stream.avail_in = csz;
	stream.next_in  = buf;

	do {
		stream.avail_out = chunk.size;
		stream.next_out  = chunk.buf;

		int err = inflate(&stream, Z_NO_FLUSH);
		if(err == Z_STREAM_ERROR || err == Z_NEED_DICT || err == Z_DATA_ERROR || err == Z_MEM_ERROR) {
			std::cout << "inflate() failed!" << std::endl;

			inflateEnd(&stream);
			freeBuffer(&chunk);
			freeBuffer(&output);

			delete[] buf;
			return;
		}

		chunk.size -= stream.avail_out;
		appendToBuffer(&chunk, &output);
		chunk.size = 1024;
	} while(stream.avail_out == 0);

	inflateEnd(&stream);
	freeBuffer(&chunk);
	delete[] buf;

	output.size = 0;
	NBTBase *base = NBTBase::readNamedTag(&output);
	freeBuffer(&output);

	if(base == NULL) {
		std::cout << "Couldn't read chunk NBT data!" << std::endl;
		return;
	}

	if(base->getId() != 10) {
		std::cout << "Invalid NBT data in chunk!" << std::endl;
		delete base;
		return;
	}

	NBTTagCompound *nbt = static_cast<NBTTagCompound*>(base);
	NBTTagCompound *lvl = nbt->getTagEx<NBTTagCompound>("Level", 10);

	if(lvl != NULL) {
		NBTTagList *sects = lvl->getTagEx<NBTTagList>("Sections", 9);

		if(sects != NULL && sects->getContentType() == 10) {
			for(int i = 0; i < sects->getTagCount(); i++) {
				if(sects->getTag(i) != NULL)
					processSection(static_cast<NBTTagCompound*>(sects->getTag(i)));
			}
		}
	}

	//Serialize NBT Data
	Buffer res;
	makeBlankBuffer(&res);
	NBTBase::writeNamedTag(nbt, &res);
	allocBuffer(&res, res.size);
	res.size = 0;
	NBTBase::writeNamedTag(nbt, &res);
	delete nbt;

	//Compress & write to file
	in.seekp(begPos, std::fstream::beg);
	csz = 0;

	stream.zalloc = Z_NULL;
	stream.zfree  = Z_NULL;
	stream.opaque = Z_NULL;

	if(deflateInit(&stream, Z_DEFAULT_COMPRESSION) != Z_OK) { //FIXME: Shouldn't we care about ctype????
		std::cout << "deflateInit() failed!" << std::endl;
		freeBuffer(&res);
		return;
	}

	stream.avail_in = res.size;
	stream.next_in  = res.buf;

	allocBuffer(&chunk, 1024);

	do {
		stream.avail_out = 1024;
		stream.next_out = chunk.buf;

		int err = deflate(&stream, Z_FINISH);
		if(err == Z_STREAM_ERROR) {
			std::cout << "deflate() failed!" << std::endl;

			deflateEnd(&stream);
			freeBuffer(&res);
			freeBuffer(&chunk);
			freeBuffer(&output);

			return;
		}

		chunk.size -= stream.avail_out;
		in.write((char*) chunk.buf, chunk.size);
		csz += chunk.size;
		chunk.size = 1024;
	} while(stream.avail_out == 0);

	deflateEnd(&stream);
	freeBuffer(&chunk);
	freeBuffer(&res);
}

void processRegion(const char *fn)
{
	std::fstream in(fn, std::istream::in | std::istream::out | std::istream::binary);
	if(!in) {
		std::cout << "Couldn't open region file " << fn << std::endl;
		return;
	}

	in.seekg(0, std::istream::end);
	int fs = (int) in.tellg();
	in.seekg(0, std::istream::beg);

	if(fs % 4096 != 0) {
		std::cout << "Invalid region file " << fn << std::endl;
		in.close();
		return;
	}

	int *offsets = new int[1024];
	for(int i = 0; i < 1024; i++)
		offsets[i] = readInt(in);

	for(int y = 0; y < 32; y++) {
		for(int x = 0; x < 32; x++) {
			int sid = x + y * 32;
			int fp  = (offsets[sid] >> 8) * 4096;

			if(fp >= fs)
				return;

			in.seekg(fp, std::fstream::beg);
			int sEnd = readInt(in);

			if(sEnd <= 0 || sEnd > 4096 * (offsets[sid] & 0xFF))
				continue;

			sEnd--;
			processChunk(in, sEnd);
			in.seekp(fp, std::fstream::beg);
			writeInt(in, sEnd + 1);
		}
	}

	delete[] offsets;
	in.close();
}

THREAD_FUNC(regionThread, ud)
{
	ThreadData *td = (ThreadData*) ud;
	processRegion(td->file.c_str());

	mutexA.lock();
	*td->isRunning = false;
	mutexA.unlock();

	return THREAD_RETVAL;
}

/************************************************ MAIN ************************************************/

int main(int argc, char **argv)
{
	if(argc < 5) {
		std::cout << "BlockReplacer.exe SourceId SourceMeta TargetId TargetMeta" << std::endl;
		return -10;
	}

	sourceId = strtol(argv[1], NULL, 10);
	sourceMeta = strtol(argv[2], NULL, 10);
	targetId = strtol(argv[3], NULL, 10);
	targetMeta = strtol(argv[4], NULL, 10);

	std::cout << "Replacing " << sourceId << ":" << sourceMeta << " by " << targetId << ":" << targetMeta << std::endl << std::endl;

	unsigned long start = getTimeMs();
	int cpuCount = getCPUCount();
	std::vector<std::string> files;

	if(!scanDir(".", files)) {
		std::cout << "Couldn't scan directory \".\"" << std::endl;
		return -8;
	}

	volatile bool *cores = new volatile bool[cpuCount];
	Thread *threads = new Thread[cpuCount];
	ThreadData *tdata = new ThreadData[cpuCount];

	for(int i = 0; i < cpuCount; i++) {
		cores[i] = false;
		setThreadInvalid(&threads[i]);
	}

	do {
		for(int i = 0; i < cpuCount; i++) {
			mutexA.lock();
			volatile bool hasTask = cores[i];
			mutexA.unlock();

			if(!hasTask) {
				if(isThreadValid(threads[i])) {
					freeThread(threads[i]);
					std::cout << "Thread " << i << " finished." << std::endl;
				}

				tdata[i].file = files[0];
				tdata[i].isRunning = &cores[i];
				std::cout << "Given " << files[0] << " to thread " << i << std::endl;
				files.erase(files.begin());

				cores[i] = true;
				makeThread(&threads[i], (void*) regionThread, (void*) &tdata[i]);
				setThreadCore(threads[i], i);

				if(files.empty())
					break;
			}
		}

		waitMs(25);
	} while(files.size());

	std::cout << "No more regions; waiting for all threads to finish..." << std::endl;

	for(int i = 0; i < cpuCount; i++) {
		if(isThreadValid(threads[i])) {
			waitForThread(threads[i]);
			freeThread(threads[i]);
		}
	}

	delete[] threads;
	delete[] cores;
	delete[] tdata;

	float spent = (float) (getTimeMs() - start);

	std::cout << std::endl << "Done: Process took " << spent / 1000.f << " seconds." << std::endl;
	std::cout << rpCount << " blocks were replaced." << std::endl;
	return 0;
}
