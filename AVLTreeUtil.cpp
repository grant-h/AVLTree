#include "AVLTreeUtil.h"

#include <vector>
#include <algorithm>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <cstdio>

using std::vector;

namespace AVL
{

void TreeUtil::PrintTree(Node * root)
{
#if defined(PRINT_TREE) && defined(DEBUG)
  // get the maximum height and convert to count of nodes, not edges
  int maxHeight = GetNodeHeight(root) + 1;

  vector<Node *> list;
  list.push_back(root);

  PrintTreeRec(list, 1, maxHeight);
#endif
}

int TreeUtil::GetNodeHeight(Node * node)
{
  if(!node)
    return -1;

  return std::max(GetNodeHeight(node->getLeft()),
      GetNodeHeight(node->getRight())) + 1;
}

int TreeUtil::GetNumNodes(Node * node)
{
  if(!node)
    return 0;

  return GetNumNodes(node->getLeft()) + GetNumNodes(node->getRight()) + 1;
}

void TreeUtil::FreeAll(Node * node)
{
  if(!node)
    return;

  // inorder free
  FreeAll(node->getLeft());
  FreeAll(node->getRight());

  delete node;
}

// Taken and adapted from http://stackoverflow.com/a/4973083/5768099
// Not used to solve the project. Only for debug
void TreeUtil::PrintTreeRec(vector<Node *> nodes, int level, int maxLevel)
{
  int nonNull = 0;

  for(int i = 0; i < nodes.size(); i++)
    if(nodes.at(i) != NULL)
      nonNull++;

  if(nodes.size() == 0 || nonNull == 0)
    return;

  int floor = maxLevel - level;
  int endgeLines = (int) pow(2, (std::max(floor - 1, 0)));
  int firstSpaces = (int) pow(2, (floor)) - 1;
  int betweenSpaces = (int) pow(2, (floor + 1)) - 1;

  PrintSpaces(firstSpaces);

  vector<Node *> newNodes;
  for(int i = 0; i < nodes.size(); i++) {
    Node * node = nodes.at(i);

    if (node != NULL) {
        printf("%u", node->getId());
        newNodes.push_back(node->getLeft());
        newNodes.push_back(node->getRight());
    } else {
        putchar(' ');
        newNodes.push_back(NULL);
        newNodes.push_back(NULL);
    }

    PrintSpaces(betweenSpaces);
  }

  putchar('\n');

  for (int i = 1; i <= endgeLines; i++) {
    for (int j = 0; j < nodes.size(); j++) {
      PrintSpaces(firstSpaces - i);

      if (nodes.at(j) == NULL) {
        PrintSpaces(endgeLines + endgeLines + i + 1);
        continue;
      }

      if (nodes.at(j)->getLeft() != NULL)
        putchar('/');
      else
        putchar(' ');

      PrintSpaces(i + i - 1);

      if (nodes.at(j)->getRight() != NULL)
        putchar('\\');
      else
        putchar(' ');

      PrintSpaces(endgeLines + endgeLines - i);
    }

    puts("");
  }

  PrintTreeRec(newNodes, level + 1, maxLevel);
}

void TreeUtil::PrintSpaces(int num, char chr)
{
  for(int i = 0; i < num; i++)
    putchar(chr);
}

}
