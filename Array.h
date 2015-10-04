#pragma once

#include <new.h>

#define ARRAY_ALLOCATION_SIZE 16
#define ARRAY_MINIMUM_SIZE 0

template<class Element,unsigned long AllocationSize = ARRAY_ALLOCATION_SIZE,unsigned long MinimumSize = ARRAY_MINIMUM_SIZE>
class Array
{
public:
	Array(void);
	~Array(void);

	void Insert(unsigned long at,const Element& element);
	void InsertBack(const Element& element);
	void InsertFront(const Element& element);

	void Erase(unsigned long at,unsigned long count = 1);
	void Erase(void);
	void EraseBack(void);
	void EraseFront(void);

	void Delete(unsigned long at,unsigned long count = 1);
	void Delete(void);
	void DeleteBack(void);
	void DeleteFront(void);

	unsigned long GetBufferSize(void) const;
	unsigned long GetSize(void) const;

	Element& Front(void);
	const Element& Front(void) const;

	Element& Back(void);
	const Element& Back(void) const;

	Element& GetElement(unsigned long element);

	operator Element*(void);
	operator const Element*(void) const;

protected:
	Element*		mBuffer;
	unsigned long	mBufferSize;
	unsigned long	mSize;

protected:
	bool PrepareBuffer(unsigned long newSize,bool preserve);
	bool ResizeBuffer(unsigned long newSize,bool preserve);
};

template<class Element,unsigned long AllocationSize,unsigned long MinimumSize>
Array<Element,AllocationSize,MinimumSize>::Array(void) :
mBuffer(NULL),
mBufferSize(0),
mSize(0)
{
	// Prepare minimum size buffer if required
	ResizeBuffer(MinimumSize,false);
}

template<class Element,unsigned long AllocationSize,unsigned long MinimumSize>
Array<Element,AllocationSize,MinimumSize>::~Array(void)
{
	// Destroy buffer
	ResizeBuffer(0,false);
}

template<class Element,unsigned long AllocationSize,unsigned long MinimumSize>
bool Array<Element,AllocationSize,MinimumSize>::PrepareBuffer(unsigned long newSize,bool preserve)
{
	// No change
	if(mBufferSize == newSize)
		return true;

	// Are we shrinking
	if(newSize < mBufferSize)
		// Do nothing if at minimum buffer size or shrink not yet of allocation size
		if(mBufferSize == MinimumSize || mBufferSize - newSize < AllocationSize)
			return true;

	// Do shrink in allocation size steps ensuring minimum buffer size
	newSize = max(MinimumSize,newSize - newSize % AllocationSize + AllocationSize);

	return ResizeBuffer(newSize,preserve);
}

template<class Element,unsigned long AllocationSize,unsigned long MinimumSize>
bool Array<Element,AllocationSize,MinimumSize>::ResizeBuffer(unsigned long newSize,bool preserve)
{
	// No change
	if(mBufferSize == newSize)
		return true;

	// Destroy the buffer
	if(!newSize)
	{
		// Destruct all objects first
		for(unsigned long i = 0; i < mSize; ++i)
			mBuffer[i].~Element();

		delete[] mBuffer;

		mBuffer = NULL;
		mBufferSize = 0;
		mSize = 0;

		return true;
	}

	// Create the buffer
	if(!mBuffer)
	{
		mBuffer = new Element[newSize];
		if(!mBuffer)
			return false;

		mBufferSize = newSize;
		mSize = 0;

		return true;
	}

	// Create a new buffer
	Element* newBuffer = new Element[newSize];
	if(!newBuffer)
		return false;

	// Should we copy over the old content?
	if(preserve)
		CopyMemory(newBuffer,mBuffer,sizeof(Element) * mSize);	// TODO What if the new buffer size is smaller that m_Size
	else
	{
		// Else destroy old elements
		for(unsigned long i = 0; i < mSize; ++i)
			mBuffer[i].~Element();

		mSize = 0;
	}

	// Delete old buffer
	delete[] mBuffer;

	// Set new buffer as current
	mBuffer = newBuffer;
	mBufferSize = newSize;

	return true;
}

template<class Element,unsigned long AllocationSize,unsigned long MinimumSize>
void Array<Element,AllocationSize,MinimumSize>::Insert(unsigned long at,const Element& element)
{
	ASSERT(at <= mSize);

	PrepareBuffer(mSize + 1,true);

	MoveMemory(mBuffer + at + 1,mBuffer + at,sizeof(Element) * (mSize - at));

	::new(&mBuffer[at]) Element(element);
	
	mSize++;
}

template<class Element,unsigned long AllocationSize,unsigned long MinimumSize>
inline void Array<Element,AllocationSize,MinimumSize>::InsertBack(const Element& element)
{
	Insert(mSize,element);
}

template<class Element,unsigned long AllocationSize,unsigned long MinimumSize>
inline void Array<Element,AllocationSize,MinimumSize>::InsertFront(const Element& element)
{
	Insert(0,element);
}

template<class Element,unsigned long AllocationSize,unsigned long MinimumSize>
void Array<Element,AllocationSize,MinimumSize>::Erase(unsigned long at,unsigned long count)
{
	ASSERT(mBuffer);
	ASSERT(at + count <= mSize);

	for(unsigned long i = at; i < at + count; ++i)
		mBuffer[i].~Element();

	CopyMemory(mBuffer + at,mBuffer + at + count,sizeof(Element) * (mSize - at - count));
	
	mSize -= count;

	PrepareBuffer(mSize,true);
}

template<class Element,unsigned long AllocationSize,unsigned long MinimumSize>
inline void Array<Element,AllocationSize,MinimumSize>::Erase(void)
{
	if(!mSize) return;

	Erase(0,mSize);
}

template<class Element,unsigned long AllocationSize,unsigned long MinimumSize>
inline void Array<Element,AllocationSize,MinimumSize>::EraseBack(void)
{
	Erase(mSize - 1);
}

template<class Element,unsigned long AllocationSize,unsigned long MinimumSize>
inline void Array<Element,AllocationSize,MinimumSize>::EraseFront(void)
{
	Erase(0);
}

template<class Element,unsigned long AllocationSize,unsigned long MinimumSize>
void Array<Element,AllocationSize,MinimumSize>::Delete(unsigned long at,unsigned long count)
{
	ASSERT(mBuffer);
	ASSERT(at + count <= mSize);

	for(unsigned long i = at; i < at + count; ++i)
	{
		delete mBuffer[i];
		mBuffer[i].~Element();
	}

	CopyMemory(mBuffer + at,mBuffer + at + count,sizeof(Element) * (mSize - at - count));
	
	mSize -= count;

	PrepareBuffer(mSize,true);
}

template<class Element,unsigned long AllocationSize,unsigned long MinimumSize>
inline void Array<Element,AllocationSize,MinimumSize>::Delete(void)
{
	if(!mSize) return;

	Delete(0,mSize);
}

template<class Element,unsigned long AllocationSize,unsigned long MinimumSize>
inline void Array<Element,AllocationSize,MinimumSize>::DeleteBack(void)
{
	Delete(mSize - 1);
}

template<class Element,unsigned long AllocationSize,unsigned long MinimumSize>
inline void Array<Element,AllocationSize,MinimumSize>::DeleteFront(void)
{
	Delete(0);
}

template<class Element,unsigned long AllocationSize,unsigned long MinimumSize>
inline unsigned long Array<Element,AllocationSize,MinimumSize>::GetBufferSize(void) const
{
	return mBufferSize;
}

template<class Element,unsigned long AllocationSize,unsigned long MinimumSize>
inline unsigned long Array<Element,AllocationSize,MinimumSize>::GetSize(void) const
{
	return mSize;
}

template<class Element,unsigned long AllocationSize,unsigned long MinimumSize>
inline Element& Array<Element,AllocationSize,MinimumSize>::Front(void)
{
	ASSERT(mBuffer);

	return mBuffer[0];
}

template<class Element,unsigned long AllocationSize,unsigned long MinimumSize>
inline const Element& Array<Element,AllocationSize,MinimumSize>::Front(void) const
{
	ASSERT(mBuffer);

	return mBuffer[0];
}

template<class Element,unsigned long AllocationSize,unsigned long MinimumSize>
inline Element& Array<Element,AllocationSize,MinimumSize>::Back(void)
{
	ASSERT(mBuffer);

	return mBuffer[m_Size - 1];
}

template<class Element,unsigned long AllocationSize,unsigned long MinimumSize>
inline const Element& Array<Element,AllocationSize,MinimumSize>::Back(void) const
{
	ASSERT(mBuffer);

	return mBuffer[m_Size - 1];
}

template<class Element,unsigned long AllocationSize,unsigned long MinimumSize>
inline Element& Array<Element,AllocationSize,MinimumSize>::GetElement(unsigned long element)
{
	ASSERT(mBuffer);
	ASSERT(element < mSize);

	return mBuffer[element];
}

template<class Element,unsigned long AllocationSize,unsigned long MinimumSize>
inline Array<Element,AllocationSize,MinimumSize>::operator Element*(void)
{
	return mBuffer;
}

template<class Element,unsigned long AllocationSize,unsigned long MinimumSize>
inline Array<Element,AllocationSize,MinimumSize>::operator const Element*(void) const
{
	return mBuffer;
}