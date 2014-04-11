PACKAGE = compiler

CC = clang++

CCFLAGS = -Wall -Wextra -pedantic -std=c++11 -MMD -Iinclude -O0
	

CPP_FILES = $(wildcard src/*.cpp)
OBJ_FILES = $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))

COMPILE_START_TIME := $(shell date +\(%j\*24\*60\*60+%H\*60\*60+%M\*60+%S\))
COMPILE_CURRENT_TIME = `date  +\(%j\*24\*60\*60+%H\*60\*60+%M\*60+%S\)`

define print_timer
@CT=$(1) && \
printf "Build complete (%.2d:%.2d)\n" \
`echo "($$CT - $(2))/60" | bc` \
`echo "($$CT - $(2))%60" | bc`
endef

all: $(PACKAGE)
	$(call print_timer,$(COMPILE_CURRENT_TIME),$(COMPILE_START_TIME))

$(PACKAGE): $(OBJ_FILES)
	# Linking...
	$(CC) -o $@ $^

obj/%.o: src/%.cpp
	# Compiling $<...
	$(CC) $(CCFLAGS) -c -o $@ $<

run: $(PACKAGE)
	$(call print_timer,$(COMPILE_CURRENT_TIME),$(COMPILE_START_TIME))
	# Running...
	./$(PACKAGE)

clean:
	# Cleaning files...
	rm -f obj/* $(PACKAGE)


-include $(OBJ_FILES:.o=.d)