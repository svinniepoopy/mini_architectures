// least frequently used cache

#include <iostream>
#include <list>
#include <utility>
#include <unordered_map>
#include <map>
#include <vector>

/*
// use an unordered map as the cache containing the key and a pair of values
// key : {val, <lru list iterator, map iterator>}
// map: items ordered by frequency
// list: items ordered by recent usage

// get: update usage and frequency count
//      update frequency count:
//          get the freq it
//          remove item from the map using the frequency iterator
//          update map by incrementing frequency
//          save iterator
//      update usage count:
//          get the lru list list
//          move the item to the front
//          save iterator
//      update cache with new values of the freq iterator and the lru list iterator

// put: add new item or if the item exists update
//      if the item exists replace the old value with the new and call get and ignore
//      the returned value
//      otherwise
//      do eviction
//      create a new entry in the cache
//      add frequency count
//          save the iterator
//      add usage count
//          save the iterator
//     update cache with the freq and lru list iterator

// eviction: remove the key with the least frequency
// locate the first entry in the map
// if there are multiple entries in this list, select the item with the lower lru value
// for this item remove it from frequency map
// remove it from the lru list
// remove it from the cache
*/

struct node {
  int val;
  int freq;
  std::list<int>::iterator it;
  std::list<std::list<int>::iterator>::iterator list_it;
};

class LFUCache {
public:       
    LFUCache(int capacity):cap{capacity} {}
    
    int get(int key) {
      std::cout << "get " << key << '\n';;
      auto it = cache.find(key);
      if (it == cache.end()) {
	return -1;
      }
      // get the value
      int val = it->second.val;
      int freq = it->second.freq;
      std::cout << "val " << val << " freq " << freq << '\n';

      // pair list_it, map_it
      auto list_it = it->second.it;

      // remove item from frequency map
      auto map_it = freq_map.find(freq);
      std::cout << "map it\n";
      if (map_it->second.size()==1) {
	freq_map.erase(freq);
      } else {
	freq_map[freq].erase(it->second.list_it);
      }
      std::cout << "update lru list \n";
      // update lru list
      lru_list.erase(list_it);
      lru_list.push_front(key);

      // update frequency map
      freq_map[freq+1].push_front(lru_list.begin());
      auto map_list_it = freq_map.find(freq+1);
      // update cache
      cache[key] = node{val, freq+1, lru_list.begin(), map_list_it->second.begin()}; 

      return val;
    }
   
    void evict() {
      if (freq_map.empty()) {
	return;
      }

      auto freq_map_it = freq_map.begin();
      auto iter_list = freq_map_it->second;
      int evict_key;
      if (iter_list.size()>1) {
	evict_key = **std::prev(iter_list.end());
      } else {
	evict_key = **iter_list.begin();
      }
      auto lru_list_it = *std::prev(iter_list.end());
      auto cache_it = cache.find(evict_key);
      std::cout << "evict key: " << evict_key << " evict val " << cache_it->second.val << '\n'; 
      cache.erase(evict_key);
      lru_list.erase(lru_list_it);
      freq_map.begin()->second.pop_back();
    }


    void put(int key, int value) {
      std::cout << "put key " << key << " val " << value << '\n';
      auto it = cache.find(key);
      if (it != cache.end()) {
	auto list_it = it->second.it;
	// update lru list entry
	lru_list.erase(list_it);
	lru_list.push_front(key);

	int freq = it->second.freq;
	// update frequency
	auto map_it = freq_map.find(freq);
	if (map_it->second.size()==1) {
	  freq_map.erase(freq);
	} else {
	  freq_map[freq].erase(it->second.list_it);
	}

	freq_map[freq+1].push_front(lru_list.begin());

	// update cache with new iterators
	auto map_list_it = freq_map.find(freq+1);
	cache.erase(key);
	std::cout << "put update key " << key << " value " << value << " new freq " << freq+1 << '\n'; 
	cache[key] = {value, freq+1, lru_list.begin(), map_list_it->second.begin()};
      } else {
	if (lru_list.size()==cap) {
	  evict();
	}

	// add the new key, val 
	// add to lru list
	lru_list.push_front(key);
	// add to frequency map
	freq_map[1].push_front(lru_list.begin());
	auto map_list_it = freq_map.find(1);
	std::cout << "k " << key << " " << " val " << value << '\n';
	cache[key] = node{value, 1, lru_list.begin(), map_list_it->second.begin()};
      }
    }

private:
    using lst_it = std::list<int>::iterator;

    int cap;
    std::list<int> lru_list;
    // BST of freq to set of list iterators
    std::map<int, std::list<lst_it>> freq_map;
    // hash table of keys to val, list iterator and map iterator
    std::unordered_map<int, node> cache;
};

int main() {
  LFUCache lfu{0};

  /*
  lfu.put(1, 1);
  lfu.put(2, 2);

  int v = lfu.get(1);
  std::cout << "get(1) 1 == " << v << '\n';

  lfu.put(3, 3);

  v = lfu.get(2);
  std::cout << "get(2) -1 == " << v << '\n';

  v = lfu.get(3);
  std::cout << "get(3) 3 == " << v << '\n';

  lfu.put(4, 4);

  v = lfu.get(1);
  std::cout << "get(1) -1 == " << v << '\n';

  v = lfu.get(3);
  std::cout << "get(3) 3 == " << v << '\n';

  v = lfu.get(4);
  std::cout << "get(4) 4 == " << v << '\n';

  lfu.put(5,5);

  v = lfu.get(5);
  std::cout << "get(5) 5 == " << v << '\n';

  lfu.put(6,6);
  lfu.put(7,7);


  v = lfu.get(6);
  std::cout << "get(6) 6 == " << v << '\n';

  v = lfu.get(7);
  std::cout << "get(7) 7 == " << v << '\n';

  lfu.put(3,1);
  lfu.put(2,1);
  lfu.put(2,2);
  lfu.put(4,4);
  int v = lfu .get(2);
  std::cout << v << '\n';
  */
  lfu.put(0,0);
  int v = lfu.get(0);
  return 0;
}
