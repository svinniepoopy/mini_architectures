#include <iostream>
#include <queue>
#include <vector>

class UserID {
  public:
    UserID():last_id{1} {}

    int Get() {
      if (pq.empty()) {
	pq.push(last_id);
	++last_id;
      }
      int id = pq.top();
      std::cout << "min " << id << '\n';
      pq.pop();
      return id; 
    }

    void Return(int id) {
      pq.push(id);
    }

    int Size() const {
      return pq.size(); 
    }

  private:
    int last_id;
    std::priority_queue<int, std::vector<int>, std::greater<int>> pq;
};

int main() {
  UserID uid;
  std::vector<int> ids;
  for (int i=0;i<10;++i) {
    ids.push_back(uid.Get());
  }
  std::cout << "Size: " << uid.Size() << '\n';
  for (int i=0;i<10;++i) {
    std::cout << ids[i] << '\n';
  }

  std::cout << "Return\n";
  for (int i=0;i<10;++i) {
    uid.Return(ids[i]);
  }

  std::cout << "Size: " << uid.Size() << '\n';

  ids.clear();
  for (int i=0;i<10;++i) {
    ids.push_back(uid.Get());
  }

  std::cout << "Size: " << uid.Size() << '\n';


  uid.Return(3);
  std::cout << uid.Get() << '\n';

  return 0;
}
