PACKAGE = compiler

CCFLAGS = -Wall -Wextra -pedantic -std=c++11 -MMD -Iinclude -I/home/wilco/l/boost_1_55_0 -O0 -g3

CPP_FILES = $(wildcard src/*.cpp)
OBJ_FILES = $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))

all: $(PACKAGE)

$(PACKAGE): $(OBJ_FILES)
	clang++ -o $@ $^

obj/%.o: src/%.cpp
	clang++ $(CCFLAGS) -c -o $@ $<

run: $(PACKAGE)
	valgrind ./$(PACKAGE)

check:
	cppcheck --enable=style -q -Iinclude .

clean:
	rm -f obj/* $(PACKAGE)


-include $(OBJ_FILES:.o=.d)