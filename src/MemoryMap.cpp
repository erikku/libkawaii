#include "MemoryMap.h"

#if defined(Q_OS_WIN32)
#include <windows.h>
#else
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif

MemoryMap::MemoryMap(const QString& path, bool writable)
{
	mPath = path;
	mWritable = writable;

	mData = 0;
	mLength = 0;

#if defined(Q_OS_WIN32)
	mFile = CreateFileW(reinterpret_cast<LPCWSTR>( mPath.ucs2() ), mWritable ? (GENERIC_READ | GENERIC_WRITE) : GENERIC_READ,
		mWritable ? (FILE_SHARE_READ | FILE_SHARE_WRITE) : FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(mFile == INVALID_HANDLE_VALUE)
		return;

	mLength = GetFileSize(mFile, NULL);
	mMapFile = CreateFileMappingA(mFile, NULL, mWritable ? PAGE_READWRITE : PAGE_READONLY, 0, 0, NULL);
	if(mMapFile == NULL)
	{
		CloseHandle(mFile);
		return;
	}

	mData = (char*)MapViewOfFile(mMapFile, mWritable ? FILE_MAP_WRITE : FILE_MAP_READ, 0, 0, 0);
#else
	mDescriptor = open(mPath.toLocal8Bit().data(), mWritable ? O_RDWR : O_RDONLY);
	if(mDescriptor < 0)
		return;

	mLength = lseek(mDescriptor, 0, SEEK_END);
	mData = (char*)mmap(0, mLength, mWritable ? (PROT_READ | PROT_WRITE) : PROT_READ, MAP_SHARED, mDescriptor, 0);
#endif
};

MemoryMap::~MemoryMap()
{
#if defined(Q_OS_WIN32)
	if(mData)
		UnmapViewOfFile(mData);

	CloseHandle(mFile);
#else
	if(mData)
		munmap(mData, mLength);

	if(mDescriptor >= 0)
		close(mDescriptor);
#endif
};

char* MemoryMap::data()
{
	return mData;
};

char* MemoryMap::data() const
{
	return mData;
};

char* MemoryMap::constData() const
{
	return mData;
};

bool MemoryMap::isWritable() const
{
	return mWritable;
};

QString MemoryMap::path() const
{
	return mPath;
};

int MemoryMap::size() const
{
	return mLength;
};

int MemoryMap::length() const
{
	return mLength;
};
