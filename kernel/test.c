#include <system.h>

#define ARR_SIZE 1029

int test() {
  unsigned char arr[ARR_SIZE] = {0};
  for (int i = 0; i < ARR_SIZE; i++) {
    arr[i] = (unsigned char)i;
  }
  unsigned char dst[ARR_SIZE] = {0};
  if (dst != memcpy(dst, arr, ARR_SIZE)) {
    return -1;
  }
  for (int i = 0; i < ARR_SIZE; i++) {
    if (dst[i] != arr[i])
      return -2;
  }
  return 0;
}
