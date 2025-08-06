EXE = syrup
SRC = $(wildcard src/*.c util/*.c)
SRC := $(filter-out util/test.c,$(SRC))

# Put object files in build/
OBJ = $(patsubst %.c,build/%.o,$(SRC))

# Default rule
$(EXE): $(OBJ)
	cc -Wall -o $@ $^

# Pattern rule: how to compile .c into build/*.o
build/%.o: %.c
	@mkdir -p $(dir $@)
	cc -Wall -c $< -o $@

format:
	clang-format -style=file -i $(SRC)

clean:
	rm -rf $(EXE) build/
