#ifndef AVLTREE_H
#define AVLTREE_H

#include "AVLNode.h"

#include <vector>

namespace AVL
{

class Tree
{
  public:
    Tree();
    ~Tree();

    // Creation and deletion of the tree
    void Clear();
    void BuildFromSortedList(std::vector<std::pair<ID, int> > list);

    // Debugging functions
    bool IsSane();
    void PrintTree();

    // Public API for the project
    void Increase(ID id, int m);
    void Reduce(ID id, int m);
    void Next(ID id);
    void Count(ID id);
    void Previous(ID id);
    void InRange(ID left, ID right);
  private:
    enum direction_t { LEFT, RIGHT };
    typedef std::vector<std::pair<Node *, direction_t> > path_t;

    Node * BuildFromSortedListRec(std::vector<std::pair<ID, int> > & list, int l, int r, int depth);
    int IsBalancedRec(Node * node, bool * result);
    // Finds the node with ID and returns it. Else NULL.
    // Also returns the path used to traverse the tree
    Node * Find(ID id, path_t & outPath);
    // Rebalances the tree starting at node. Returns the new root
    Node * Rebalance(Node * node);
  private:
    Node * root;
    int nodeCount;
};

}

#endif
