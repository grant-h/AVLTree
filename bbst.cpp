#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <vector>

#include "util.h"
#include "AVLTree.h"

using namespace std;

int main(int argc, char * argv[])
{
  if(argc != 2)
  {
    printf("fatal: input file required\n");
    return 1;
  }

  char * filename = argv[1];

  vector<vector<string> > lines = readInputFile(filename);
  vector<pair<AVL::ID, int> > nodes;

  AVL::ID lastId = 0;

  for(int i = 0; i < lines.size(); i++)
  {
    if(lines[i].size() != 2)
    {
      printf("fatal: malformed node input at line %d\n", i+1);
      return 1;
    }

    int id = readInt(lines[i][0]);
    int count = readInt(lines[i][1]);

    if(id <= 0 || count <= 0)
    {
      printf("fatal: malformed node ID(%d) at line %d\n", id, i+1);
      return 1;
    }

    // cast the signed int to an unsigned ID now that we have verified the ID's sanity
    AVL::ID newId = (AVL::ID)id;

    if(lastId >= newId)
    {
      printf("fatal: ID was not strictly increasing at line %d\n", i+1);
      return 1;
    }

    lastId = newId;

    nodes.push_back(pair<AVL::ID, int>(newId, count));
  }

  AVL::Tree tree;

  // Build the tree in O(n) time
  tree.BuildFromSortedList(nodes);

#ifdef DEBUG
  tree.PrintTree();
  if(!tree.IsSane())
  {
    printf("Tree is INSANE after creation\n");
    exit(1);
  }
#endif

  bool bQuit = false;

  while(!bQuit)
  {
    string line;
    getline(cin, line);

    if(!cin)
      break;

    vector<string> args = getTokens(line);
    int numArgs = args.size();

    if(numArgs == 0)
      continue;

    string cmd = args[0];
    AVL::ID id = (AVL::ID)readInt(args[1]);

    if(cmd == "increase")
    {
      int count = readInt(args[2]);

#ifdef DEBUG
      printf("ID %u += %d\n", id, count);
#endif

      tree.Increase(id, count);
    }
    else if(cmd == "reduce")
    {
      int count = readInt(args[2]);

#ifdef DEBUG
      printf("ID %u -= %d\n", id, count);
#endif

      tree.Reduce(id, count);
    }
    else if(cmd == "next")
    {
#ifdef DEBUG
      printf("ID %u next\n", id);
#endif
      tree.Next(id);
    }
    else if(cmd == "count")
    {
#ifdef DEBUG
      printf("ID %u count\n", id);
#endif
      tree.Count(id);
    }
    else if(cmd == "previous")
    {
#ifdef DEBUG
      printf("ID %u previous\n", id);
#endif
      tree.Previous(id);
    }
    else if(cmd == "inrange")
    {
      AVL::ID id2 = readInt(args[2]);
#ifdef DEBUG
      printf("ID1 %u <-> ID2 %u\n", id, id2);
#endif
      tree.InRange(id, id2);
    }
    else if(cmd == "quit")
    {
      bQuit = true;
    }
    else
    {
      printf("fatal: unrecognized command '%s'\n", cmd.c_str());
      exit(1);
    }

#ifdef DEBUG
  tree.PrintTree();
  if(!tree.IsSane())
  {
    printf("Tree is INSANE after last command\n");
    exit(1);
  }
#endif
  }

  return 0;
}
