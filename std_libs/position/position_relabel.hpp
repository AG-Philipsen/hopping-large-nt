/*
 * Created: 25-02-2015
 * Modified: Wed 25 Feb 2015 13:56:19 CET
 * Author: Jonas R. Glesaaen (jonas@glesaaen.com)
 */

#ifndef POSITION_RELABEL_HPP
#define POSITION_RELABEL_HPP

#include"position_class.hpp"
#include"position_container.hpp"

#include<set>

namespace Position {
namespace Manipulator {

class PositionRelabeler
{
public:

  typedef std::pair<pos::value_type, pos::value_type> relabel_type;
  typedef std::vector<relabel_type> relabel_container;

  void operator() (pos & p, const relabel_type & label)
  {
    std::swap(p[label.first], p[label.second]);
  }

  void operator() (pos & p, const relabel_container & labels)
  {
    for (const auto & label : labels)
      (*this)(p,label);
  }

};

class PositionContainerRelabeler
{
public:

	template <
    class InputIterator,
    typename = EnableIf<
      std::is_same<
        typename std::iterator_traits<InputIterator>::value_type,
        pos
    > >
  >
  void operator() (InputIterator begin, InputIterator end, std::set<pos::size_type> locked = {})
  {
    locked_indices = std::move(locked);
    auto position_relabeler = PositionRelabeler {};

    for (; begin != end; ++begin) {

      auto relabelings = GetPositionRelabels(*begin);

      if (relabelings.empty())
        continue;

      for (auto it = begin; it != end; ++it)
        position_relabeler(*it, relabelings);
    }
  }

private:
  std::set<pos::size_type> locked_indices;

  typedef PositionRelabeler::relabel_type relabel_type;
  typedef PositionRelabeler::relabel_container relabel_container;

  relabel_container GetPositionRelabels(const pos & p)
  {
    auto relabelings = relabel_container {};

    for (auto i = 0u; i < p.size(); ++i) {

      if (p.at(i) == 0 or locked_indices.find(i) != locked_indices.end())
        continue;

      auto j = 0u;
      while (j < i) {
        if (locked_indices.find(j) == locked_indices.end()) {
          relabelings.push_back({j,i});
          break;
        }
        ++j;
      }

      locked_indices.insert(j);
    }

    return relabelings;
  }
  
};

} //Namespace Manipulator
} //Namespace Position

#endif /* POSITION_RELABEL_HPP */
