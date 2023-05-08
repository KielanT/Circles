#pragma once

template <typename T>
class PoolAllocator
{
    //-------------------------------------------------------------------------------------------------
    // Construction
    //-------------------------------------------------------------------------------------------------
public:
    PoolAllocator(uint32_t size) : mSize(size), mNextFree(0)
    {
        // Allocate space for the objects and a stack of free indexes
        // There is a method that only requires one array (it uses the unused object slots for the free indexes), but keeping it simple here
        mObjects = new uint8_t[mSize * sizeof(T)]; // Allocate space for objects in bytes (allocating objects would require them to be constructed)
        mFreeIndexes = new uint32_t[mSize];

        // Fill stack with all indexes to begin with
        for (uint32_t i = 0; i < mSize; ++i)  mFreeIndexes[i] = i;
    }

    ~PoolAllocator()
    {
        delete[] mFreeIndexes;
        delete[] mObjects;
    }

    // Prevent copy/move/assignment
    PoolAllocator(const PoolAllocator&) = delete;
    PoolAllocator(PoolAllocator&&) = delete;
    PoolAllocator& operator=(const PoolAllocator&) = delete;
    PoolAllocator& operator=(PoolAllocator&&) = delete;


    //-------------------------------------------------------------------------------------------------
    // Usage
    //-------------------------------------------------------------------------------------------------
public:
    // Get a free object from the pool and construct in place using forwarded arguments
    template<class... Args>
    T* Get(Args&&... args)
    {
        if (mNextFree == mSize)  throw std::bad_alloc();

        // Pop next free index from stack and get associated object to user
        uint32_t freeIndex = mFreeIndexes[mNextFree];
        mNextFree++;
        T* object = reinterpret_cast<T*>(&mObjects[freeIndex * sizeof(T)]);

        // Construct in-place (memory is already allocated), forwarding all the arguments 
        new (object) T(std::forward<Args>(args)...);

        return object;
    }


    // Release an object back into the pool
    void Release(T* object)
    {
        // Manually call the object's destructor (destructs but doesn't release the object's memory)
        object->~T();

        // Get index of released object and push onto free index stack
        uint32_t index = static_cast<uint32_t>(object - reinterpret_cast<T*>(&mObjects[0]));
        if (index > mSize)  throw std::out_of_range("Invalid release");

        --mNextFree;
        mFreeIndexes[mNextFree] = index;
    }


    // Free all objects
    void Reset()
    {
        for (uint32_t i = 0; i < mSize; ++i)  mFreeIndexes[i] = i;
        mNextFree = 0;
    }


    //-------------------------------------------------------------------------------------------------
    // Data
    //-------------------------------------------------------------------------------------------------
private:
    uint8_t* mObjects; // Allocate space for objects in bytes (allocating objects would require them to be constructed)
    uint32_t* mFreeIndexes;
    uint32_t  mSize;
    uint32_t  mNextFree;
};