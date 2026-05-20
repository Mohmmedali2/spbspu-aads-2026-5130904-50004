#include <iostream>
#include <fstream>
#include <string>
#include "bst.hpp"

namespace ali
{
  struct Dataset
  {
    std::string name;
    BST< int, std::string > tree;
  };

  Dataset * findDataset(Dataset * data, int count, const std::string & name)
  {
    for (int i = 0; i < count; ++i)
    {
      if (data[i].name == name)
      {
        return &data[i];
      }
    }
    return nullptr;
  }

  void printDataset(const Dataset & dataset)
  {
    if (dataset.tree.empty())
    {
      std::cout << "<EMPTY>\n";
      return;
    }

    std::cout << dataset.name << ' ';
    dataset.tree.print();
    std::cout << '\n';
  }

  BST< int, std::string > makeComplement(
    const BST< int, std::string > & left,
    const BST< int, std::string > & right)
  {
    BST< int, std::string > result;

    left.for_each(
      [&result, &right](int key, const std::string & value)
      {
        if (!right.has(key))
        {
          result.insert(key, value);
        }
      });

    return result;
  }

  BST< int, std::string > makeIntersect(
    const BST< int, std::string > & left,
    const BST< int, std::string > & right)
  {
    BST< int, std::string > result;

    left.for_each(
      [&result, &right](int key, const std::string & value)
      {
        if (right.has(key))
        {
          result.insert(key, value);
        }
      });

    return result;
  }

  BST< int, std::string > makeUnion(
    const BST< int, std::string > & left,
    const BST< int, std::string > & right)
  {
    BST< int, std::string > result = left;

    right.for_each(
      [&result](int key, const std::string & value)
      {
        if (!result.has(key))
        {
          result.insert(key, value);
        }
      });

    return result;
  }
}

int main(int argc, char ** argv)
{
  if (argc != 2)
  {
    std::cerr << "error\n";
    return 1;
  }

  std::ifstream input(argv[1]);

  if (!input)
  {
    std::cerr << "error\n";
    return 1;
  }

  ali::Dataset datasets[100];
  int datasetsCount = 0;

  std::string name;

  while (input >> name && datasetsCount < 100)
  {
    ali::Dataset dataset;
    dataset.name = name;

    int key = 0;
    std::string value;

    while (input.peek() != '\n' && input >> key >> value)
    {
      dataset.tree.insert(key, value);
    }

    datasets[datasetsCount] = dataset;
    ++datasetsCount;

    input.clear();
    input.ignore(10000, '\n');
  }

  std::string command;

  while (std::cin >> command)
  {
    if (command == "print")
    {
      std::string datasetName;
      std::cin >> datasetName;

      ali::Dataset * dataset =
        ali::findDataset(datasets, datasetsCount, datasetName);

      if (!dataset)
      {
        std::cout << "<INVALID COMMAND>\n";
      }
      else
      {
        ali::printDataset(*dataset);
      }
    }
    else if (command == "complement")
    {
      std::string newName;
      std::string leftName;
      std::string rightName;

      std::cin >> newName >> leftName >> rightName;

      ali::Dataset * left =
        ali::findDataset(datasets, datasetsCount, leftName);

      ali::Dataset * right =
        ali::findDataset(datasets, datasetsCount, rightName);

      if (!left || !right || ali::findDataset(datasets, datasetsCount, newName))
      {
        std::cout << "<INVALID COMMAND>\n";
      }
      else
      {
        datasets[datasetsCount].name = newName;
        datasets[datasetsCount].tree =
          ali::makeComplement(left->tree, right->tree);
        ++datasetsCount;
      }
    }
    else if (command == "intersect")
    {
      std::string newName;
      std::string leftName;
      std::string rightName;

      std::cin >> newName >> leftName >> rightName;

      ali::Dataset * left =
        ali::findDataset(datasets, datasetsCount, leftName);

      ali::Dataset * right =
        ali::findDataset(datasets, datasetsCount, rightName);

      if (!left || !right || ali::findDataset(datasets, datasetsCount, newName))
      {
        std::cout << "<INVALID COMMAND>\n";
      }
      else
      {
        datasets[datasetsCount].name = newName;
        datasets[datasetsCount].tree =
          ali::makeIntersect(left->tree, right->tree);
        ++datasetsCount;
      }
    }
    else if (command == "union")
    {
      std::string newName;
      std::string leftName;
      std::string rightName;

      std::cin >> newName >> leftName >> rightName;

      ali::Dataset * left =
        ali::findDataset(datasets, datasetsCount, leftName);

      ali::Dataset * right =
        ali::findDataset(datasets, datasetsCount, rightName);

      if (!left || !right || ali::findDataset(datasets, datasetsCount, newName))
      {
        std::cout << "<INVALID COMMAND>\n";
      }
      else
      {
        datasets[datasetsCount].name = newName;
        datasets[datasetsCount].tree =
          ali::makeUnion(left->tree, right->tree);
        ++datasetsCount;
      }
    }
    else
    {
      std::cout << "<INVALID COMMAND>\n";
    }
  }

  return 0;
}
