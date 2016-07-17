#include "util.h"

#include <sstream>
#include <fstream>
#include <cstdio>
#include <cstdlib>

using namespace std;

vector<vector<string> > readInputFile(char * filename)
{
  vector<vector<string> > splitlines;
  ifstream inputFile(filename);
  string line;

  if(inputFile.fail())
  {
      printf("fatal: could not open %s\n", filename);
      exit(1);
  }

  splitlines = readInputFile(dynamic_cast<istream &>(inputFile));
  inputFile.close();

  return splitlines;
}

vector<vector<string> > readInputFile(istream & stream)
{
  vector<vector<string> > splitlines;
  string line;

  // get N count from beginning of file
  getline(stream, line);

  // string -> integer
  int items = readInt(line);

#ifdef DEBUG
  printf("Input file has %d items\n", items);
#endif

  // read the N items
  for(int i = 0; i < items; i++)
  {
    if(!stream)
    {
      printf("fatal: stream error when reading\n");
      exit(1);
    }

    getline(stream, line);

    // ignore blank lines
    if(line == "")
    {
      i--;
      continue;
    }

    vector<string> subitems = getTokens(line);
    splitlines.push_back(subitems);
  }

  return splitlines;
}

vector<string> getTokens(string line)
{
    vector<string> subitems;

    string item;
    stringstream ss(line);

    // split input line in to tokens delimited by ' '
    while(getline(ss, item, ' '))
    {
      subitems.push_back(item);
      item = "";
    }

    return subitems;
}

int readInt(string input)
{
  stringstream ss;
  int output = 0;

  ss << input;
  ss >> output;

  return output;
}
