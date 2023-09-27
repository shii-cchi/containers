#include <gtest/gtest.h>

#include "../red_black_tree/red_black_tree.h"
#include "../red_black_tree/red_black_tree.tpp"

TEST(RedBlackTree, Constructors_1) {
  s21::RedBlackTree<int> tree_1;
  tree_1.Insert(2);
  tree_1 = tree_1;

  s21::RedBlackTree<int> tree_2;
  tree_2 = tree_1;
  EXPECT_TRUE(tree_2.GetSize() == 1);
  EXPECT_NE(tree_2.Find(2), tree_2.End());

  s21::RedBlackTree<int> tree_3;
  tree_2 = tree_3;
  EXPECT_TRUE(tree_2.GetSize() == 0);
  EXPECT_EQ(tree_2.Find(2), tree_2.End());
}

TEST(RedBlackTree, Constructors_2) {
  s21::RedBlackTree<int> tree_1;
  tree_1.Insert(2);

  s21::RedBlackTree<int> tree_2;
  tree_2 = std::move(tree_1);
  EXPECT_TRUE(tree_2.GetSize() == 1);
  EXPECT_NE(tree_2.Find(2), tree_2.End());
}

TEST(RedBlackTree, Constructors_3) {
  s21::RedBlackTree<int> tree_1;
  tree_1.Insert(2);
  tree_1.Insert(1);
  tree_1.Insert(4);
  tree_1.Insert(5);

  s21::RedBlackTree<int> tree_2 = tree_1;
  EXPECT_TRUE(tree_2.GetSize() == 4);
  EXPECT_NE(tree_2.Find(2), tree_2.End());
  EXPECT_NE(tree_2.Find(1), tree_2.End());
  EXPECT_NE(tree_2.Find(4), tree_2.End());
  EXPECT_NE(tree_2.Find(5), tree_2.End());
}

TEST(RedBlackTree, Constructors_4) {
  s21::RedBlackTree<int> tree_1;
  tree_1.Insert(2);

  s21::RedBlackTree<int> tree_2 = std::move(tree_1);

  EXPECT_TRUE(tree_2.GetSize() == 1);
  EXPECT_NE(tree_2.Find(2), tree_2.End());
}

TEST(RedBlackTree, Iterator) {
  s21::RedBlackTree<int> tree;
  for (int i = 5; i >= 0; --i) {
    tree.Insert(i);
  }

  int expectedValue = 0;
  for (auto it = tree.Begin(); it != tree.End(); it++) {
    EXPECT_EQ(*it, expectedValue);
    ++expectedValue;
  }

  expectedValue = 5;
  for (auto it = tree.End(); it != tree.Begin(); it--) {
    if (it != tree.End()) {
      EXPECT_EQ(*it, expectedValue);
      --expectedValue;
    }
  }
}

TEST(RedBlackTree, IteratorConst) {
  s21::RedBlackTree<int> tree;
  for (int i = 5; i >= 0; --i) {
    tree.Insert(i);
  }

  const s21::RedBlackTree<int> &const_tree = tree;

  int expectedValue = 0;
  for (auto it = const_tree.Begin(); it != const_tree.End(); it++) {
    EXPECT_EQ(*it, expectedValue);
    ++expectedValue;
  }

  expectedValue = 5;
  for (auto it = const_tree.End(); it != const_tree.Begin(); it--) {
    if (it == const_tree.End()) {
      continue;
    } else {
      EXPECT_EQ(*it, expectedValue);
      --expectedValue;
    }
  }
}

TEST(RedBlackTree, isEmpty_GetSize) {
  s21::RedBlackTree<int> tree;
  EXPECT_TRUE(tree.isEmpty());
  EXPECT_TRUE(tree.GetSize() == 0);
  tree.Insert(2);
  EXPECT_FALSE(tree.isEmpty());
  EXPECT_TRUE(tree.GetSize() == 1);
}

TEST(RedBlackTree, Insert_Find) {
  s21::RedBlackTree<int> tree;
  tree.Insert(2);
  tree.Insert(5);
  tree.Insert(3);
  tree.Insert(5);
  EXPECT_TRUE(tree.GetSize() == 3);
  tree.Insert(1);
  tree.Insert(6);
  EXPECT_FALSE(tree.isEmpty());
  EXPECT_TRUE(tree.GetSize() == 5);
  EXPECT_NE(tree.Find(3), tree.End());
  EXPECT_NE(tree.Find(5), tree.End());
  EXPECT_EQ(tree.Find(7), tree.End());
}

TEST(RedBlackTree, Merge_1) {
  s21::RedBlackTree<int> tree_1;
  tree_1.Insert(2);
  tree_1.Insert(5);
  s21::RedBlackTree<int> tree_2;
  tree_2.Insert(2);
  tree_2.Insert(5);
  tree_1.Merge(tree_2);
  EXPECT_EQ(tree_1.GetSize(), 2);
  EXPECT_EQ(tree_2.GetSize(), 2);
  EXPECT_NE(tree_1.Find(2), tree_1.End());
  EXPECT_NE(tree_1.Find(5), tree_1.End());
  EXPECT_NE(tree_2.Find(2), tree_2.End());
  EXPECT_NE(tree_2.Find(5), tree_2.End());
}

TEST(RedBlackTree, Merge_2) {
  s21::RedBlackTree<int> tree_1;
  tree_1.Insert(2);
  tree_1.Insert(5);
  s21::RedBlackTree<int> tree_2;
  tree_2.Insert(1);
  tree_2.Insert(4);
  tree_1.Merge(tree_2);
  EXPECT_EQ(tree_1.GetSize(), 4);
  EXPECT_EQ(tree_2.GetSize(), 0);
  EXPECT_NE(tree_1.Find(1), tree_1.End());
  EXPECT_NE(tree_1.Find(2), tree_1.End());
  EXPECT_NE(tree_1.Find(4), tree_1.End());
  EXPECT_NE(tree_1.Find(5), tree_1.End());
  EXPECT_EQ(tree_2.Find(1), tree_2.End());
  EXPECT_EQ(tree_2.Find(4), tree_2.End());
}

TEST(RedBlackTree, Erase_1) {
  s21::RedBlackTree<int> tree;
  tree.Insert(2);
  tree.Insert(5);
  tree.Erase(tree.Find(3));
  tree.Erase(tree.Find(2));
  EXPECT_EQ(tree.Find(2), tree.End());
}

TEST(RedBlackTree, Erase_2) {
  s21::RedBlackTree<int> tree;
  tree.Insert(2);
  tree.Insert(5);
  tree.Insert(1);
  tree.Erase(tree.Find(2));
  EXPECT_EQ(tree.Find(2), tree.End());
}

TEST(RedBlackTree, Erase_3) {
  s21::RedBlackTree<int> tree;
  tree.Insert(2);
  tree.Insert(5);
  tree.Insert(1);
  tree.Insert(4);
  tree.Insert(3);
  tree.Insert(7);
  tree.Erase(tree.Find(3));
  EXPECT_EQ(tree.Find(3), tree.End());
  tree.Erase(tree.Find(4));
  EXPECT_EQ(tree.Find(4), tree.End());
  tree.Erase(tree.Find(7));
  EXPECT_EQ(tree.Find(7), tree.End());
  // tree.Erase(tree.Find(1));
  // EXPECT_EQ(tree.Find(1), tree.End());
}

// TEST(RedBlackTree, Merge_2) {

// }

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
