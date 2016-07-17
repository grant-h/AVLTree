#ifndef AVLNODE_H
#define AVLNODE_H

namespace AVL
{

typedef unsigned int ID;

class Node
{
  public:
    Node(ID id, int count);
    ~Node();

    // Tree structure
    // Set the left child and also the left child’s parent to this
    void setLeft(Node * node);
    // Set the right child and also the right child’s parent to this
    void setRight(Node * node);
    void setParent(Node * node);
    Node * getLeft();
    Node * getRight();
    Node * getParent();

    // Field mutators and getters
    ID getId();
    int getCount();
    // Returns the new count and decreases count by amt
    int increase(int amt);
    // Returns the new count and increases count by amt
    int decrease(int amt);

    // Tree metadata
    int getHeight();
    int getBalance();
    bool needsRebalance();
  private:
    void calculateHeight();
  private:
    ID id;
    int count;

    // Cached version of the height. Invalidated when structure changed
    int height;
    bool heightValid;

    Node *left, *right, *parent;
};

}

#endif
