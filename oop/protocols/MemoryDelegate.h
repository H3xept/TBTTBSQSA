#ifndef __MemoryDelegate__
#define __MemoryDelegate__

#include "../constants/var_word_size.h"

struct MemoryDelegate {
	uword_t (*MemoryDelegate_Word_At_Address)(struct MemoryDelegate * self, uword_t addr);
	void (*MemoryDelegate_Set_Word_At_Address)(struct MemoryDelegate * self, uword_t addr, uword_t word);
	void (*MemoryDelegate_Clear_Memory)(struct MemoryDelegate * self);
	void (*MemoryDelegate_Load_Memory_From_Ptr)(struct MemoryDelegate * self, void* ptr, size_t size);
};

#endif
