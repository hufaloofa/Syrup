EXE=syrup

$(EXE): src/main.c
	cc -Wall -o $@ $^

format:
	clang-format -style=file -i *.c

clean:
	rm -f $(EXE)