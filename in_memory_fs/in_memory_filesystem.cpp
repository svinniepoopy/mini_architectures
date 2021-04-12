#include <algorithm>
#include <iterator>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>


class FileSystem {  
  struct Dir {
    Dir(){}
    std::unordered_map<std::string, std::string> files;
    std::unordered_map<std::string, Dir*> dirs;
  };

public:
    FileSystem():root{new Dir{}} {
    }
   
    /*
     * Given a path in string format. If it is a file path, return a list that
     * only contains this file's name. If it is a directory path, return the
     * list of file and directory names in this directory. Your output (file and
     * directory names together) should in lexicographic order.
     */
    std::vector<std::string> ls(std::string path) {
      std::vector<std::string> all_files;
      if (path.size() == 1 && path == "/") {
	for (const auto& f:root->files) {
	  all_files.push_back(f.first);
	}
	for (const auto& dir:root->dirs) {
	  all_files.push_back(dir.first);
	}
      } else {
	Dir* d = root;
	auto pathcmp{PathToComponents(path)};
	int sz = pathcmp.size();
	for (int i=0;i<sz-1;++i) {
	  d = d->dirs[pathcmp[i]];
	}
	auto file_name = pathcmp[sz-1];
	if (d->files.find(file_name) != d->files.end()) {
	  return {file_name};
	} else {
	  d = d->dirs[file_name];
	  for (const auto& f:d->files) {
	    all_files.push_back(f.first);
	  }
	  for (const auto& dir:d->dirs) {
	    all_files.push_back(dir.first);
	  }
	}
      }

      std::sort(all_files.begin(), all_files.end());
      return all_files;
    }
    
    /*
     * Given a directory path that does not exist, you should make a new
     * directory according to the path. If the middle directories in the path
     * don't exist either, you should create them as well. This function has
     * void return type.
     */
    void mkdir(std::string path) {
      auto pathcmp{PathToComponents(path)};
      // create all intermediate paths
      Dir* d = root;
      for (const auto& p:pathcmp) {
	auto it = d->dirs.find(p);
	if (it == d->dirs.end()) {
	  // std::cout << "mkdir " << p << '\n';
	  d->dirs.insert({p, new Dir{}});
	}
	d = d->dirs[p];
      }
    }
   
    /*
     *  Given a file path and file content in string format. If the file doesn't
     *  exist, you need to create that file containing given content. If the
     *  file already exists, you need to append given content to original
     *  content. This function has void return type.
     */
    void addContentToFile(std::string filePath, std::string content) {
      auto pathcmp{PathToComponents(filePath)};
      Dir* d = root;
      const int sz = pathcmp.size(); 
      for (int i=0;i<sz-1;++i) {
	d = d->dirs[pathcmp[i]];
      }
      d->files[pathcmp[sz-1]] = content;
    }
   
    /*
     * Given a file path, return its content in string format
     */
    std::string readContentFromFile(std::string filePath) {
      auto pathcmp{PathToComponents(filePath)};
      Dir* d = root;
      const int sz = pathcmp.size(); 
      for (int i=0;i<sz-1;++i) {
	d = d->dirs[pathcmp[i]];
      }
      auto it = d->files.find(pathcmp[sz-1]);
      if (it != d->files.end()) {
	return it->second;
      }
      return {};
    }

private:   
    std::vector<std::string> PathToComponents(const std::string& path) {
      std::vector<std::string> pathcmp;
      if (path.size() == 1 && path == "/") {
	pathcmp.push_back("");
      } else {
	int start = 1;
	size_t pos = 0;
	while (start < path.size()) {
	  pos = path.find("/", start);
	  if (pos != std::string::npos) {
	    pathcmp.push_back(path.substr(start, pos-start));
	    start = pos+1;
	  } else {
	    pathcmp.push_back(path.substr(start));
	    start = path.size();
	  }
	}
      }     
      return pathcmp;
    }

    Dir* root;
};

void test_simple() {
  FileSystem fs;
  auto contents = fs.ls("/"); 
  std::cout << "ls /\n";
  std::copy(contents.begin(), contents.end(), std::ostream_iterator<std::string>(std::cout, " "));
  std::cout << '\n';
  std::string dirpath{"/a/b/c"};
  fs.mkdir(dirpath);
  contents = fs.ls("/");
  std::copy(contents.begin(), contents.end(), std::ostream_iterator<std::string>(std::cout, " "));
  std::cout << '\n';


  fs.addContentToFile(dirpath+"/d", "this is d");
  fs.addContentToFile(dirpath+"/e", "this is e");
  fs.addContentToFile(dirpath+"/f", "this is f");
  contents = fs.ls(dirpath);
  std::copy(contents.begin(), contents.end(), std::ostream_iterator<std::string>(std::cout, " "));
  std::cout << '\n';
  std::cout << fs.readContentFromFile(dirpath+"/d") << '\n';;
  std::cout << fs.readContentFromFile(dirpath+"/e") << '\n';;
  std::cout << fs.readContentFromFile(dirpath+"/f") << '\n';;

}

void test_hard() {

}

int main() {
  test_simple();
  return 0;
}

