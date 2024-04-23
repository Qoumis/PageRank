# PageRank Algorithm
This is a simple version of PageRank algorithm that was implemented and parallelized using C pthreads, as a part of academic course CS - 342

## How to compile:
● make -> generates pagerank binary file

● make clean -> removes binary/object files & potential csv files
## How to run:
./pagerank <input_file> <output_file.csv>
#
Optional execution flags:

● -t #n , to specify the number of threads that will be created (n
can be a value from 1 to 4)

● -ginfo, generates a file containing details regarding the incoming
and outgoing edges of the input graph
