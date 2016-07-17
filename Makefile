CC=$(PREFIX)gcc
CXX=$(PREFIX)g++
#CXXFLAGS=-DDEBUG -DPRINT_TREE -Wall -ggdb -fno-omit-frame-pointer -fsanitize=address
#CXXFLAGS=-O2 -Wall -fsanitize=address
#CXXFLAGS=-ggdb -fno-omit-frame-pointer -fsanitize=address
CXXFLAGS=-O2

#LDFLAGS=-fsanitize=address

#NOTE: turn on DEBUG and set NDEBUG before submission!

SRC=bbst.cpp AVLTree.cpp AVLNode.cpp AVLTreeUtil.cpp util.cpp
OBJ=$(SRC:%.cpp=%.o)

all : bbst

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

bbst : $(OBJ)
	$(CXX) -o bbst $(OBJ) $(LDFLAGS)

clean:
	-rm -f $(OBJ) bbst

test: all
	./test.sh

.PHONY: all test clean
