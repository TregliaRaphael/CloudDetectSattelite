#ifndef MISC_H
#define MISC_H

extern struct {
  size_t size;
  void** adr;
} MemoryManager = {NULL, 0};

void* NewAdr(size_t size) {
  void* tmp = realloc(MemoryManager.adr, ++MemoryManager.size * sizeof(void*));
  if (tmp == NULL) {
    return NULL;
  }
  MemoryManager.adr = tmp;
  tmp = calloc(1, size); // tmp is now the new allocated memory
  MemoryManager.adr[MemoryManager.size - 1] = tmp;
  return tmp;
}

int AddAdr(void* adr) {
  void* tmp = realloc(MemoryManager.adr, ++MemoryManager.size);
  if (tmp == NULL) {
    return 0;
  }
  MemoryManager.adr[MemoryManager.size - 1] = adr;
  return 1;
}

void Delete(void* adr) {
  int i = 0;
  for (; i < MemoryManager.size; ++i) {
    if (MemoryManager.adr[i] == adr) break;
  }
  free(MemoryManager.adr[i]);
  MemoryManager.adr[i] = MemoryManager.adr[MemoryManager.size--];
}

#endif
