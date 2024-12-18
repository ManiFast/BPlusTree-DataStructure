#include <iostream>
#include <vector>
#include <algorithm>

const int ORDER = 3; // порядок дерева (минимальное количество ключей в узле)

struct Node {
  std::vector<int> keys;
  std::vector<Node*> children;
  bool is_leaf;

  Node(bool is_leaf = false) : is_leaf(is_leaf) {}
};

class BPlusTree {
public:
  Node* root;

  BPlusTree() : root(new Node(true)) {}

  void insert(int key) {
    if (root->keys.size() == (2 * ORDER - 1)) {
      // корень заполнен, создаем новый корень и разделяем старый
      Node* new_root = new Node();
      new_root->children.push_back(root);
      split_child(new_root, 0);
      insert_non_full(new_root, key);
      root = new_root;
    }
    else {
      insert_non_full(root, key);
    }
  }

  void insert_non_full(Node* node, int key) {
    if (node->is_leaf) {
      // листовой узел, просто вставляем ключ
      node->keys.push_back(key);
      std::sort(node->keys.begin(), node->keys.end());
    }
    else {
      // внутренний узел, находим дочерний узел и вставляем ключ
      int i = 0;
      while (i < node->keys.size() && key > node->keys[i]) {
        i++;
      }
      if (node->children[i]->keys.size() == (2 * ORDER - 1)) {
        // дочерний узел заполнен, разделяем его
        split_child(node, i);
        if (key > node->keys[i]) {
          i++;
        }
      }
      insert_non_full(node->children[i], key);
    }
  }

  void split_child(Node* parent, int index) {
    Node* child = parent->children[index];
    Node* new_child = new Node(child->is_leaf);
    parent->children.insert(parent->children.begin() + index + 1, new_child);

    // разделение ключей и дочерних узлов
    std::vector<int> keys = child->keys;
    std::vector<Node*> children = child->children;
    child->keys.clear();
    child->children.clear();

    int mid = ORDER - 1;
    for (int i = 0; i < mid; i++) {
      child->keys.push_back(keys[i]);
      if (!child->is_leaf) {
        child->children.push_back(children[i]);
      }
    }
    for (int i = mid; i < keys.size(); i++) {
      new_child->keys.push_back(keys[i]);
      if (!child->is_leaf) {
        new_child->children.push_back(children[i]);
      }
    }

    parent->keys.insert(parent->keys.begin() + index, keys[mid]);
  }

  int search(int key) {
    Node* node = root;
    while (!node->is_leaf) {
      int i = 0;
      while (i < node->keys.size() && key > node->keys[i]) {
        i++;
      }
      node = node->children[i];
    }
    for (int i = 0; i < node->keys.size(); i++) {
      if (node->keys[i] == key) {
        return key;
      }
    }
    return -1; // ключ не найден
  }
};

int main() {
  BPlusTree tree;
  tree.insert(5);
  tree.insert(3);
  tree.insert(7);
  tree.insert(2);
  tree.insert(4);
  tree.insert(6);
  tree.insert(8);

  std::cout << "Search result: " << tree.search(3) << std::endl;
  std::cout << "Search result: " << tree.search(9) << std::endl;

  return 0;
}
