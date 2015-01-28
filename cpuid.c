#include <stdio.h>
#include <inttypes.h>

#include "cpuid.h"

// 0
static void basic()
{
  uint32_t x[4] = {0};
  cpuid(0, x);
  uint32_t vendor[] = {x[1], x[3], x[2], 0};
  printf("%s\n", (char *)&vendor);
}

// 2  cache and tlb info
static void cache_info()
{
  uint8_t desc[16];
  int num_calls = 0;
  int i;
  do {
    cpuid(2, (uint32_t *)desc);
    for(i = 0; i < 16; i++) {
      if(!desc[i]) continue;
      cpuid_cache_info_t *info = &cpuid_cache_info_descriptor[desc[i]];
      printf("[%s]\t%s\n", info->type, info->desc);
    }
  } while(++num_calls < desc[0]);
}

// 4 deterministic cache parameters
static void cache_params()
{
  int idx;
  for(idx = 0; ; idx++) {
    uint32_t x[4];
    cpuid_index(4, idx, x);
    uint32_t type = CPUID_EXTR(x[0], 0, 4);
    if(type == 0) break;

    CPUID_REPF("type", x[0], 0, 4);
    CPUID_REPF("level", x[0], 5, 7);
    CPUID_REPF("self init", x[0], 8, 8);
    CPUID_REPF("full assoc", x[0], 9, 9);
    CPUID_REPFP1("sharing logical procs", x[0], 14, 25);
    CPUID_REPFP1("sharing package procs", x[0], 26, 31);
    CPUID_REPFP1("coherency line size", x[1], 0, 11);
    CPUID_REPFP1("phys line partitions", x[1], 12, 21);
    CPUID_REPFP1("ways", x[1], 22, 31);
    CPUID_REPFP1("sets", x[2], 0, 31);
    CPUID_REPF("wbinv", x[3], 0, 0);
    CPUID_REPF("inclusive", x[3], 1, 1);
    CPUID_REPF("complex", x[3], 2, 2);
  }
}

// 0x80000006
static void extended_l2()
{
  uint32_t x[4];
  cpuid(0x80000006, x);
  printf("cache line=%d B\n", x[2]&0xff);
  printf("L2 associativity=%d\n", (x[2]>>12)&0xf);
  printf("cache size=%d KiB\n", (x[2]>>16));
}

int main()
{
  basic();
  cache_info();
  cache_params();
  extended_l2();
  return 0;
}
