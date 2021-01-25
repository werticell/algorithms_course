#include <iostream>
#include <map>
#include <memory>

class SuffixTree {
 private:
  struct Node {
    std::map<char, std::unique_ptr<Node>> edges_;
    int32_t start_;
    int32_t end_;
    Node* suffix_link_;

    Node* GetNext(char symbol) { return edges_.find(symbol)->second.get(); }
    bool HasTransition(char symbol) {
      return (edges_.find(symbol) != edges_.end());
    }
    void InsertEdge(char symbol, Node* vertex) {
      auto it = edges_.find(symbol);
      // if we have this transition already, then rewrite it
      if (it != edges_.end()) {
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

  void DFS(Node* node, int32_t& dfs_order, int32_t parent_order,
           int32_t first_string_size) const;
  void PrintNode(Node* node, int32_t first_string_size,
                 int32_t parent_order) const;

  void Split(Node* next, int32_t pos_on_edge, int32_t pos,
             Node*& last_created_node);

 public:
  explicit SuffixTree(std::string text);
  void ExtendWithSymbol(int32_t pos);
  void ToString(int32_t) const;
  void MoveActivePoint(int32_t pos);
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
      // if we don't have a transition from our explicit vertex using new
      // symbol, than we should create it
      ++tree_size_;

      Node* new_leaf = new Node(pos, text_.size() - 1);
      active_point_.node_->InsertEdge(text_[active_point_.edge_], new_leaf);
      CreateSuffixLink(last_created_node, active_point_.node_);

    } else {
      Node* next = active_point_.node_->GetNext(text_[active_point_.edge_]);
      if (WalkDownTheEdge(next)) {
        /* if we walk down successfully than we shouldn't decrease the remainder
         * but walk down, pass all the operations below and give it another try
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
    // in the end of step we need to move the active point
    MoveActivePoint(pos);
  }
}

void SuffixTree::PrintNode(SuffixTree::Node* node, int32_t first_string_size,
                           int32_t parent_order) const {
  if (node->start_ < first_string_size) {
    std::cout << parent_order << " 0 " << node->start_ << " "
              << std::min(node->end_ + 1, first_string_size) << "\n";
  } else {
    std::cout << parent_order << " 1 " << node->start_ - first_string_size
              << " " << node->end_ + 1 - first_string_size << "\n";
  }
}

void SuffixTree::DFS(Node* node, int32_t& dfs_order, int32_t parent_order,
                     int32_t first_string_size) const {
  PrintNode(node, first_string_size, parent_order);
  parent_order = dfs_order;
  for (const auto& vertex_edge : node->edges_) {
    DFS(vertex_edge.second.get(), ++dfs_order, parent_order, first_string_size);
  }
}

void SuffixTree::ToString(int32_t first_string_size) const {
  std::cout << tree_size_ << "\n";
  int32_t dfs_order = 0;
  for (const auto& vertex_edge : root_->edges_) {
    DFS(vertex_edge.second.get(), ++dfs_order, 0, first_string_size);
  }
}

int main() {
  std::string s;
  std::string t;
  std::cin >> s >> t;
  SuffixTree my_suffix_tree(s + t);
  my_suffix_tree.ToString(s.size());
  return 0;
}