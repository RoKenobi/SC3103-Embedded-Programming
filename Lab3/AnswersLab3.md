SC3103 EMBEDDED PROGRAMMING - PRACTICAL EXERCISE #3 SOLUTION GUIDE
Based on Lab Manual: Familiarization with Linux Environment and GNU Toolchain
===============================================================================

INTRODUCTION
------------
This document provides a complete step-by-step solution to the 5 assigned 
questions for Exercise #3. It aligns with the learning objectives of the 
SC3103 Lab Manual (Sections 1, 8, 9, and 10). 

You can copy this entire text into a file named "README.txt" or paste it 
into a Word Document. All commands and code snippets are indented by 4 spaces 
for clarity.

PHASE 1: ENVIRONMENT SETUP (Manual Section 1)
===============================================================================

1.1 Open Terminal
    - On Ubuntu: Press Ctrl+Alt+T or search for "Terminal".
    - On RPi: Use SSH (ssh pi@192.168.3.1).

1.2 Create Working Directory
    Run the following commands to set up your workspace:

    mkdir ce3103_ex3
    cd ce3103_ex3
    mkdir work
    cd work

1.3 Create Input File
    We need an input file for the shell script pipeline later.

    echo "5" > input.txt


PHASE 2: C PROGRAM FILES (Manual Section 6 & 8)
===============================================================================

Question 1 requires 4 files: mathlib.h, mathlib.c, printFactorial.c, 
and printMultiplyBy100.c. Use 'nano' to create them.

2.1 File: mathlib.h
    Command: nano mathlib.h
    Content (paste this):

    #ifndef MATHLIB_H
    #define MATHLIB_H

    // Function Prototypes
    int findFactorial(int n);
    int multiplyBy100(int n);

    #endif

2.2 File: mathlib.c
    Command: nano mathlib.c
    Content (paste this):

    #include "mathlib.h"

    // Recursive implementation for debugging practice
    int findFactorial(int n) {
        if (n <= 1) {
            return 1;
        } else {
            return n * findFactorial(n - 1);
        }
    }

    int multiplyBy100(int n) {
        return n * 100;
    }

2.3 File: printFactorial.c
    Command: nano printFactorial.c
    Content (paste this):

    #include <stdio.h>
    #include "mathlib.h"

    int main() {
        int n;
        // Read from STDIN
        if (scanf("%d", &n) == 1) {
            int result = findFactorial(n);
            // Print to STDOUT
            printf("%d\n", result);
        } else {
            printf("Invalid input\n");
        }
        return 0;
    }

2.4 File: printMultiplyBy100.c
    Command: nano printMultiplyBy100.c
    Content (paste this):

    #include <stdio.h>
    #include "mathlib.h"

    int main() {
        int n;
        // Read from STDIN
        if (scanf("%d", &n) == 1) {
            int result = multiplyBy100(n);
            // Print to STDOUT
            printf("%d\n", result);
        } else {
            printf("Invalid input\n");
        }
        return 0;
    }


PHASE 3: MAKEFILE WITH DEBUG & PROFILE FLAGS (Manual Section 8, 9, 10)
===============================================================================

Question 2 requires a Makefile that creates two binaries and supports 
debugging (-g) and profiling (-pg).

3.1 File: Makefile
    Command: nano Makefile
    Content (paste this):

    # Compiler
    CC = gcc

    # Flags
    CFLAGS = -Wall
    CFLAGS_DEBUG = $(CFLAGS) -g
    CFLAGS_PROFILE = $(CFLAGS) -pg

    # Object files
    OBJS_LIB = mathlib.o
    OBJS_FACT = printFactorial.o
    OBJS_MULT = printMultiplyBy100.o

    # Default Target
    all: printFactorial printMultiplyBy100

    # Standard Build
    printFactorial: $(OBJS_FACT) $(OBJS_LIB)
        $(CC) $(CFLAGS) -o $@ $^

    printMultiplyBy100: $(OBJS_MULT) $(OBJS_LIB)
        $(CC) $(CFLAGS) -o $@ $^

    # Compile Object Files
    %.o: %.c mathlib.h
        $(CC) $(CFLAGS) -c $< -o $@

    # Debug Build (Task 2 Requirement)
    debug: CFLAGS = $(CFLAGS_DEBUG)
    debug: clean all
        @echo "Debug binaries built with -g"

    # Profile Build (Task 2 Requirement)
    profile: CFLAGS = $(CFLAGS_PROFILE)
    profile: LDFLAGS = -pg
    profile: clean all
        @echo "Profile binaries built with -pg"

    # Clean
    clean:
        rm -f *.o printFactorial printMultiplyBy100 gmon.out

    IMPORTANT: Ensure lines under targets (like $(CC)...) are indented 
    with a TAB key, not spaces, when you paste this into nano.

3.2 Build the Programs
    Run the following command to compile:

    make

    Verify files exist:
    ls -l


PHASE 4: SHELL SCRIPT FOR PIPELINE (Manual Section 4 & 5)
===============================================================================

Question 3 requires a shell script that redirects input.txt to 
printFactorial, pipes to printMultiplyBy100, and redirects to output.txt.

4.1 File: run_pipeline.sh
    Command: nano run_pipeline.sh
    Content (paste this):

    #!/bin/bash
    # Ensure binaries exist
    if [ ! -f ./printFactorial ] || [ ! -f ./printMultiplyBy100 ]; then
        echo "Binaries not found. Running make..."
        make
    fi

    # The Pipeline:
    # 1. < input.txt       : Redirect input.txt to stdin of printFactorial
    # 2. |                 : Pipe stdout of printFactorial to stdin of printMultiplyBy100
    # 3. > output.txt      : Redirect stdout of printMultiplyBy100 to output.txt
    ./printFactorial < input.txt | ./printMultiplyBy100 > output.txt

    echo "Pipeline complete. Check output.txt:"
    cat output.txt

4.2 Set Permissions & Run
    Make the script executable (Manual Section 5.5):

    chmod ugo+rwx run_pipeline.sh

    Run the script:

    ./run_pipeline.sh

    Expected Output:
    If input.txt contains 5:
    Factorial(5) = 120
    Multiply(120) = 12000
    output.txt should contain 12000.


PHASE 5: DEBUGGING WITH GDB (Manual Section 9)
===============================================================================

Question 4 requires debugging printFactorial and checking recursion.

5.1 Build Debug Version
    Use the 'debug' target in the Makefile (adds -g flag):

    make debug

5.2 Start GDB
    Load the executable into the debugger:

    gdb ./printFactorial

5.3 GDB Command Sequence
    Inside the (gdb) prompt, type the following commands:

    1. Set Breakpoint at findFactorial
       break findFactorial

    2. Run with Input
       run < input.txt

    3. Step Through Recursion
       step
       (Keep typing 'step' or 's' to move through lines)
       
       Check variable 'n' at each step:
       print n

       Observation: You will see 'n' decrease (5, 4, 3...) until it hits 1, 
       then the returns unwind.

    4. Continue Execution
       continue

    5. Quit GDB
       quit


PHASE 6: PROFILING WITH GPROF (Manual Section 10)
===============================================================================

Question 5 requires profiling with small and large numbers to see differences.

6.1 Build Profile Version
    Use the 'profile' target in the Makefile (adds -pg flag):

    make profile

6.2 Test 1: Small Number
    Set input to a small number (e.g., 5):

    echo "5" > input.txt
    ./printFactorial < input.txt

    Generate Report:
    gprof ./printFactorial gmon.out > analysis_small.txt

6.3 Test 2: Large Number
    Set input to a larger number (e.g., 15). 
    WARNING: Do not use 100 (integer overflow/hang).

    echo "15" > input.txt
    ./printFactorial < input.txt

    Generate Report:
    gprof ./printFactorial gmon.out > analysis_large.txt

6.4 Analyze Differences
    View the reports:

    less analysis_small.txt
    (Press 'q' to quit)
    less analysis_large.txt

    What to look for (Manual Section 10.3):
    1. Flat Profile: Check the '% time' column.
    2. Call Count: Check the 'calls' column for findFactorial.
       - Small (5): Called ~5-6 times.
       - Large (15): Called ~15-16 times.
    3. Time Difference: The 'self' seconds for findFactorial will be higher 
       in the large number test because recursion depth is deeper.


FINAL CHECKLIST
===============================================================================

Before submitting or finishing, ensure you have completed:

[ ] Created 4 C files (mathlib.h, mathlib.c, printFactorial.c, printMultiplyBy100.c)

[ ] Created Makefile with 'all', 'debug', 'profile', and 'clean' targets

[ ] Created run_pipeline.sh with correct permissions (chmod +x)

[ ] Verified pipeline output (input.txt -> output.txt)

[ ] Successfully stepped through recursion in GDB

[ ] Generated gmon.out and analysis files for small and large inputs


END OF DOCUMENT
===============================================================================
