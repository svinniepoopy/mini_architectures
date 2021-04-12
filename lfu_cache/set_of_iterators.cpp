#include <unordered_set>
#include <list>

int main() {
  using list_it = std::list<int>::iterator;
  std::unordered_set<list_it> S;
  return 0;
}
