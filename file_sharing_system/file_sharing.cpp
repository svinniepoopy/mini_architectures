#include <algorithm>
#include <queue>
#include <utility>
#include <vector>
#include <unordered_map>
#include <set>

class FileSharing {
  public:
    FileSharing(int m) {

    }

    // assign a new user id to the user (smallest positive integer not taken by any other user)
    // return the id
    // 
    int join(std::vector<int> ownedChunks) {
      int user_id = GetID();
      // add the chunks to the file sharing system
      AddChunks(user_id, ownedChunks);
      return user_id;
    }

    void leave(int userID) {
      ClaimID(userID);
      RemoveChunks(userID);
    }

    // send a chunk with the given to the given user id
    std::vector<int> request(int userID, int chunkID) {
      auto user_set = GetChunkUsers(chunkID);
      if (user_set.empty()) {
	return {};
      }
      std::vector<int> users{user_set.begin(), user_set.end()};
      AssignChunk(userID, chunkID);
      return users;
    }

    std::vector<int> chunks() {
      return {};
    }

  private:

    std::set<int> GetChunkUsers(int chunk_id) {
      return ChunkToUser[chunk_id];
    }

    void AddChunks(int user_id, std::vector<int>& chunks) {
      for (const auto& chunk:chunks) {
	ChunkToUser[chunk].insert(user_id);
      }
      UserToChunk[user_id] = std::move(chunks);
    }

    // remove the chunks that were in the file sharing system for this user id
    void RemoveChunks(int user_id) {
      // get all the chunks assigned to the user
      auto chunks = UserToChunk[user_id];
      // remove the chunks from the chunk map
      for (const auto& c:chunks) {
	ChunkToUser[c].erase(user_id);
      }
    }

    // assign the user id to this chunk id
    void AssignChunk(int user_id, int chunk_id) {
      UserToChunk[user_id].push_back(chunk_id);
      ChunkToUser[chunk_id].insert(user_id);
    }

    void ClaimID(int user_id) {
      pq.push(user_id);
    }

    int GetID() {
      if (pq.empty()) {
	pq.push(user_id);
	++user_id;
      }
      int id = pq.top();
      pq.pop();
      return id;
    }

    int user_id;

    // user id to set of chunks
    std::unordered_map<int, std::vector<int>> UserToChunk;
    // chunk id to list of user ids who have this chunk
    std::unordered_map<int, std::set<int>> ChunkToUser;
    // return the smallest user id
    std::priority_queue<int, std::vector<int>, std::greater<int>> pq;
};

int main() {
  FileSharing fs{4};
  int id1 = fs.join({1,2});
  int id2 = fs.join({2,3});
  int id3 = fs.join({4});

  fs.leave(id1);
  auto avl = fs.chunks();

  fs.leave(id2);
  // 1 chunk available
  avl = fs.chunks();

  fs.leave(id3);
  // no chunks available
  avl = fs.chunks();

  return 0;
}
