

CC=clang
CFLAGS=-g -Wall
OBJS=shared_memory.o
EXE=writeshmem.elf readshmem.elf destroy_shmem.elf


all: $(EXE)


%.elf: %.o $(OBJS)
	$(CC) $(CFLAGS) $< $(OBJS) -o $@ -lpthread

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ 

clean:
	rm -rf *.dSYM *.o $(EXE)