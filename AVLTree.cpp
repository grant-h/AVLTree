#include "AVLTree.h"

#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include <queue>

#include "AVLTreeUtil.h"

using namespace std;

namespace AVL
{

Tree::Tree()
  :root(0), nodeCount(0)
{

}

Tree::~Tree()
{
  Clear();
}

void Tree::Clear()
{
  TreeUtil::FreeAll(root);
  root = NULL;
}

void Tree::BuildFromSortedList(vector<pair<ID, int> > list)
{
  Clear();

  // We want O(n) tree creation from a sorted list.
  // The algorithm for this is simple: find the middle element of the list
  // This element becomes the root. This step is performed recursively
  // on the right and left lists which create the right and left childs
  root = BuildFromSortedListRec(list, 0, list.size()-1, 0);

  nodeCount = list.size();
}

Node * Tree::BuildFromSortedListRec(vector<pair<ID, int> > & sublist, int l, int r, int depth)
{
  // if the list we have is empty or the right bound is less than the left bound,
  // return no node
  if(sublist.size() == 0 || r < l)
    return NULL;

  int middle =  l + (r - l)/2;

  pair<ID, int> middleNode = sublist[middle];

  // left - id, right - count
  Node * subroot = new Node(middleNode.first, middleNode.second);

  //printf("Root %u, l %d, m %d, r %d\n", middleNode.first, l, middle, r);

  subroot->setLeft(BuildFromSortedListRec(sublist, l, middle-1, depth+1));
  subroot->setRight(BuildFromSortedListRec(sublist, middle+1, r, depth+1));

  return subroot;
}

bool Tree::IsSane()
{
  if(this->root == NULL && nodeCount != 0)
  {
    printf("Root is NULL but we should have nodes left!\n");
    return false;
  }

  int nodeCountCheck = TreeUtil::GetNumNodes(this->root);
  if(nodeCountCheck != nodeCount)
  {
    printf("Node count FAILED. Got %d, expected %d\n",
        nodeCountCheck, nodeCount);
    return false;
  }

  bool balanced = true;

  // Check if the AVL tree is balanced
  // returns the height, which we ignore
  IsBalancedRec(this->root, &balanced);

  // Make sure every node has correct parent pointers
  // left - node, right expected parent
  bool parentsOkay = true;

  typedef pair<Node *, Node *> item_t;
  queue<item_t> frontier;

  frontier.push(item_t(this->root, NULL));

  while(!frontier.empty())
  {
    item_t i = frontier.front();
    frontier.pop();

    Node * n = i.first, *expectedParent = i.second;
    Node * parent = n->getParent();

    if(parent != expectedParent)
    {
      parentsOkay = false;
      if(parent)
        printf("Parent failure check at node %u - got %p, exp %u\n",
            n->getId(), parent, expectedParent->getId());
      else
        printf("Parent failure check at node %u - got NULL, exp %u\n",
            n->getId(), expectedParent->getId());
      break;
    }

    if(n->getLeft())
      frontier.push(item_t(n->getLeft(), n));
    if(n->getRight())
      frontier.push(item_t(n->getRight(), n));
  }

  return balanced && parentsOkay;
}

int Tree::IsBalancedRec(Node * node, bool * result)
{
  if(!node)
    return -1;

  int leftHeight = IsBalancedRec(node->getLeft(), result);
  int rightHeight = IsBalancedRec(node->getRight(), result);
  int height = std::max(leftHeight, rightHeight) + 1;

  if(abs(leftHeight - rightHeight) > 1) {
    printf("Tree imbalance at ID %u\n", node->getId());
    *result = false;
  }

  return height;
}

void Tree::PrintTree()
{
  TreeUtil::PrintTree(root);
}

///////////////////////////////////////////////////////////
// PUBLIC API FOR PROJECT
///////////////////////////////////////////////////////////

void Tree::Increase(ID id, int m)
{
  path_t path;

  // ignore weird counts
  if(m <= 0)
    return;

  Node * found = Find(id, path);

  if(found) // found a match
  {
    found->increase(m);
    printf("%d\n", found->getCount());
    return;
  }

  // The only way to reach this point is that a node with
  // id was not found. We must add a node and possibly rebalance the tree
  Node * newNode = new Node(id, m);

  // Increase the node count
  nodeCount++;

  // trivial case: first insert
  if(path.size() == 0)
  {
    root = newNode;
    printf("%d\n", root->getCount());
    return;
  }

  // insert new node in to tree
  // If we fell off going left, set the left child to the
  // new node. Else set right child
  if(path.at(path.size()-1).second == LEFT)
    path.at(path.size()-1).first->setLeft(newNode);
  else
    path.at(path.size()-1).first->setRight(newNode);

  for(int i = path.size()-1; i >= 0; i--)
  {
    Node * cur = path.at(i).first;

#ifdef DEBUG
    printf("Path %u,D %d\n",
        cur->getId(),
        path.at(i).second);
#endif

    Node * parent = cur->getParent();
    Node * newRoot = Rebalance(cur);
#ifdef DEBUG
    printf("Rebalanced %u\n", cur->getId());
#endif

    // is this the last node?
    if(parent == NULL)
    {
      root = newRoot;
    }
    else
    {
      if(path.at(i-1).second == LEFT)
        parent->setLeft(newRoot);
      else
        parent->setRight(newRoot);
    }
  }

  printf("%d\n", newNode->getCount());
  return;
}

void Tree::Reduce(ID id, int m)
{
  path_t path;

  // ignore weird counts
  if(m <= 0)
    return;

  Node * found = Find(id, path);

  if(!found) // no match
  {
    printf("0\n");
    return;
  }

  // We found a match. Decrease the count
  int newCount = found->decrease(m);

  // If the new count is above the threshold, then keep the node
  if(newCount > 0) {
    printf("%d\n", newCount);
    return;
  }

  // adjust the node count
  nodeCount--;

  // The node has dropped below the count minimum. Remove it, but first find a suitable candidate
  Node * leftChild = found->getLeft();
  Node * rightChild = found->getRight();
  Node * parent = found->getParent();
  Node * replacement = NULL;

  // Trivial cases
  if(leftChild == NULL || rightChild == NULL)
  {
    Node * newRoot = NULL;

    // Trivial case: leaf node
    if(leftChild == NULL && rightChild == NULL)
      replacement = NULL;
    // Right child only
    else if(leftChild == NULL)
      replacement = rightChild;
    // Left child only
    else
      replacement = leftChild;

    // Replacement hasn't been assigned a parent yet
    if(replacement)
      replacement->setParent(NULL);

    // Most trivial case: delete root with one or no children
    // No rebalance needed as balance factor must have already been abs(1) or less
    if(!parent)
    {
      // if we have a non-leaf node
      if(replacement)
        replacement->setParent(NULL);

      root = replacement;

      printf("0\n");
      delete found;
      return;
    }
  }
  // This node has both its children
  else
  {
    // Find the leftmost node in the right subtree as a replacement candidate
    Node * leftMost = found->getRight();
    direction_t leftMostDir = RIGHT;

    // keep going left until we can't
    while(leftMost->getLeft()) {
      leftMost = leftMost->getLeft();
      leftMostDir = LEFT;
    }

    Node * leftMostRightTree = leftMost->getRight();

#ifdef DEBUG
    printf("Leftmost %u\n", leftMost->getId());
#endif

    // detach left most node from right subtree
    // Also set the new child of the leftMost parent to the right child of the
    // leftmost (it may be null). This also sets the parent
    /*
     *             DIR
     *     Left           Right
     *
     *         X           D
     *        /             \
     *       L               L      L ==> D
     *        \               \
     *         Y               Z
     */
    if(leftMostDir == LEFT)
      leftMost->getParent()->setLeft(leftMostRightTree);

#ifdef DEBUG
    printf("BBBBBB\n");
    PrintTree();
#endif

    // remove reference to right subtree as it's in another tree
    // but only if we traveled left
    if(leftMostDir == LEFT)
      leftMost->setRight(NULL);

    // left most was our replacement node
    replacement = leftMost;
    // Replacement hasn't been assigned a parent yet
    replacement->setParent(NULL);

    // Inject the replacement in to the tree
    replacement->setLeft(leftChild);

    if(leftMostDir == LEFT)
    {
      // Rebalance the right child of the found node as it may require
      // a rebalance after the removal of the left most subchild
      replacement->setRight(Rebalance(found->getRight()));
    }
    // Case where there was no leftmost subchild (we never traveled left)

#ifdef DEBUG
    printf("After leftmost right rebalance\n");
    PrintTree();
    TreeUtil::PrintTree(replacement);
#endif
  }

  // We had a parent, meaning we're not the root
  if(parent)
  {
    // Get the last direction we took to get here
    direction_t dir = path.at(path.size()-1).second;

    // Change the parent's corresponding child
    if(dir == LEFT)
      parent->setLeft(replacement);
    else
      parent->setRight(replacement);
  }
  else
  {
    replacement = Rebalance(replacement);

    replacement->setParent(NULL);
    root = replacement;
  }

#ifdef DEBUG
  printf("AAAAAAAA\n");
  PrintTree();
#endif

  // Now rebalance the path upwards
  for(int i = path.size()-1; i >= 0; i--)
  {
    Node * cur = path.at(i).first;

#ifdef DEBUG
    printf("Path %u,D %d\n",
        cur->getId(),
        path.at(i).second);
#endif

    Node * rebalParent = cur->getParent();
    Node * newRoot = Rebalance(cur);
#ifdef DEBUG
    printf("Rebalanced %u\n", cur->getId());
#endif

    // is this the last node?
    if(rebalParent == NULL)
    {
      root = newRoot;
    }
    else
    {
      if(path.at(i-1).second == LEFT)
        rebalParent->setLeft(newRoot);
      else
        rebalParent->setRight(newRoot);
    }
  }

  printf("0\n");
  delete found;
  return;
}

void Tree::Next(ID id)
{
  Node * cur = root;
  ID minId = 4000000000; // Infinity
  Node * minNode = NULL;

  while(cur != NULL)
  {
    // go left to find a smaller match
    if(id < cur->getId())
    {
      if(minId > cur->getId())
      {
        minId = cur->getId();
        minNode = cur;
      }

      cur = cur->getLeft();
    }
    // go right to see if something is bigger than id
    else // id >= cur.id
    {
      cur = cur->getRight();
    }
  }

  if(minNode)
    printf("%u %d\n", minNode->getId(), minNode->getCount());
  else
    printf("0 0\n");
}

void Tree::Count(ID id)
{
  path_t path;

  Node * found = Find(id, path);

  if(!found) // no match
  {
    printf("0\n");
    return;
  }

  printf("%d\n", found->getCount());
}

void Tree::Previous(ID id)
{
  Node * cur = root;
  ID maxId = 0; // No id (0)
  Node * maxNode = NULL;

  while(cur != NULL)
  {
    // go right to get something closer
    if(id > cur->getId())
    {
      if(maxId < cur->getId())
      {
        maxId = cur->getId();
        maxNode = cur;
      }

      cur = cur->getRight();
    }
    // go left to get something smaller than id
    else // id <= cur.id
    {
      cur = cur->getLeft();
    }
  }

  if(maxNode)
    printf("%u %d\n", maxNode->getId(), maxNode->getCount());
  else
    printf("0 0\n");
}

void Tree::InRange(ID left, ID right)
{
  // some bad case 
  if(right < left)
  {
    printf("0\n");
    return;
  }

  int countAccum = 0;

  // Do a tree search starting from root
  queue<Node *> frontier;

  frontier.push(this->root);

  while(!frontier.empty())
  {
    Node * n = frontier.front();
    frontier.pop();

    bool inLeft = false, inRight = false;
    ID nId = n->getId();

    // If the current ID is less than the left range
    // Dont go left as that subtree can never be
    // greater than the left range
    if(nId >= left)
    {
      if(n->getLeft())
        frontier.push(n->getLeft());

      inLeft = true;
    }

    // If the current ID is greater than the right range
    // Dont go right as that subtree can never be
    // less than than the right range
    if(nId <= right)
    {
      if(n->getRight())
        frontier.push(n->getRight());

      inRight = true;
    }

    if(inLeft && inRight)
      countAccum += n->getCount();
  }

  printf("%d\n", countAccum);
}

Node * Tree::Find(ID id, path_t & outPath)
{
  Node * cur = root;
  Node * lastNode = NULL;

  // Which node and which direction did we choose on that node
  path_t path;

  while(cur != NULL)
  {
    // keep note of the path we took
    lastNode = cur;

    if(id > cur->getId()) // go right
    {
      path.push_back(pair<Node *, direction_t>(cur, RIGHT));
      cur = cur->getRight();
    }
    else if(id < cur->getId()) // go left
    {
      path.push_back(pair<Node *, direction_t>(cur, LEFT));
      cur = cur->getLeft();
    }
    else // found a match
    {
      outPath = path;
      return cur;
    }
  }

  outPath = path;
  return NULL;
}

/** Rebalance the tree starting at node
 *
 *  This can return a new rearrainged tree, if a balance was made.
 *  In this case, the new tree has no parent and it must be set.
 *
 *  @returns New root node with no parent or NULL
 */
Node * Tree::Rebalance(Node * node)
{
  /* Steps to decide on rebalancing
   *
   * 1. Check starting node balance, if needs rebalance continue to 2, else
   *    process parent, if any. If no parent, stop
   * 2. Determine rebalance type
   * 3. Perform appropriate rotation:
   *
   *   * Left (LR)
   *
   *         A         <--.          B
   *        / \            \        / \
   *       X   B            "      /   \
   *          / \       B   |     A     C
   *         Y   C         /     / \   / \
   *            / \   '---'     X   Y 0   0
   *           0   0
   *
   *   * Right (RR)
   *
   *         A         .-->          B
   *        / \       /             / \
   *       B   X     "             /   \
   *      / \        |  B         C     A
   *     C   Y       \           / \   / \
   *    / \           '---'     0   0 Y   X
   *   0   0
   *
   *   * Left-Right (LRR)
   *
   *         A         <--.          A       .-->         C
   *        / \            \        / \     /            / \
   *       B   X            "      C   X   "            /   \
   *      / \           B   |     / \      |  C        B     A
   *     Y   C             /     B   I     \          / \   / \
   *        / \       '---'     / \         '---'    Y   J I   X
   *       J   I               Y   J
   *
   *   * Right-Left (RLR)
   *
   *         A          .-->     A           <--.          C
   *        / \        /        / \              \        / \
   *       X   B      "        X   C              "      /   \
   *          / \     |  B        / \         C   |     A     B
   *         C   Y    \          I   B           /     / \   / \
   *        / \        '---'        / \     '---'     X   I J   Y
   *       I   J                   J   Y
   */

  // empty tree, no parent
  if(!node)
    return NULL;

  int balance = node->getBalance();

#ifdef DEBUG
  printf("Rebalance: balance %d\n", balance);
#endif

  // leaf node
  if(node->getLeft() == NULL && node->getRight() == NULL)
    return node;

  // Tree is right heavy
  if(balance == -2)
  {
    Node * A = node;
    Node * B = node->getRight();

    // Left rotate (LR)
    if(B->getBalance() < 0)
    {
#ifdef DEBUG
      printf("Rebalance: LR\n");
#endif

      Node * C = B->getRight();
      Node * Y = B->getLeft();

      A->setRight(Y);
      B->setLeft(A);
      B->setRight(C);
      B->setParent(NULL);

      return B;
    }
    // Right-Left rotation (RLR)
    else
    {
#ifdef DEBUG
      printf("Rebalance: RLR\n");
#endif

      Node * C = B->getLeft();
      Node * I = C->getLeft();
      Node * J = C->getRight();

      A->setRight(I);
      B->setLeft(J);
      C->setLeft(A);
      C->setRight(B);
      C->setParent(NULL);

      return C;
    }
  }
  // Tree is left heavy
  else if(balance == 2)
  {
    Node * A = node;
    Node * B = node->getLeft();

    // Right rotate (RR)
    if(B->getBalance() > 0)
    {
#ifdef DEBUG
      printf("Rebalance: RR\n");
#endif

      Node * C = B->getLeft();
      Node * Y = B->getRight();

      A->setLeft(Y);
      B->setRight(A);
      B->setLeft(C);
      B->setParent(NULL);

      return B;
    }
    // Left-Right rotation (LRR)
    else
    {
#ifdef DEBUG
      printf("Rebalance: LRR pivot %u\n", A->getId());
#endif

      Node * C = B->getRight();
      Node * J = C->getLeft();
      Node * I = C->getRight();

      B->setRight(J);
      A->setLeft(I);
      C->setLeft(B);
      C->setRight(A);
      C->setParent(NULL);

      return C;
    }
  }
  // Tree is balanced
  else
  {
    return node;
  }
}


}
