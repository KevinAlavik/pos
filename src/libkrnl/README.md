# libkrnl-essentials
just a kernel library.

## Features
### x86 specific stuff
essentials,cpuid,floating point initialization and an RTC driver all included
### utilities
just some utils :P
### types
vectors,queues and bitmaps
### io
simple portio
*the "b" in outb stands for bit not byte* 
## Requirements
vectors,queues and the string split util, require a heap
that must have `malloc` and `realloc`.</br>
The heap must be at include path `utils/heapdef.h`.<br>
to signal that your kernel has the required heap, please define `HEAP_ACCESSABLE`</br>
to signal that your kernel enables the FPU on boot, please define `SUPPORT_FLOAT`
