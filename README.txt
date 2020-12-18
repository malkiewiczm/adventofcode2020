Advent of Code 2020

https://adventofcode.com/2020

I exclusivly used C++ for this year.

The programs read in input through a file called "in.txt" by default
but any file can be used if you pass a different file name through the
command line args.

You will probably have to read the source code for the given day you
are looking for because sometimes I put the input directly in the
source code if it made more sense to me. I apologize for this
inconsistency.

If you define the compiler constant "PARTONE" then the program will
give you the part one solution, otherwise the default is part
two. #define PARTONE is left commented out for each day.

The output files are always .exe, even on linux. I did this to allow
"make clean" to remove the executables easily.

Example: compile and run day 25 with

$ make 25.exe
$ ./25.exe
