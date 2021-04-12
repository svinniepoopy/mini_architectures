#include <cassert>
#include <iterator>
#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>

class FileSystem {
  public:
    FileSystem() {

    }

    std::vector<std::string> PathToComponents(const std::string& path) {
      int start = 1;
      std::vector<std::string> pvec;
      while (start < path.size()) {
	auto pos = path.find("/", start);
	if (pos != std::string::npos) {
	  pvec.push_back(path.substr(start, pos-start));
	  start = pos+1;
	} else {
	  pvec.push_back(path.substr(start, std::string::npos));
	  start = path.size();
	}
      }
      return pvec;
    }

    bool createPath(const std::string& path, int value) {
      if (path[path.size()-1] == '/') {
	return false;
      }
      if (path.empty() || path == "/") {
	return false;
      }
      if (path[0]!='/') {
	return false;
      }

      // path already exists
      if (Files.find(path) != Files.end()) {
	std::cout << "wtf\n";
	return false;
      }

      auto pvec{PathToComponents(path)};

      if (pvec.size()==1) {
	Files[path] = value;
	return true;
      }

      auto pos = path.find_last_of("/");
      const std::string path_level_down = path.substr(0, pos);
      if (Files.find(path_level_down) == Files.end()) {
	return false;
      }

      Files[path] = value;
      return true;
    }

    int get(std::string path) {
      auto it = Files.find(path);
      if (it == Files.end()) {
	return -1;
      }
      return it->second;
    }

  private:
    std::unordered_map<std::string, int> Files;
};

int main() {
  FileSystem fs;
  assert(fs.createPath("/a", 1) == true);

  assert(fs.get("/a") == 1); 

  assert(fs.createPath("/abc", 1) == true);
  assert(fs.createPath("/abc/def", 2) == true);

  assert(fs.get("/abc")  == 1);
  assert(fs.get("/abc/def")  == 2);
  assert(fs.get("/c") == -1);

  assert(fs.createPath("/c/d", 1) == false);

  assert(fs.get(".") == false);

  return 0;
}
