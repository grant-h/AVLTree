#ifndef UTIL_CPP
#define UTIL_CPP

#include <vector>
#include <string>
#include <sstream>

using namespace std;

vector<vector<string> > readInputFile(char * filename);
vector<vector<string> > readInputFile(istream & stream);
vector<string> getTokens(string line);
int readInt(string input);

template <typename T>
string join(vector<T> tokens)
{
  stringstream ss;
  string result;

  for(int i = 0; i < tokens.size(); i++)
  {
    ss << tokens[i];

    result += ss.str();

    if(i+1 != tokens.size())
      result += " ";

    ss.str("");
    ss.clear();
  }

  return result;
}


#endif
