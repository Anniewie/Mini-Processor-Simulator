## Introduction

This project simulates the execution of a subset of a 32-bit five-stage CPU Pipeline
described in “Computer Organization and Design (COD)” by Patterson & Hennessy.

This is a core part of a mini processor simulator called MySPIM using the C language on a Unix/Linux platform.

MySPIM demonstrates some functions of the MIPS processor as well as the principle of separating the data-path 
from the control signals of the MIPS processor.

The MySPIM simulator reads in a file containing MIPS machine code (in a specified the format)
and simulates what MIPS does cycle-by-cycle (single-cycle data path).

The code is based on a class project from my Computer Logic and Organization class when I was in college. See Project Description.pdf for more information.

## Usage

Copy the project using `git clone  https://github.com/cwalk/Mini-Processor-Simulator`

Then cd into the project folder `cd Mini-Processor-Simulator/`

Navigate to the directory containing the source code `cd src/`

Run the following command to compile:

$ gcc -o [Executable File Name] spimcore.c project.c

Then to run the executable file:

$ ./[Executable File Name] [test file name]

The test files are in .asc format, but are viewable as .txt in the Test-Files folder.

##Example: 

$ gcc -o MySpim spimcore.c project.c

$ ./MySpim test.asc