#pragma once
#include <functional>
#include <stdexcept>

template <typename K, typename V>
class BinarySearchTree {
    struct Node {
        K key;
        V value;
        Node* left;
        Node* right;
        Node(K k, V v) : key(k), value(v), left(nullptr), right(nullptr) {}
    };

    Node* root;

    Node* insert(Node* node, K key, V value) {
        if (!node) return new Node(key, value);
        if (key < node->key) {
            node->left = insert(node->left, key, value);
        } else {
            node->right = insert(node->right, key, value);
        }
        return node;
    }

    Node* find(Node* node, K key) const {
        if (!node) return nullptr;
        if (key == node->key) return node;
        return key < node->key ? find(node->left, key) : find(node->right, key);
    }

    void clear(Node* node) {
        if (node) {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }

    void in_order(Node* node, std::function<void(K, V)> visit) {
        if (node) {
            in_order(node->left, visit);
            visit(node->key, node->value);
            in_order(node->right, visit);
        }
    }

public:
    BinarySearchTree() : root(nullptr) {}
    ~BinarySearchTree() { clear(root); }

    void insert(K key, V value) { 
        root = insert(root, key, value); 
    }
    
    bool contains(K key) const { 
        return find(root, key) != nullptr; 
    }
    
    V& get(K key) {
        Node* node = find(root, key);
        if (!node) throw std::out_of_range("Key not found");
        return node->value;
    }

    void clear() {
        clear(root);
        root = nullptr;
    }

    void in_order(std::function<void(K, V)> visit) {
        in_order(root, visit);
    }
};