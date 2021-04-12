#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <set>

struct lexicographic_less {
  bool operator()(const std::string& a, const std::string& b) const {
    return std::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
  }
};

class SearchSugestions {
  public:
    SearchSugestions(const std::vector<std::string>& P) {
      for (const auto& s:P) {
	Dict.insert(s);
      }
    }

    std::vector<std::vector<std::string>> Suggestions(
	const std::string& word) {
      std::vector<std::vector<std::string>> word_list;
      std::string search;
      for (int i=0;i<word.size();++i) {
	search += word[i];
	std::cout << "Searching: " << search << '\n';
	auto p = std::lower_bound(Dict.begin(), Dict.end(), search); 
	std::vector<std::string> C;
	while (p != Dict.end() && (p->find(search, 0) == 0)) {
	  if (C.size()<3) {
	    C.push_back(*p);
	  }
	  p = std::next(p);
	}
	word_list.push_back(C);
      }
      return word_list;
    }

  private:
    std::set<std::string, lexicographic_less> Dict;
    std::vector<std::string> data;
};

int main() {
  // std::vector<std::string> words{"mobile","mouse","moneypot","monitor","mousepad"};
  // std::vector<std::string> words{"havana"};
  std::vector<std::string> words{"bags","baggage","banner","box","cloths"};
  SearchSugestions sugg{words};
  auto S = sugg.Suggestions("bags");
  for (const auto& v:S) {
    for (const auto& w:v) {
      std::cout << w << " ";
    }
    std::cout << '\n';
  }

  return 0;
}
