#include <iostream>
#include <map>
#include <memory>
#include <vector>

class SuffixTree {
 private:
  struct Node {
    std::map<char, std::unique_ptr<Node>> edges_;
    int32_t start_;
    int32_t end_;
    Node* suffix_link_;

    bool starts_in_second_string = false;
    bool starts_in_first_string = false;

    Node* GetNext(char symbol) { return edges_.find(symbol)->second.get(); }
    bool HasTransition(char symbol) {
      return (edges_.find(symbol) != edges_.end());
    }
    void InsertEdge(char symbol, Node* vertex) {
      auto it = edges_.find(symbol);
      if (it != edges_.end()) {
        // if we have this transition already, then rewrite it
        it->second.release();
        it->second.reset(vertex);
      } else {
        edges_.emplace_hint(it, symbol, vertex);
      }
    }

    Node(int32_t start, int32_t end)
        : suffix_link_(nullptr), start_(start), end_(end) {}
  };

  struct ActivePoint {
    Node* node_;
    int32_t edge_;
    int32_t length_;
  };

  std::unique_ptr<Node> root_;
  ActivePoint active_point_;
  int32_t remainder_;

  size_t tree_size_;
  std::string text_;

  // returns true if it walks down successfully
  bool WalkDownTheEdge(Node* cur_node);
  static void CreateSuffixLink(Node*& from, Node* to);

  bool FindKCommonSubstringHelper(Node* node, int64_t& k,
                                  std::vector<char>& path_to_result) const;

  void Split(Node* next, int32_t pos_on_edge, int32_t pos,
             Node*& last_created_node);
  void MoveActivePoint(int32_t pos);

 public:
  explicit SuffixTree(std::string text);
  void ExtendWithSymbol(int32_t pos);
  std::string FindKCommonSubstring(int64_t k) const;
  void DFS(Node* node, int32_t first_string_size);
  Node* GetRoot() const { return root_.get(); }
};

SuffixTree::SuffixTree(std::string text)
    : root_(nullptr),
      active_point_({nullptr, 0, 0}),
      text_(std::move(text)),
      remainder_(0),
      tree_size_(1) {
  root_ = std::make_unique<Node>(-1, 0);

  active_point_.node_ = root_.get();
  for (int32_t pos = 0; pos < text_.size(); ++pos) {
    ExtendWithSymbol(pos);
  }
}
void SuffixTree::CreateSuffixLink(SuffixTree::Node*& from,
                                  SuffixTree::Node* to) {
  if (from != nullptr) {
    from->suffix_link_ = to;
    from = nullptr;
  }
}

bool SuffixTree::WalkDownTheEdge(SuffixTree::Node* cur_node) {
  int32_t edge_length = cur_node->end_ - cur_node->start_ + 1;
  if (active_point_.length_ >= edge_length) {
    active_point_.node_ = cur_node;
    active_point_.length_ -= edge_length;
    active_point_.edge_ += edge_length;
    return true;
  } else
    return false;
}

void SuffixTree::Split(Node* next, int32_t pos_on_edge, int32_t pos,
                       Node*& last_created_node) {
  Node* new_internal_node = new Node(next->start_, pos_on_edge - 1);
  active_point_.node_->InsertEdge(text_[next->start_], new_internal_node);

  Node* new_leaf = new Node(pos, text_.size() - 1);
  new_internal_node->InsertEdge(text_[pos], new_leaf);
  new_internal_node->InsertEdge(text_[next->start_ + active_point_.length_],
                                next);
  next->start_ += active_point_.length_;

  CreateSuffixLink(last_created_node, new_internal_node);
  last_created_node = new_internal_node;
  tree_size_ += 2;
}

void SuffixTree::MoveActivePoint(int32_t pos) {
  if (active_point_.node_ == root_.get() && active_point_.length_ > 0) {
    --active_point_.length_;
    active_point_.edge_ = pos - remainder_ + 1;
  } else {
    active_point_.node_ = (active_point_.node_->suffix_link_ == nullptr)
                              ? root_.get()
                              : active_point_.node_->suffix_link_;
  }
}

void SuffixTree::ExtendWithSymbol(int32_t pos) {
  Node* last_created_node = nullptr;
  for (++remainder_; remainder_ > 0;) {
    if (active_point_.length_ == 0) {
      // if we are in some explicit vertex, than we should try to go with the
      // text_[pos] symbol
      active_point_.edge_ = pos;
    }

    if (!active_point_.node_->HasTransition(text_[active_point_.edge_])) {
      // if we don't have a transition from our explicit vertex using
      // active_edge symbol, than we should create it
      ++tree_size_;

      Node* new_leaf = new Node(pos, text_.size() - 1);
      active_point_.node_->InsertEdge(text_[active_point_.edge_], new_leaf);
      CreateSuffixLink(last_created_node, active_point_.node_);

    } else {
      Node* next = active_point_.node_->GetNext(text_[active_point_.edge_]);
      if (WalkDownTheEdge(next)) {
        /* if we walk down successfully than we shouldn't decrease the remainder
         * but walk down, pass all the operations below and give it one more try
         * to walk down
         */
        continue;
      }

      int32_t pos_on_edge = next->start_ + active_point_.length_;
      if (text_[pos_on_edge] == text_[pos]) {
        // if the continuation of our string is correct we only need to increase
        // active length and don't decrease remainder
        ++active_point_.length_;
        if (active_point_.node_ != root_.get())
          CreateSuffixLink(last_created_node, active_point_.node_);
        break;

      } else {
        // if the continuation is wrong we need to do the split
        Split(next, pos_on_edge, pos, last_created_node);
      }
    }
    --remainder_;
    MoveActivePoint(pos);
  }
}

void SuffixTree::DFS(Node* node, int32_t first_string_size) {
  if (node->edges_.empty()) {
    // if it's a leaf than we can decide to what string it belongs
    if (node->start_ <= first_string_size) {
      node->starts_in_first_string = true;
    } else {
      node->starts_in_second_string = true;
    }
  }
  for (const auto& vertex_edge : node->edges_) {
    DFS(vertex_edge.second.get(), first_string_size);
    node->starts_in_first_string |= vertex_edge.second->starts_in_first_string;
    node->starts_in_second_string |=
        vertex_edge.second->starts_in_second_string;
  }
}

bool SuffixTree::FindKCommonSubstringHelper(
    SuffixTree::Node* node, int64_t& k,
    std::vector<char>& path_to_result) const {
  if (node->starts_in_first_string && node->starts_in_second_string) {
    int32_t edge_length = node->end_ - node->start_ + 1;
    if (edge_length < k) {
      k -= edge_length;
    } else {
      return true;
    }
  } else {
    /* if the string is not substring of the first string
     * and the second one at the same time than we should go down
     */
    return false;
  }
  for (const auto& vertex_edge : node->edges_) {
    if (FindKCommonSubstringHelper(vertex_edge.second.get(), k,
                                   path_to_result)) {
      // if we found the answer, than we should return from recursion
      path_to_result.push_back(vertex_edge.first);
      return true;
    }
  }
  return false;
}

std::string SuffixTree::FindKCommonSubstring(int64_t k) const {
  std::vector<char> path_to_result;
  for (const auto& vertex_edge : root_->edges_) {
    if (FindKCommonSubstringHelper(vertex_edge.second.get(), k,
                                   path_to_result)) {
      path_to_result.push_back(vertex_edge.first);
      break;
    }
  }

  std::string result;
  Node* current_node = root_.get();
  for (auto it = path_to_result.rbegin(); it != path_to_result.rend(); ++it) {
    current_node = current_node->GetNext(*it);
    int64_t edge_length = current_node->end_ - current_node->start_ + 1;
    if (it == path_to_result.rend() - 1) {
      // on the last edge we should go only k steps for the answer
      edge_length = k;
      k = 0;
    }
    for (int32_t i = 0; i < edge_length; ++i) {
      result.push_back(text_[current_node->start_ + i]);
    }
  }

  return (k > 0) ? "-1" : result;
}

std::string KCommonSubstring(const std::string& s, const std::string& t,
                             int64_t k) {
  SuffixTree my_suffix_tree(s + "#" + t + "$");
  my_suffix_tree.DFS(my_suffix_tree.GetRoot(), s.size());
  return my_suffix_tree.FindKCommonSubstring(k);
}

int main() {
  std::string s;
  std::string t;
  int64_t k = 0;
  std::cin >> s >> t >> k;
  std::cout << KCommonSubstring(s, t, k);
  return 0;
}
