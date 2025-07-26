EXE=syrup

$(EXE): src/main.c src/lexer.c src/syrup.c src/expr.c src/parser.c src/debug.c
	cc -Wall -o $@ $^

format:
	clang-format -style=file -i *.c

clean:
	rm -f $(EXE)