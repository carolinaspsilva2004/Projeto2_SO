CC = gcc
CFLAGS = -Wall

SUFFIX = $(shell getconf LONG_BIT)

CHEF         = semSharedMemChef
WAITER       = semSharedMemWaiter
GROUP        = semSharedMemGroup
RECEPTIONIST = semSharedMemReceptionist
MAIN         = probSemSharedMemRestaurant

OBJS = sharedMemory.o semaphore.o logging.o

.PHONY: all ct ct_ch all_bin \
	clean cleanall

all:		group         waiter      chef       receptionist     main clean
gr:		    group         waiter_bin  chef_bin   receptionist_bin main clean
wt:		    group_bin     waiter      chef_bin   receptionist_bin main clean
ch:		    group_bin     waiter_bin  chef       receptionist_bin main clean
rt:		    group_bin     waiter_bin  chef_bin   receptionist     main clean
all_bin:	group_bin     waiter_bin  chef_bin   receptionist_bin main clean

chef:	$(CHEF).o $(OBJS)
	$(CC) -o ../run/$@ $^ -lm

waiter:		$(WAITER).o $(OBJS)
	$(CC) -o ../run/$@ $^

group:	$(GROUP).o $(OBJS)
	$(CC) -o ../run/$@ $^ -lm

receptionist:	$(RECEPTIONIST).o $(OBJS)
	$(CC) -o ../run/$@ $^ -lm

main:		$(MAIN).o $(OBJS)
	$(CC) -o ../run/$(MAIN) $^ -lm

chef_bin:
	cp ../run/chef_bin_$(SUFFIX) ../run/chef

waiter_bin:
	cp ../run/waiter_bin_$(SUFFIX) ../run/waiter

group_bin:
	cp ../run/group_bin_$(SUFFIX) ../run/group

receptionist_bin:
	cp ../run/receptionist_bin_$(SUFFIX) ../run/receptionist

clean:
	rm -f *.o

cleanall:	clean
	rm -f ../run/$(MAIN) ../run/chef ../run/waiter ../run/group ../run/receptionist

