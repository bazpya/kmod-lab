obj-m += bazmodule.o

KDIR  := /lib/modules/$(shell uname -r)/build
CWD   := $(shell pwd)
BUILD := $(CWD)/build

all:
	mkdir -p $(BUILD)
	ln -sf $(CWD)/bazmodule.c $(CWD)/Makefile $(BUILD)/
	$(MAKE) -C $(KDIR) M=$(BUILD) modules

clean:
	rm -rf $(BUILD)
