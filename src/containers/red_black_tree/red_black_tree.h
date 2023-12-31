#ifndef CONTAINERS_RED_BLACK_TREE_RED_BLACK_TREE_H_
#define CONTAINERS_RED_BLACK_TREE_RED_BLACK_TREE_H_

#include <functional>
#include <limits>

namespace RBtreeMapSet {

enum class Color { kRed, kBlack };

template <typename Key, typename Compare = std::less<Key>>
class RedBlackTree {
 private:
  struct Node;
  struct Iterator;
  struct IteratorConst;

 public:
  using key_type = Key;
  using reference = key_type &;
  using const_reference = const key_type &;
  using iterator = Iterator;
  using const_iterator = IteratorConst;
  using size_type = std::size_t;

  RedBlackTree();
  RedBlackTree(const RedBlackTree &other);
  RedBlackTree(RedBlackTree &&other) noexcept;
  RedBlackTree &operator=(const RedBlackTree &other);
  RedBlackTree &operator=(RedBlackTree &&other) noexcept;
  ~RedBlackTree();

  void RemoveTree();

  iterator Begin() noexcept;
  const_iterator Begin() const noexcept;
  iterator End() noexcept;
  const_iterator End() const noexcept;

  bool isEmpty() const noexcept;
  size_type GetSize() const noexcept;
  size_type GetMaxSize() const noexcept;

  std::pair<iterator, bool> Insert(const key_type key);
  template <typename... Args>
  std::vector<std::pair<iterator, bool>> Insert_many(Args &&...args);
  void Erase(iterator position);
  void SwapTree(RedBlackTree &other) noexcept;
  void Merge(RedBlackTree &other);
  iterator Find(const_reference key) noexcept;

  bool CheckTree() const;

 private:
  void CopyTree(const RedBlackTree &other);
  Node *CopyNode(const Node *node, Node *parent);
  void RemoveNode(Node *node);

  Node *GetRoot();
  const Node *GetRoot() const;
  void SetRoot(Node *node);

  void SetupHead();
  Node *GetMinNode() const;
  Node *GetMaxNode() const;
  void SetMinNode(Node *node);
  void SetMaxNode(Node *node);

  std::pair<iterator, bool> InsertNode(Node *root, Node *new_node);
  void BalanceForInsert(Node *node);
  void RotateLeft(Node *node);
  void RotateRight(Node *node);
  void UpdateSizeAndMinMaxNode(Node *new_node);
  iterator LowerBound(const_reference key);

  Node *ExtractNode(iterator position);
  void UpdateParam(Node *node);
  void ExtractFromTree(Node *node);
  void SwapForErase(Node *node, Node *other);
  void SwapNode(Node *node_1, Node *node_2);
  void UpdateParent(Node *node);
  void BalanceForErase(Node *extracted_node);
  bool isRed(Node *node) const;
  bool IsChildrenBlack(Node *node) const;
  bool IsLeftChildRed(Node *node) const;
  bool IsRightChildRed(Node *node) const;
  Node *SearchMinNode(Node *node) const;
  Node *SearchMaxNode(Node *node) const;

  bool CheckRedNodes(const Node *node) const;
  int CheckBlackHeight(const Node *node) const;

  struct Node {
    Node()
        : parent(nullptr),
          left(this),
          right(this),
          key(key_type{}),
          color(Color::kRed) {}

    Node(const key_type &key)
        : parent(nullptr),
          left(nullptr),
          right(nullptr),
          key(key),
          color(Color::kRed) {}

    Node(key_type key, Color color)
        : parent(nullptr),
          left(nullptr),
          right(nullptr),
          key(key),
          color(color) {}

    void ToDefault() noexcept {
      left = nullptr;
      right = nullptr;
      parent = nullptr;
      color = Color::kRed;
    }

    Node *GetNextNode() const noexcept {
      Node *node = const_cast<Node *>(this);
      if (node->color == Color::kRed &&
          (node->parent == nullptr || node->parent->parent == node)) {
        node = node->left;
      } else if (node->right != nullptr) {
        node = node->right;

        while (node->left != nullptr) {
          node = node->left;
        }
      } else {
        Node *parent = node->parent;

        while (node == parent->right) {
          node = parent;
          parent = parent->parent;
        }
        if (node->right != parent) {
          node = parent;
        }
      }

      return node;
    }

    Node *GetPreviousNode() const noexcept {
      Node *node = const_cast<Node *>(this);

      if (node->color == Color::kRed &&
          (node->parent == nullptr || node->parent->parent == node)) {
        node = node->right;
      } else if (node->left != nullptr) {
        node = node->left;
        while (node->right != nullptr) {
          node = node->right;
        }
      } else {
        Node *parent = node->parent;
        while (node == parent->left) {
          node = parent;
          parent = parent->parent;
        }
        if (node->left != parent) {
          node = parent;
        }
      }
      return node;
    }

    Node *parent;
    Node *left;
    Node *right;
    key_type key;
    Color color;
  };

  struct Iterator {
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = typename RedBlackTree<Key, Compare>::key_type;
    using pointer = value_type *;
    using reference = value_type &;

    Iterator() = delete;

    explicit Iterator(Node *node) : node_(node) {}

    reference operator*() const noexcept { return node_->key; }

    iterator &operator++() noexcept {
      node_ = node_->GetNextNode();
      return *this;
    }

    iterator operator++(int) noexcept {
      iterator tmp{node_};
      ++(*this);
      return tmp;
    }

    iterator &operator--() noexcept {
      node_ = node_->GetPreviousNode();
      return *this;
    }

    iterator operator--(int) noexcept {
      iterator tmp{node_};
      --(*this);
      return tmp;
    }

    bool operator==(const iterator &other) const noexcept {
      return node_ == other.node_;
    }

    bool operator!=(const iterator &other) const noexcept {
      return node_ != other.node_;
    }

    Node *node_;
  };

  struct IteratorConst {
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = typename RedBlackTree<Key, Compare>::key_type;
    using pointer = const value_type *;
    using reference = const value_type &;

    IteratorConst() = delete;

    explicit IteratorConst(const Node *node) : node_(node) {}

    IteratorConst(const iterator &it) : node_(it.node_) {}

    reference operator*() const noexcept { return node_->key; }

    const_iterator &operator++() noexcept {
      node_ = node_->GetNextNode();
      return *this;
    }

    const_iterator operator++(int) noexcept {
      const_iterator tmp{node_};
      ++(*this);
      return tmp;
    }

    const_iterator &operator--() noexcept {
      node_ = node_->GetPreviousNode();
      return *this;
    }

    const_iterator operator--(int) noexcept {
      const_iterator tmp{node_};
      --(*this);
      return tmp;
    }

    friend bool operator==(const const_iterator &it1,
                           const const_iterator &it2) noexcept {
      return it1.node_ == it2.node_;
    }

    friend bool operator!=(const const_iterator &it1,
                           const const_iterator &it2) noexcept {
      return it1.node_ != it2.node_;
    }

    const Node *node_;
  };

  Node *head;
  size_type tree_size;
  Compare cmp;
};

}  // namespace RBtreeMapSet

#include "red_black_tree.tpp"
#endif  // CONTAINERS_RED_BLACK_TREE_RED_BLACK_TREE_H_