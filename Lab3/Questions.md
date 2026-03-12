Questions (1 mark each) 
1. Create these 4 files:
  1. mathlib.h - Prototypes: int findFactorial(int n); and int multiplyBy100(int n);
  2. mathlib.c - Implement both functions
  3. printFactorial.c - Read from STDIN > call findFactorial() > print to STDOUT
  4. printMultiplyBy100.c - Read from STDIN > call multiplyBy100() > print to STDOUT
2. Create a Makefile that creates two binaries printFactorial and
   printMultiplyBy100 by compiling the c files. Make sure to use the correct
   flags so that the binaries can be used for debugging and profiling
3. Create a shell script that redirects a file input.txt with a single number to
   printFactorial process. The output is then piped to printMultiplyBy100. The
   output of which is finally redirected to output.txt
4. Debug printFactorial, put a breakpoint in findFactorial function check how
   the recursion (or loop) is being executed
5. Profile printFactorial, find the factorial of a small number and then of a
   large number. Do you see any difference in the profiling.
