CC=gcc
CFLAGS=-Wall

prog=cpuid

all: $(prog)

$(prog): cpuid_cache_info_descriptor.o
