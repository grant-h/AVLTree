# AVLTree

A quick AVL tree implementation that I whipped up for a UF data structures.
It reads in a description of a tree from an input file and then executes commands on this tree from STDIN.

Not the best data structure implementation I've ever written (and it certainly shouldn't be used in any real projects), but it's still significant enough that it should be public to demonstrate my understanding of C++.

## Building

* Compiler: g++
* Tool: GNU Make

```
AVLTree $ make
g++ -O2 -c bbst.cpp -o bbst.o
g++ -O2 -c AVLTree.cpp -o AVLTree.o
g++ -O2 -c AVLNode.cpp -o AVLNode.o
g++ -O2 -c AVLTreeUtil.cpp -o AVLTreeUtil.o
g++ -O2 -c util.cpp -o util.o
g++ -o bbst bbst.o AVLTree.o AVLNode.o AVLTreeUtil.o util.o
```

```
AVLTree $ ./bbst input_tree.txt
```

See the `test/` directory for example trees and commands.
