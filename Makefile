LDFLAGS := -lpthread
FILES := network options monitor manager director utils

define genrule
$1.o: $1.c $1.h
	$(CC) -c -o $$@ $$<
endef

.PHONY: all
all: a.out

network: network.o options.o monitor.o manager.o director.o utils.o
	$(CC) $(LDFLAGS) -o $@ $^

.PHONY: network.h
$(foreach file,$(FILES),$(eval $(call genrule,$(file))))

.PHONY: clean
clean:
	rm -f *.o network
