#include <stddef.h>
#include <stdint.h>

void *_sbrk(ptrdiff_t incr)
{
  extern uint8_t _end;
  static uint8_t *heap_end;

  if (heap_end == NULL) {
    heap_end = &_end;
  }

  uint8_t *prev = heap_end;
  heap_end += incr;
  return prev;
}
