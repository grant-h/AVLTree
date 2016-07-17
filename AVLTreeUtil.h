#ifndef AVLTREEUTIL_H
#define AVLTREEUTIL_H

#include "AVLNode.h"

#include <vector>

namespace AVL
{

class TreeUtil
{
public:
  static void PrintTree(Node * root);
  static int GetNodeHeight(Node * node);
  static int GetNumNodes(Node * node);
  static void FreeAll(Node * node);
private:
  static void PrintTreeRec(std::vector<Node *> root, int level, int maxLevel);
  static void PrintSpaces(int num, char chr=' ');
};

}

#endif
