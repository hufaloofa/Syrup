EXE = syrup
SRC = $(wildcard src/*.c util/*.c)
SRC := $(filter-out util/test.c src/web_main.c,$(SRC))

# Put object files in build/
OBJ = $(patsubst %.c,build/%.o,$(SRC))

# Default rule
$(EXE): $(OBJ)
	cc -Wall -o $@ $^

# Pattern rule: how to compile .c into build/*.o
build/%.o: %.c
	@mkdir -p $(dir $@)
	cc -Wall -c $< -o $@

WEB_SRC = $(wildcard src/*.c util/*.c)
WEB_SRC := $(filter-out util/test.c src/main.c, $(WEB_SRC))

web:
	@mkdir -p web/public
	emcc $(WEB_SRC) \
		-o web/public/syrup.js \
		-s EXPORTED_FUNCTIONS='["_run_source"]' \
		-s EXPORTED_RUNTIME_METHODS='["ccall"]' \
		-s ENVIRONMENT=web \
		-s NO_EXIT_RUNTIME=1 \
		-s ALLOW_MEMORY_GROWTH=1

.PHONY: web format clean

format:
	clang-format -style=file -i $(SRC)

clean:
	rm -rf $(EXE) build/
