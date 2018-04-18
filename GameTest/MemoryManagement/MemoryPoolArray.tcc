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
 */



//#ifndef MEMORY_BLOCK_TCC
//#define MEMORY_BLOCK_TCC


#include "MemoryPoolArray.h"

template <typename T, size_t BlockSize>
inline typename MemoryPoolArray<T, BlockSize>::size_type
MemoryPoolArray<T, BlockSize>::padPointer(data_pointer_ p, size_type align)
const noexcept
{
  uintptr_t result = reinterpret_cast<uintptr_t>(p);
  return ((align - result) % align);
}



template <typename T, size_t BlockSize>
MemoryPoolArray<T, BlockSize>::MemoryPoolArray()
noexcept
{
  firstSlot_ = nullptr;
  currentBlock_ = nullptr;
  currentSlot_ = nullptr;
  lastSlot_ = nullptr;
  freeSlots_ = nullptr;
  freeSlotsTail_ = nullptr;

}



template <typename T, size_t BlockSize>
MemoryPoolArray<T, BlockSize>::MemoryPoolArray(const MemoryPoolArray& MemoryPoolArray)
noexcept :
MemoryPoolArray()
{}



template <typename T, size_t BlockSize>
MemoryPoolArray<T, BlockSize>::MemoryPoolArray(MemoryPoolArray&& MemoryPoolArray)
noexcept
{
  currentBlock_ = MemoryPoolArray.currentBlock_;
  MemoryPoolArray.currentBlock_ = nullptr;
  firstSlot_ = MemoryPoolArray.firstSlot_;
  MemoryPoolArray.firstSlot_ = nullptr;
  currentSlot_ = MemoryPoolArray.currentSlot_;
  lastSlot_ = MemoryPoolArray.lastSlot;
  freeSlots_ = MemoryPoolArray.freeSlots;
  
}


template <typename T, size_t BlockSize>
template<class U>
MemoryPoolArray<T, BlockSize>::MemoryPoolArray(const MemoryPoolArray<U>& MemoryPoolArray)
noexcept :
MemoryPoolArray()
{}



template <typename T, size_t BlockSize>
MemoryPoolArray<T, BlockSize>&
MemoryPoolArray<T, BlockSize>::operator=(MemoryPoolArray&& MemoryPoolArray)
noexcept
{
  if (this != &MemoryPoolArray)
  {
    std::swap(currentBlock_, MemoryPoolArray.currentBlock_);
	firstSlot_ = MemoryPoolArray.firstSlot_;
    currentSlot_ = MemoryPoolArray.currentSlot_;
    lastSlot_ = MemoryPoolArray.lastSlot_;
    freeSlots_ = MemoryPoolArray.freeSlots;
  }
  return *this;
}



template <typename T, size_t BlockSize>
MemoryPoolArray<T, BlockSize>::~MemoryPoolArray()
noexcept
{
  slot_pointer_ curr = currentBlock_;
  while (curr != nullptr) {
    slot_pointer_ prev = curr->next;
    operator delete(reinterpret_cast<void*>(curr));
    curr = prev;
  }
}



template <typename T, size_t BlockSize>
inline typename MemoryPoolArray<T, BlockSize>::pointer
MemoryPoolArray<T, BlockSize>::address(reference x)
const noexcept
{
  return &x;
}



template <typename T, size_t BlockSize>
inline typename MemoryPoolArray<T, BlockSize>::const_pointer
MemoryPoolArray<T, BlockSize>::address(const_reference x)
const noexcept
{
  return &x;
}



template <typename T, size_t BlockSize>
void
MemoryPoolArray<T, BlockSize>::allocateBlock()
{
  // Allocate space for the new block and store a pointer to the previous one
  data_pointer_ newBlock = reinterpret_cast<data_pointer_>
                           (operator new(BlockSize));
  reinterpret_cast<slot_pointer_>(newBlock)->next = currentBlock_;
  currentBlock_ = reinterpret_cast<slot_pointer_>(newBlock);
  // Pad block body to staisfy the alignment requirements for elements
  data_pointer_ body = newBlock + sizeof(slot_pointer_);
  size_type bodyPadding = padPointer(body, alignof(slot_type_));
  currentSlot_ = reinterpret_cast<slot_pointer_>(body + bodyPadding);
  if (firstSlot_ == nullptr){
	  firstSlot_ = currentSlot_;
  }
  lastSlot_ = reinterpret_cast<slot_pointer_>
              (newBlock + BlockSize - sizeof(slot_type_) + 1);
}



template <typename T, size_t BlockSize>
inline typename MemoryPoolArray<T, BlockSize>::pointer
MemoryPoolArray<T, BlockSize>::allocate(size_type n, const_pointer hint)
{
  if (freeSlots_ != nullptr) {
    pointer result = reinterpret_cast<pointer>(freeSlots_);
    freeSlots_ = freeSlots_->next;
    return result;
  }
  else {
    if (currentSlot_ >= lastSlot_)
      allocateBlock();
    return reinterpret_cast<pointer>(currentSlot_++);
  }
}



template <typename T, size_t BlockSize>
inline void
MemoryPoolArray<T, BlockSize>::deallocate(pointer p, size_type n)
{
	if (p != nullptr) {
		//OG
		/*reinterpret_cast<slot_pointer_>(p)->next = freeSlots_;
		freeSlots_ = reinterpret_cast<slot_pointer_>(p);*/

		//MODIFICATION
		//AUTHOR: Edward curran
		//alright so, I'm modifying this so that the freeSlots_
		//linked list is in address order... this is so we can iterate
		//through pool in address order
		//by updating current freeSlot with next as we iterate and
		//to check whether next slot is free
		//because this memory pool puts a slot object
		//in free slots this makes everything difficult


		if (freeSlots_ == nullptr) {
			freeSlots_ = reinterpret_cast<slot_pointer_>(p);
			//freeSlots_->next = freeSlotsTail_;
			freeSlotsTail_ = freeSlots_;

			return;
		}

		//small optimisation (hopefully) keep trak of free slots tail
		//and check if this slot should be sorted after it
		//quite a common case when number of free slots small
		//compared to size of pool

		if (reinterpret_cast<slot_pointer_>(p) > freeSlotsTail_) {

			slot_pointer_ nextSlot = reinterpret_cast<slot_pointer_>(p);
			nextSlot->next = nullptr;
			freeSlotsTail_->next = reinterpret_cast<slot_pointer_>(p);
			freeSlotsTail_ = reinterpret_cast<slot_pointer_>(p);

			return;
		}

		if (reinterpret_cast<slot_pointer_>(p) < freeSlots_) {
			reinterpret_cast<slot_pointer_>(p)->next = freeSlots_;
			freeSlots_ = reinterpret_cast<slot_pointer_>(p);

			return;
		}

		//somewhere in between

		//iterate through free slots list until we find where this new slot belongs
		for (slot_pointer_ slot = freeSlots_; slot != nullptr; slot = freeSlots_->next) {
			if (reinterpret_cast<slot_pointer_>(p) > slot) {
				//found where this slot belong in address order
				//so insert it
				reinterpret_cast<slot_pointer_>(p)->next = slot->next;
				slot->next = reinterpret_cast<slot_pointer_>(p);

				/*reinterpret_cast<slot_pointer_>(p)->next = slot;
				freeSlots_ = reinterpret_cast<slot_pointer_>(p);*/

				return;
			}
		}
	}

	
}

/*
 * Modification start
 * Author: Edward Curran
 */
template <typename T, size_t BlockSize>
inline typename MemoryPoolArray<T, BlockSize>::pointer
MemoryPoolArray<T, BlockSize>::next(pointer availableP, pointer& freeSlot)
{

	slot_pointer_ freeSlotPtr = reinterpret_cast<slot_pointer_>(freeSlot);
	slot_pointer_ availableSlot = reinterpret_cast<slot_pointer_>(availableP);

	if(availableSlot == nullptr){
		availableSlot = firstSlot_;
	}else{
		++availableSlot;
	}

	//if (availableSlot == currentSlot_) {
	//	return nullptr;
	//}

	//if (freeSlotPtr == nullptr) {
	//	freeSlotPtr = freeSlots_;
	//}

	//freeSlot = reinterpret_cast<slot_pointer_>(freeSlot);
	//availableP = reinterpret_cast<slot_pointer_>(availableP);

	while(availableSlot < currentSlot_){
		if(availableSlot != freeSlotPtr){
			return reinterpret_cast<pointer>(availableSlot);
		}

		++availableSlot;
		freeSlotPtr = freeSlotPtr->next;
		freeSlot = reinterpret_cast<pointer>(freeSlotPtr);
	}

	return nullptr;
	//loop untill find avaiable object / pointer
	//this only works as free slots are in address order
	while(availableSlot == freeSlotPtr){
		++availableSlot;
		//lol
		freeSlotPtr = freeSlotPtr->next;
		freeSlot = reinterpret_cast<pointer>(freeSlotPtr);
		
		if(availableSlot == currentSlot_){
			return nullptr;
		}
	}

	//availableP is not on free list, so we can return it
	return reinterpret_cast<pointer>(availableSlot);
	
}

template <typename T, size_t BlockSize>
inline typename MemoryPoolArray<T, BlockSize>::pointer
MemoryPoolArray<T, BlockSize>::first() {
	pointer p = firstFree();
	return this->next(nullptr, p);
}

template <typename T, size_t BlockSize>
inline typename MemoryPoolArray<T, BlockSize>::pointer
MemoryPoolArray<T, BlockSize>::firstFree() {
	return reinterpret_cast<pointer>(freeSlots_);
}
/*
 * modification end
 */


template <typename T, size_t BlockSize>
inline typename MemoryPoolArray<T, BlockSize>::size_type
MemoryPoolArray<T, BlockSize>::max_size()
const noexcept
{
  size_type maxBlocks = -1 / BlockSize;
  return (BlockSize - sizeof(data_pointer_)) / sizeof(slot_type_) * maxBlocks;
}



template <typename T, size_t BlockSize>
template <class U, class... Args>
inline void
MemoryPoolArray<T, BlockSize>::construct(U* p, Args&&... args)
{
  new (p) U (std::forward<Args>(args)...);
}



template <typename T, size_t BlockSize>
template <class U>
inline void
MemoryPoolArray<T, BlockSize>::destroy(U* p)
{
  p->~U();
}



template <typename T, size_t BlockSize>
template <class... Args>
inline typename MemoryPoolArray<T, BlockSize>::pointer
MemoryPoolArray<T, BlockSize>::newElement(Args&&... args)
{
  pointer result = allocate();
  construct<value_type>(result, std::forward<Args>(args)...);
  return result;
}



template <typename T, size_t BlockSize>
inline void
MemoryPoolArray<T, BlockSize>::deleteElement(pointer p)
{
  if (p != nullptr) {
    p->~value_type();
    deallocate(p);
  }
}



//#endif // MEMORY_BLOCK_TCC
