#include <stdio.h>
#include <stdlib.h>
#include "./oop/umbrella.h"
#include "./oop/protocols/MemoryDelegate.h"

int main(int argc, char const *argv[])
{
	CPU* cpu = alloc_init(CPU_Class_Descriptor);
	CPU_Fetch_Execute_Cycle(cpu);
}
