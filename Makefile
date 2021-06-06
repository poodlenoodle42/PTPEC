TARGET_EXEC ?= client

BUILD_DIR ?= ./build
SRC_DIRS ?= ./

SRCS := $(shell find $(SRC_DIRS)  -name *.c)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS :=	./include
INC_FLAGS := $(addprefix -I,$(INC_DIRS))
LDFLAGS := -lncurses -lssl -lcrypto
CPPFLAGS ?= $(INC_FLAGS) -MMD -MP -std=gnu11 -g
CC = gcc
$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# c source
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p