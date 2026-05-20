#ifndef BST_HPP
#define BST_HPP

#include <iostream>
#include <stdexcept>

namespace ali
{
  template< typename Key, typename Value >
  class BST
  {
  private:
    struct Node
    {
      Key key;
      Value value;
      Node * left;
      Node * right;

      Node(const Key & k, const Value & v):
        key(k),
        value(v),
        left(nullptr),
        right(nullptr)
      {}
    };

    Node * root_;

    void clear(Node * node)
    {
      if (!node)
      {
        return;
      }
      clear(node->left);
      clear(node->right);
      delete node;
    }

    Node * copy(Node * node)
    {
      if (!node)
      {
        return nullptr;
      }

      Node * newNode = new Node(node->key, node->value);
      newNode->left = copy(node->left);
      newNode->right = copy(node->right);
      return newNode;
    }

    void insert(Node *& node, const Key & key, const Value & value)
    {
      if (!node)
      {
        node = new Node(key, value);
      }
      else if (key < node->key)
      {
        insert(node->left, key, value);
      }
      else if (key > node->key)
      {
        insert(node->right, key, value);
      }
      else
      {
        node->value = value;
      }
    }

    bool has(Node * node, const Key & key) const
    {
      if (!node)
      {
        return false;
      }
      if (key == node->key)
      {
        return true;
      }
      if (key < node->key)
      {
        return has(node->left, key);
      }
      return has(node->right, key);
    }

    Value & get(Node * node, const Key & key)
    {
      if (!node)
      {
        throw std::logic_error("no key");
      }
      if (key == node->key)
      {
        return node->value;
      }
      if (key < node->key)
      {
        return get(node->left, key);
      }
      return get(node->right, key);
    }

    template< typename Func >
    void for_each(Node * node, Func func) const
    {
      if (!node)
      {
        return;
      }

      for_each(node->left, func);
      func(node->key, node->value);
      for_each(node->right, func);
    }

    void print(Node * node, bool & first) const
    {
      if (!node)
      {
        return;
      }

      print(node->left, first);

      if (!first)
      {
        std::cout << ' ';
      }

      std::cout << node->key << ' ' << node->value;
      first = false;

      print(node->right, first);
    }

  public:
    BST():
      root_(nullptr)
    {}

    BST(const BST & other):
      root_(copy(other.root_))
    {}

    BST & operator=(const BST & other)
    {
      if (this != &other)
      {
        clear(root_);
        root_ = copy(other.root_);
      }
      return *this;
    }

    ~BST()
    {
      clear(root_);
    }

    void insert(const Key & key, const Value & value)
    {
      insert(root_, key, value);
    }

    bool has(const Key & key) const
    {
      return has(root_, key);
    }

    Value & get(const Key & key)
    {
      return get(root_, key);
    }

    bool empty() const
    {
      return root_ == nullptr;
    }

    template< typename Func >
    void for_each(Func func) const
    {
      for_each(root_, func);
    }

    void print() const
    {
      bool first = true;
      print(root_, first);
    }
  };
}

#endif
