#pragma once


#include <memory>
#include <vector>
#include <utility>
#include <ranges>

class Node
{
public:
  virtual ~Node() = default;

  void add_child(const std::shared_ptr<Node> &child)
  {
    m_children.emplace_back(child);
  }
  bool remove_child(const std::shared_ptr<Node> &child)
  {
    return std::erase(m_children, child);
  }
  template<typename Pred>
  bool remove_child_if(Pred pred)
  {
    return std::erase_if(m_children, pred);
  }
  void remove_child(size_t index)
  {
    if (m_children.size() > index && index >= 0)
      m_children.erase(m_children.begin() + index);
  }

  template<typename Derived>
  std::vector<std::shared_ptr<Derived>> get_children_of_type() const
  {
    std::vector<std::shared_ptr<Derived>> tmp{};
    for (auto &child : m_children) {
      auto casted = std::dynamic_pointer_cast<Derived>(child);
      if (casted) tmp.emplace_back(casted);
    }
    return tmp;
  }
  const auto &get_children() const { return m_children; }
  auto get_children_reverse() const { return std::ranges::reverse_view{ m_children }; }

private:
  std::vector<std::shared_ptr<Node>> m_children{};
};
