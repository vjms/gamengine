#pragma once


#include <memory>
#include <string>
#include <vector>
#include <utility>
#include <ranges>

template<typename T>
class Node
{
public:
  Node(std::string name) : m_name(name) {}


  void add_child(const std::shared_ptr<Node<T>> &child)
  {
    m_children.emplace_back(child);
  }
  bool remove_child(const std::shared_ptr<Node<T>> &child)
  {
    return std::erase(m_children, child);
  }
  template<typename Pred>
  bool remove_child_if(Pred pred)
  {
    return std::erase_if(m_children, pred);
  }
  bool remove_child(size_t index)
  {
    return m_children.size() <= index
           && m_children.erase(m_children.begin() + index);
  }


  const auto &get_children() const { return m_children; }
  auto get_children_reverse() const { return std::ranges::reverse_view{ m_children }; }
  const auto &get_name() const { return m_name; }

private:
  std::string m_name;
  std::vector<std::shared_ptr<Node<T>>> m_children{};
};
