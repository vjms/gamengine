#pragma once


#include <memory>
#include <vector>
#include <utility>
#include <ranges>

template<typename T>
class TNode : public T
{
public:
  void add_child(const std::shared_ptr<TNode<T>> &child)
  {
    m_children.emplace_back(child);
  }
  bool remove_child(const std::shared_ptr<TNode<T>> &child)
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

private:
  std::vector<std::shared_ptr<TNode<T>>> m_children{};
};
