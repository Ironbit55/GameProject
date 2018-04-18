/*-
 * Copyright (c) 2013 Cosku Acay, http://www.coskuacay.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 * 
 * Modified: Edward Curran
 * 
 * I modified this to allow iterating through the pool
 * from start to finish using first() and next()
 * then we have a contigious chunk of memory 
 * that we can iterate through in order (with some gaps) which is cool.
 * 
 * The original author stored the free list within empty slots of the pool
 * which would be clever but its not helpful for iterating through the
 * list as we need to be able to check whether a given slot is free or not
 * solution was either completely change how the free list is implemented
 * (would require allocating a special chunk of memory for the free list)
 * 
 * or the solution i went with was to maintain the free list in address order
 * so can iterate from start to end
 * the downside of this is that deallocating objects requires inserting a free slot node
 * into the free list in the correct order which is no longer O(1)
 * the more free slots the higher the cost of deallocation
 * so fuller pools are better
 * 
 * the benefit is the pool uses less memory overall
 */

//#ifndef MEMORY_POOL_H
//#define MEMORY_POOL_H

#include <climits>
#include <cstddef>

template <typename T, size_t BlockSize = 4096>
class MemoryPoolArray
{
  public:
    /* Member types */
    typedef T               value_type;
    typedef T*              pointer;
    typedef T&              reference;
    typedef const T*        const_pointer;
    typedef const T&        const_reference;
    typedef size_t          size_type;
    typedef ptrdiff_t       difference_type;
    typedef std::false_type propagate_on_container_copy_assignment;
    typedef std::true_type  propagate_on_container_move_assignment;
    typedef std::true_type  propagate_on_container_swap;

    template <typename U> struct rebind {
      typedef MemoryPoolArray<U> other;
    };

	bool maintainFreeList = true;

    /* Member functions */
    MemoryPoolArray() noexcept;
    MemoryPoolArray(const MemoryPoolArray& MemoryPoolArray) noexcept;
    MemoryPoolArray(MemoryPoolArray&& MemoryPoolArray) noexcept;
    template <class U> MemoryPoolArray(const MemoryPoolArray<U>& MemoryPoolArray) noexcept;

    ~MemoryPoolArray() noexcept;

    MemoryPoolArray& operator=(const MemoryPoolArray& MemoryPoolArray) = delete;
    MemoryPoolArray& operator=(MemoryPoolArray&& MemoryPoolArray) noexcept;

    pointer address(reference x) const noexcept;
    const_pointer address(const_reference x) const noexcept;

    // Can only allocate one object at a time. n and hint are ignored
    pointer allocate(size_type n = 1, const_pointer hint = 0);
    void deallocate(pointer p, size_type n = 1);

	/*
	 * MODIFICATION
	 * Author: Edward Curran
	 * returns next available object after given object
	 * checks if p == freePointer
	 * updates freePointer with next free slot
	 * and continues untill p!=freePointer
	 * and returns pointer to avaiable object
	 * 
	 * free pointer is a reference to a pointer
	 * as we want to modify the pointer value
	 * 
	 */
	pointer next(pointer availableP, pointer& freePointer);

	// Can only allocate one object at a time. n and hint are ignored

	//return "object" at start of pool
	//not guranteed to be an allocated object
	
	pointer first();

	pointer firstFree();
	//Modification end

    size_type max_size() const noexcept;

    template <class U, class... Args> void construct(U* p, Args&&... args);
    template <class U> void destroy(U* p);

    template <class... Args> pointer newElement(Args&&... args);
	/**
     * \brief calls destructor on element and deallocates 
     * \param p 
     */
    void deleteElement(pointer p);

  private:
    struct Slot_ {
      value_type element;
	  int id;
      Slot_* next;
    };


    typedef char* data_pointer_;
    typedef Slot_ slot_type_;
    typedef Slot_* slot_pointer_;

    slot_pointer_ currentBlock_;
	slot_pointer_ firstSlot_;
    slot_pointer_ currentSlot_;
    slot_pointer_ lastSlot_;
    slot_pointer_ freeSlots_;
	slot_pointer_ freeSlotsTail_;

    size_type padPointer(data_pointer_ p, size_type align) const noexcept;
    void allocateBlock();

    static_assert(BlockSize >= 2 * sizeof(slot_type_), "BlockSize too small.");
};

//#include "MemoryPoolArray.tcc"

//#endif // MEMORY_POOL_H
