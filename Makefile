LDFLAGS := -lpthread
FILES := network options monitor manager director utils
OBJECTS := $(FILES:.o)
BINARY := coniuncti

define genrule
$1.o: $1.c $1.h
	$(CC) -c -o $$@ $$<
endef

.PHONY: all
all: $(BINARY)

$(BINARY): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^

.PHONY: network.h
$(foreach file,$(FILES),$(eval $(call genrule,$(file))))

.PHONY: clean
clean:
	rm -f *.o $(BINARY)
