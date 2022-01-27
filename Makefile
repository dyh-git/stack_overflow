CC ?= gcc
AR ?= ar

USER_SRCS := stack_main.c
USER_OBJS := $(patsubst %.c,%.o,$(wildcard $(USER_SRCS)))
USER_D_SRCS := stack_overflow.c
USER_D_OBJS := $(patsubst %.c,%.o,$(wildcard $(USER_D_SRCS)))

OUTPUT_DIR := output

LD_FLAGS := -lstack

D_LIB_NAME := libstack.so

USER_PROGRAM_NAME := stack_main

CFLAGS = -Wall -g
CFLAGS += -fno-stack-protector

.PHONY: all clean

$(OUTPUT_DIR)/libs/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ -c $<

$(OUTPUT_DIR)/libs/$(D_LIB_NAME): $(USER_D_OBJS)
	mkdir -p $(dir $@)
	$(CC) -fPIC -shared -o $@ $^
	echo "Build $@ is finished"

$(USER_PROGRAM_NAME): $(USER_SRCS)
	$(CC) $^ $(CFLAGS) -L./$(OUTPUT_DIR)/libs -Wl,-Bdynamic $(LD_FLAGS) -o $@

all: $(OUTPUT_DIR)/libs/$(D_LIB_NAME) \
	$(USER_PROGRAM_NAME)

clean:
	rm *.o -rf
	rm *.a -rf
	rm *.so -rf
	rm -rf $(USER_PROGRAM_NAME)
	rm output -rf