# bsort
Integer sorting algorithm using radix sort.
Code was written based on @rcgldr original code as discussed in [this thread](https://cboard.cprogramming.com/c-programming/178915-binary-radix-sort.html).

Compilation:

`g++ bsort.cc -o bsort.x -Wall -O3`

Usage:

`./bsort.x [-r record length in bytes] [-k key length in bytes] [-s key initial byte] [-o output filename] filename`

The input file should be a binary file. Code can be easily modified to handle ascii files.
