# Constraint_Programming
Project for the constraint programming course at MPRO

## Launch a resolution

With this code, you can solve two types of DECISION problems : Queens and Coloration on a graph.

For the coloration problem, you need a `.col` file, the program in its current state can only read this kind.

You can either run it by creating your instance in `main.cpp` (else loop around line 38) and compiling, or by creating the `.exe` (say for instance `lol.exe`) file and calling
- For the queens problem :
```
./lol.exe queen NUMBER_OF_QUEENS
```
- For the coloration problem :
```
./lol.exe color PATH_TO_THE_COL_FILE COLORATION_NUMBER_TO_TRY
```
Make sure to check that `parameters.txt` is in the same directory as your executable or else it will crash. And you don't want that.

For each run, you can use different options, activated through the file `parameters.txt`. The options correspond to the following :
```
AC : Use the AC4 algorithm to maintain arc consistency at each node of the search tree
FC : Use forward checking at each node of the search tree, cannot be combined with the arc consistency because of redanduncy
RV : Shuffle the order of visit for the variables and each variable domain before doing the run
SD : Visit first the variables with the smallest domains when going through the search tree
LD : Visit first the variables with the largest domains when going through the search tree
HA : Maintain arc consistency but only half the time to reduce the computation cost
VA : Choose to use values constrained with more values from variables that haven't been fixed yet
SY : Use symmetries to simplify problems (works for both the queens problem and the coloration)
```