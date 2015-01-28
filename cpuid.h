#ifndef CPUID_H_GUARD
#define CPUID_H_GUARD

#include <stdio.h>
#include <inttypes.h>

#define CPUID_ONES(N)       (~(~UINT64_C(0) << (N)))
#define CPUID_EXTR(X,L,H)   (((X)>>(L)) & CPUID_ONES((H)-(L)+1))
#define CPUID_REP(N,X) { printf(N "=%" PRIu64 "\n", (X)); }
#define CPUID_REPF(N,X,L,H) CPUID_REP(N, CPUID_EXTR(X,L,H))
#define CPUID_REPFP1(N,X,L,H) CPUID_REP(N, 1+CPUID_EXTR(X,L,H))

static inline void cpuid_index(int type, int index, uint32_t *x)
{
  printf("cpuid %02x\t", type);
  asm volatile ( "cpuid \n"
      : "=a" (x[0]), "=b" (x[1]), "=c" (x[2]), "=d" (x[3])
      : "0" (type), "2" (index));
  printf("%08x %08x %08x %08x\n", x[0], x[1], x[2], x[3]);
}

static inline void cpuid(int type, uint32_t *x)
{
  cpuid_index(type, 0, x);
}

typedef struct {
  char type[16];
  char desc[256];
} cpuid_cache_info_t;

extern cpuid_cache_info_t cpuid_cache_info_descriptor[256];

#endif // CPUID_H_GUARD
