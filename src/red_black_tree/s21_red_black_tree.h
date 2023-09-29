#ifndef CPP2_S21_CONTAINERS_RED_BLACK_TREE_S21_RED_BLACK_TREE_H_
#define CPP2_S21_CONTAINERS_RED_BLACK_TREE_S21_RED_BLACK_TREE_H_

#include <functional>
#include <limits>

namespace s21 {

enum Color { kRed, kBlack };

template <typename Key, typename Compare = std::less<Key>>
class RedBlackTree {
 public:
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

  void CopyTree(const RedBlackTree &other);
  Node *CopyNode(const Node *node, Node *parent);
  void RemoveNode(Node *node);
  void RemoveTree();
  Node *GetRoot();
  const Node *GetRoot() const;
  void SetRoot(Node *node);
  void SetupHead();
  size_type GetSize() const noexcept;
  void SwapTree(RedBlackTree &other);
  Node *GetMinNode();
  Node *GetMaxNode();
  void SetMinNode(Node *node);
  void SetMaxNode(Node *node);
  bool isEmpty();
  size_type GetMaxSize() const noexcept;
  iterator Insert(const key_type key);
  std::pair<iterator, bool> InsertNode(Node *root, Node *new_node);
  void BalanceTree(Node *node);
  void RotateLeft(Node *node);
  void RotateRight(Node *node);
  void UpdateSizeAndMinMaxNode(Node *new_node);
  iterator Find(const_reference key);
  iterator LowerBound(const_reference key);
  iterator UpperBound(const_reference key);
  iterator Begin() noexcept;
  const_iterator Begin() const noexcept;
  iterator End() noexcept;
  const_iterator End() const noexcept;
  void Merge(RedBlackTree &other);
  void Erase(iterator position) noexcept;
  Node *ExtractNode(iterator position) noexcept;
  void UpdateParam(Node *node);
  void ExtractFromTree(Node *node);
  void SwapForErase(Node *node, Node *other) noexcept;
  void SwapNode(Node *node_1, Node *node_2) noexcept;
  void UpdateParent(Node *node) noexcept;
  void BalanceForErase(Node *extracted_node) noexcept;
  void BalanceRedSibling(Node *sibling, Node *extracted_node,
                         Node *parent) noexcept;
  bool BalanceBlackSiblingWithBlackChildren(Node *sibling, Node *extracted_node,
                                            Node *parent) noexcept;
  void BalanceBlackSiblingWithOneBlackChild(Node *sibling, Node *extracted_node,
                                            Node *parent) noexcept;
  bool isRed(Node *node) noexcept;
  bool IsChildrenBlack(Node *node) const noexcept;
  bool IsLeftChildRed(Node *node) const noexcept;
  bool IsRightChildRed(Node *node) const noexcept;
  Node *SearchMinNode(Node *node) const noexcept;
  Node *SearchMaxNode(Node *node) const noexcept;

  bool CheckTree() const noexcept;
  bool CheckRedNodes(const Node *node) const noexcept;
  int CheckBlackHeight(const Node *node) const noexcept;

  struct Node {
    Node()
        : parent(nullptr),
          left(this),
          right(this),
          key(key_type{}),
          color(kRed) {}

    Node(const key_type &key)
        : parent(nullptr),
          left(nullptr),
          right(nullptr),
          key(key),
          color(kRed) {}

    Node(key_type &&key)
        : parent(nullptr),
          left(nullptr),
          right(nullptr),
          key(std::move(key)),
          color(kRed) {}

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
      color = kRed;
    }

    Node *GetNextNode() const noexcept {
      Node *node = const_cast<Node *>(this);
      if (node->color == kRed &&
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

      if (node->color == kRed &&
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

}  // namespace s21

#endif  // CPP2_S21_CONTAINERS_RED_BLACK_TREE_S21_RED_BLACK_TREE_H_