import sys

# parse txt file
descriptors = [None]*256
lines = [l.strip() for l in sys.stdin]
for i in range(0, len(lines), 3):
  b = int(lines[i][:2], 16)
  t = lines[i+1]
  d = lines[i+2]
  descriptors[b] = (t, d)

# output C array
print '#include "cpuid.h"'
print ''
print 'cpuid_cache_info_t cpuid_cache_info_descriptor[256] = {'
for x in descriptors:
  if not x:
    print '  {{0}, {0}},'
    continue
  print '  {"%s", "%s"},' % x
print '};'
