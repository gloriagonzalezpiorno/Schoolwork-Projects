make assembler
./run-valgrind ./assembler -p1 input/simple.s out/simple.int
./run-valgrind ./assembler -p1 input/combined.s out/combined.int
./run-valgrind ./assembler -p1 input/comments.s out/comments.int
./run-valgrind ./assembler -p1 input/imm.s out/imm.int
./run-valgrind ./assembler -p1 input/labels.s out/labels.int
./run-valgrind ./assembler -p1 input/pseudo.s out/pseudo.int
./run-valgrind ./assembler -p1 input/rtypes.s out/rtypes.int
./run-valgrind ./assembler -p1 input/simple.s out/simple.int

# ./assembler -p1 input/simple.s out/simple.int
# ./assembler -p1 input/combined.s out/combined.int
# ./assembler -p1 input/comments.s out/comments.int
# ./assembler -p1 input/imm.s out/imm.int
# ./assembler -p1 input/labels.s out/labels.int
# ./assembler -p1 input/pseudo.s out/pseudo.int
# ./assembler -p1 input/rtypes.s out/rtypes.int
# ./assembler -p1 input/simple.s out/simple.int

