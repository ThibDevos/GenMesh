#ifndef FILE_FUNCTIONS
#define FILE_FUNCTIONS

#include <fstream>

void go_to_keyword(std::ifstream& f, std::string keyword)
{
  bool found = false;
  bool eof = false;
  f.clear();
  f.seekg(0);
  std::string line;
  getline(f,line);
  while ( (line.find(keyword) == std::string::npos) && !eof)
  {
    eof = !getline(f,line);
  }
  if(!found) std::runtime_error("keyword not found");
}

#endif //FILE_FUNCTIONS