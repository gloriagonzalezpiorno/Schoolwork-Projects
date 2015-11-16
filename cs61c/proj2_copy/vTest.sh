 # ./run-valgrind input/jumps.s 
 # ./run-valgrind input/combined.s
 # ./run-valgrind input/comments.s
 # ./run-valgrind input/imm.s
 # ./run-valgrind input/labels.s 
 # ./run-valgrind input/pseudo.s 
 # ./run-valgrind input/rtypes.s	
 # ./run-valgrind input/simple.s
 #  ./run-valgrind input/p1_errors.s 
 # ./run-valgrind input/p2_errors.s

# mars a dump .text HexText mars_output/jump.txt  input/jumps.s
# mars a dump .text HexText mars_output/combined.txt  input/combined.s
# mars a dump .text HexText mars_output/comments.txt  input/comments.s
# mars a dump .text HexText mars_output/imm.txt  input/imm.s
# mars a dump .text HexText mars_output/labels.txt  input/labels.s
# mars a dump .text HexText mars_output/pseudo.txt  input/pseudo.s
# mars a dump .text HexText mars_output/rtypes.txt  input/rtypes.s
# mars a dump .text HexText mars_output/simple.txt  input/simple.s
# mars a dump .text HexText mars_output/p1_errors.txt  input/p1_errors.s
# mars a dump .text HexText mars_output/p2_errors.txt  input/p2_errors.s


# ./assembler input/simple.s out/test/simple.int out/test/simple.out
make assembler
./run-valgrind ./assembler -p1 input/simple.s out/simple.int
./run-valgrind ./assembler -p1 input/combined.s out/combined.int
./run-valgrind ./assembler -p1 input/comments.s out/comments.int
./run-valgrind ./assembler -p1 input/imm.s out/imm.int
./run-valgrind ./assembler -p1 input/labels.s out/labels.int
./run-valgrind ./assembler -p1 input/pseudo.s out/pseudo.int
./run-valgrind ./assembler -p1 input/rtypes.s out/rtypes.int
./run-valgrind ./assembler -p1 input/simple.s out/simple.int

./run-valgrind ./assembler -p2 input/simple.s out/simple.int
./run-valgrind ./assembler -p2 input/combined.s out/combined.int
./run-valgrind ./assembler -p2 input/comments.s out/comments.int
./run-valgrind ./assembler -p2 input/imm.s out/imm.int
./run-valgrind ./assembler -p2 input/labels.s out/labels.int
./run-valgrind ./assembler -p2 input/pseudo.s out/pseudo.int
./run-valgrind ./assembler -p2 input/rtypes.s out/rtypes.int
./run-valgrind ./assembler -p2 input/simple.s out/simple.int



# ./assembler input/simple.s out/simple.int out/simple.out 
# diff log/p2_errors.txt log/ref/p2_errors_ref.txt

# ./assembler input/simple.s out/simple.int out/simple.out 
# ./assembler input/combined.s out/combined.int out/combined.out 
# ./assembler input/comments.s out/comments.int out/comments.out 
# ./assembler input/imm.s out/imm.int out/imm.out 
# ./assembler input/labels.s out/labels.int out/labels.out 
# ./assembler input/pseudo.s out/pseudo.int out/pseudo.out 
# ./assembler input/rtypes.s out/rtypes.int out/rtypes.out 

# ./assembler -p1 input/p1_errors.s out/p1_errors.int -log log/p1_errors.txt
# diff log/p1_errors.txt log/ref/p1_errors_ref.txt


