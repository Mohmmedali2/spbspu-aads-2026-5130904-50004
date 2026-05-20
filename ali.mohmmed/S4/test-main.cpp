#define BOOST_TEST_MODULE S4
#include <boost/test/included/unit_test.hpp>

#include "bst.hpp"

BOOST_AUTO_TEST_CASE(insert_and_get)
{
  ali::BST< int, std::string > tree;

  tree.insert(2, "two");
  tree.insert(1, "one");
  tree.insert(3, "three");

  BOOST_TEST(tree.has(1));
  BOOST_TEST(tree.has(2));
  BOOST_TEST(tree.has(3));

  BOOST_TEST(tree.get(1) == "one");
  BOOST_TEST(tree.get(2) == "two");
  BOOST_TEST(tree.get(3) == "three");
}

BOOST_AUTO_TEST_CASE(replace_value)
{
  ali::BST< int, std::string > tree;

  tree.insert(1, "old");
  tree.insert(1, "new");

  BOOST_TEST(tree.get(1) == "new");
}

BOOST_AUTO_TEST_CASE(copy_constructor)
{
  ali::BST< int, std::string > first;

  first.insert(1, "one");
  first.insert(2, "two");

  ali::BST< int, std::string > second(first);

  BOOST_TEST(second.has(1));
  BOOST_TEST(second.has(2));

  BOOST_TEST(second.get(1) == "one");
  BOOST_TEST(second.get(2) == "two");
}
