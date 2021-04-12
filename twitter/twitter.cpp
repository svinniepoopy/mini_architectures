// 
// for each user associate a tweet and a user id
//
// follow:
// 	add an edge from follower to followee
//
// unfollow
// 	remove the edge from the follower to the followee
//
// getnewsfeed
// 	while the number of tweets is less than 10
// 	accumulate this userid's tweets
// 	traverse the graph and accumulate other tweets
// 	arrange them by tweetid
// 	return the 10 most recent tweets
//
// data structures
// 	graph: map of user id to set of followers
// 	tweetset: userid to list of tweets

#include <algorithm>
#include <iostream>
#include <iterator>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class Twitter { 
  public:
    /** Initialize your data structure here. */
    Twitter():ts{0} {        
    }
    
    /** Compose a new tweet. */
    void postTweet(int userId, int tweetId) {
      tweets[userId].emplace_back(tweetId, ts);
      ++ts;
    }   

    /** Retrieve the 10 most recent tweet ids in the user's news feed. 
    Each item in the news feed must be posted by users who the user followed or
    by the user herself. Tweets must be ordered from most recent to least
    recent. */ 
    std::vector<int> getNewsFeed(int userId) {
      using pii = std::pair<int, int>;
      auto cmp = [](const pii& a, const pii& b) {
	return a.second > b.second;
      };

      std::priority_queue<pii, std::vector<pii>, decltype(cmp)> pq{cmp};

      // add the users tweets
      for (const auto& p:tweets[userId]) {
	pq.push(p);
      }
      // add the follower tweets
      for (const auto& v:graph[userId]) {
	for (const auto& p:tweets[v]) {
	  pq.push(p);
	}
      }
      std::vector<int> recent_tweets;
      while (!pq.empty()) {
	recent_tweets.push_back(pq.top().first);
	if(recent_tweets.size()>=10) {
	  break;
	}
	pq.pop();
      }

      return recent_tweets;
    }
    
    /** Follower follows a followee. If the operation is invalid, it should be a
     * no-op. */
    void follow(int followerId, int followeeId) {
      auto it = graph.find(followerId);
      graph[followerId].insert(followeeId);
    }
    
    /** Follower unfollows a followee. If the operation is invalid, it should be
     * a no-op. */
    void unfollow(int followerId, int followeeId) {
      auto it = graph.find(followerId);
      if (it != graph.end()) {
	auto f_it = it->second.find(followeeId);
	if (f_it != it->second.end()) {
	  it->second.erase(f_it);
	}
      }
    }

  private:
    int ts;
    // graph
    std::unordered_map<int, std::unordered_set<int>> graph;
    // tweets
    std::unordered_map<int, std::vector<std::pair<int,int>>> tweets;
};

int main() {
  Twitter t;
  t.postTweet(1, 5);
  t.postTweet(1, 3);
  auto feed = t.getNewsFeed(1);
  std::cout << "1 feed: ";
  std::copy(feed.begin(), feed.end(), std::ostream_iterator<int>(std::cout, " "));
  /*
  t.postTweet(1, 100);

  auto feed = t.getNewsFeed(1);
  std::cout << "1 feed: ";
  std::copy(feed.begin(), feed.end(), std::ostream_iterator<int>(std::cout, " "));
  std::cout << '\n';

  t.postTweet(1, 101);

  feed = t.getNewsFeed(1);
  std::cout << "1 feed: ";
  std::copy(feed.begin(), feed.end(), std::ostream_iterator<int>(std::cout, " "));
  std::cout << '\n';

  t.postTweet(1, 102);

  t.postTweet(2, 200);
  t.postTweet(2, 201);
  t.postTweet(2, 202);

  feed = t.getNewsFeed(2);
  std::cout << "2 feed: ";
  std::copy(feed.begin(), feed.end(), std::ostream_iterator<int>(std::cout, " "));
  std::cout << '\n';

  t.follow(1, 2);

  feed = t.getNewsFeed(1);
  std::cout << "1 feed: ";
  std::copy(feed.begin(), feed.end(), std::ostream_iterator<int>(std::cout, " "));
  std::cout << '\n';

  feed = t.getNewsFeed(2);
  std::cout << "2 feed: ";
  std::copy(feed.begin(), feed.end(), std::ostream_iterator<int>(std::cout, " "));
  std::cout << '\n';

  t.postTweet(4, 400);
  t.postTweet(4, 401);

  t.follow(2, 4);

  t.follow(4, 5);
  t.follow(4, 3);

  t.postTweet(5, 500);
  t.postTweet(5, 501);

  t.postTweet(3, 300);

  feed = t.getNewsFeed(2);
  std::cout << "2 feed: ";
  std::copy(feed.begin(), feed.end(), std::ostream_iterator<int>(std::cout, " "));
  std::cout << '\n';

  feed = t.getNewsFeed(2);
  std::copy(feed.begin(), feed.end(), std::ostream_iterator<int>(std::cout, " "));
  std::cout << '\n';
  */
  return 0;
}
