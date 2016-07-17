#include "AVLNode.h"

#include <cassert>
#include <cmath>
#include <cstdlib>
#include <cstdio>

#include "AVLTreeUtil.h"

namespace AVL
{

Node::Node(ID id, int count)
  :id(id), count(count), height(0), heightValid(true)
{
  left = right = parent = 0;
  assert(count > 0 && id > 0);
}

Node::~Node()
{
  left = right = parent = 0;
}

void Node::setLeft(Node * node)
{
  left = node;
  heightValid = false;

  if(left)
    left->setParent(this);
}

void Node::setRight(Node * node)
{
  right = node;
  heightValid = false;

  if(right)
    right->setParent(this);
}

void Node::setParent(Node * node)
{
  parent = node;

  // adding a parent doesnt change the height of this node
}

Node * Node::getLeft()
{
  return left;
}

Node * Node::getRight()
{
  return right;
}

Node * Node::getParent()
{
  return parent;
}

ID Node::getId()
{
  return id;
}

int Node::getCount()
{
  return count;
}

int Node::increase(int amt)
{
  assert(amt > 0);

  count += amt;

  return count;
}

int Node::decrease(int amt)
{
  // make sure we dont decrease a node once it is below or equal
  // to zero
  assert(count > 0);
  assert(amt > 0);

  count -= amt;

  return count;
}

/* Calculates the height and balance of the current node.
 * A height of -1 means no height (no node).
 * Height is measured in the number of edges, not nodes.
 *
 * Case 1 - No children
 *   - leftH = -1
 *   - rightH = -1
 *   - height = max(l, r) + 1 = 0
 *   - balance = leftH - rightH = 0
 *
 * Case 2 - Left child only
 *   - leftH = 0
 *   - rightH = -1
 *   - height = max(l, r) + 1 = 1
 *   - balance = leftH - rightH = 1
 *
 * Case 3 - Right child only
 *   - leftH = -1
 *   - rightH = 0 
 *   - height = max(l, r) + 1 = 1
 *   - balance = leftH - rightH = -1
 *
 * Case 4 - Both children
 *   - leftH = 0 
 *   - rightH = 0 
 *   - height = max(l, r) + 1 = 1
 *   - balance = leftH - rightH = 0
 *
 * Case 5 - Imbalanced left tree
 *   - leftH = 2 
 *   - rightH = 0 
 *   - height = max(l, r) + 1 = 3
 *   - balance = leftH - rightH = 2
 *
 * Case 6 - Imbalanced right tree
 *   - leftH = -1 
 *   - rightH = 1 
 *   - height = max(l, r) + 1 = 2
 *   - balance = leftH - rightH = -2
 *
 */

int Node::getHeight()
{
  if(heightValid)
    return height;

  int leftH = this->getLeft() ? this->getLeft()->getHeight() : -1;
  int rightH = this->getRight() ? this->getRight()->getHeight() : -1;
  int newHeight = std::max(leftH, rightH) + 1;

  height = newHeight;
  heightValid = true;

  return height;
}

int Node::getBalance()
{
  int leftH = this->getLeft() ? this->getLeft()->getHeight() : -1;
  int rightH = this->getRight() ? this->getRight()->getHeight() : -1;
  return leftH - rightH;
}

bool Node::needsRebalance()
{
  return abs(getBalance()) > 1;
}

}
