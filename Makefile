CC := gcc
CFLAGS := -Wall -Werror -Wextra -Iinclude -MMD -MP

SRC_DIR := src
OBJ_DIR := build

TARGET := monitor

SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES := $(SRC_FILES:src/%.c=$(OBJ_DIR)/%.o)
DEP_FILES := $(OBJ_FILES:.o=.d)

all : $(TARGET)

$(TARGET) : $(OBJ_FILES)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ_FILES) : $(OBJ_DIR)/%.o : $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR) :
	mkdir -p $@

.PHONY : all clean

-include $(DEP_FILES) 

clean :
	rm -f $(OBJ_FILES) $(DEP_FILES)
	rm -rf $(OBJ_DIR)