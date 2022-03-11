build: sudoku.c 
	gcc -o sudoku -I. -Wall -Wextra sudoku.c 
run: build
	./sudoku
.PHONY: clean
clean:
	rm -f *.o sudoku