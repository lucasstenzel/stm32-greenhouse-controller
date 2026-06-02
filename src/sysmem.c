/*
 * sysmem.c
 *
 * Heap allocator hook for newlib. Provides _sbrk(), which malloc() calls
 * to grow the heap. Without this, any code path that pulls in malloc
 * (including printf via newlib's internal buffering) fails to link.
 *
 * Heap grows from the end of .bss upward; stack grows from _estack down.
 * _sbrk fails if they would collide.
 */

#include <errno.h>
#include <sys/types.h>

caddr_t _sbrk(int incr) {
    extern char end asm("end");           // symbol defined by linker script: end of .bss
    static char *heap_end;
    register char *stack_ptr asm("sp");

    if (heap_end == 0) {
        heap_end = &end;
    }

    char *prev_heap_end = heap_end;
    if (heap_end + incr > stack_ptr) {
        errno = ENOMEM;
        return (caddr_t)-1;
    }

    heap_end += incr;
    return (caddr_t)prev_heap_end;
}
