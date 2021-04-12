
/*
 * algorithm
 * starting with the first hit, increment the hits in batches of 300
 * to get hits, find the first hit at t = ts-300 and the distance
 * between this hit and the latest hit
 * 
 * keep track of the hits at ts-300
 * 1-1 < 300<0 1
 * 2 3 4
 * 300-300 0
 * 301 - 300
 *
 * 1 - 1
 * 1 - 2
 * 1 - 4
 *
 * 1 - 300
 *
 * 1 - 301 = 300
 *
 */

#include <iostream>
#include <queue>
#include <utility>

class HitCounter {
  public:
    HitCounter(){}

    void hit(int ts) {
      if (q.empty()) {
	q.emplace_back(ts, 1);
      } else {
	auto last = q.front();
	if (last.first == ts+1) {
	  q.front().second += 1;
	} else {
	  q.emplace_back(ts, 1);
	}
      }
    }

    int getHits(int ts) {
      for (int i=0;i<q.size();++i) {
	const auto e = q[i];
	// std::cout << ts - e.first << '\n';
	if (ts - e.first >= 300) {
	  q.pop_front();
	} else {
	  break;
	}
      }

      int hits = 0;
      for (int i=0;i<q.size();++i) {
	hits += q[i].second;
      }

      return hits;
    }

private:
    // hit, increment
    std::deque<std::pair<int,int>> q;
};

int main() {
  HitCounter c;
  c.hit(1);
  std::cout << c.getHits(1) << '\n';
  c.hit(1);
  std::cout << c.getHits(2) << '\n';
  c.hit(3);
  std::cout << c.getHits(3) << '\n';
  c.hit(3);
  std::cout << c.getHits(3) << '\n';
  c.hit(4); 
  std::cout << c.getHits(4) << '\n';
  c.hit(300);
  std::cout << c.getHits(300) << '\n';
  std::cout << c.getHits(301) << '\n';

  return 0;
}
