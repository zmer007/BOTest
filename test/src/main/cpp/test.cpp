#include <cstdio>
#include <unistd.h>
#include <cmath>
#include <cstdlib>
#include <cstring>

#define arm_plain_ins01 __asm__ __volatile__( \
  "mov r1, r0\n"                              \
);
#define arm_plain_ins02 __asm__ __volatile__( \
  "add r1, r0, r3\n"                          \
);
#define arm_plain_ins03 __asm__ __volatile__( \
  "sub r4, r0\n"                              \
);
#define arm_sp_ins01 __asm__ __volatile__( \
  "add sp, sp, #0x100"                     \
);

int main(int argc, char *argv[]) {
  if (argc != 1) {
    arm_plain_ins01
    arm_plain_ins03
    arm_sp_ins01
    arm_plain_ins03
    arm_plain_ins02
  }
  printf("argc: %d\n", argc);
  printf("Hello world!\n");
  return 0;
}