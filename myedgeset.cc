#include "myedgeset.h"

std::pair<EdgeSet::const_iterator, bool> EdgeSet::insert(value_type value) {
  const_iterator itr;
  auto s = get_set();

  if (!s) {
    for (int i = 0; i < kInline; ++i) {
      if (ptrs_[i] == value) {
        itr.array_iter_ = &ptrs_[i];
        return std::make_pair(itr, false);
      }
    }

    for (int i = 0; i < kInline; ++i) {
      if (ptrs_[i] == nullptr) {
        ptrs_[i] = value;
        itr.array_iter_ = &ptrs_[i];
        return std::make_pair(itr, true);
      }
    }

    s = new std::set<const Edge*>;
    for (int i = 0; i < kInline; ++i) {
      s->insert(static_cast<const Edge*>(ptrs_[i]));
    }

    ptrs_[0] = this;
    ptrs_[1] = s;
  }

  auto p = s->insert(value);
  itr.tree_iter_ = p.first;
  return std::make_pair(itr, p.second);
}

EdgeSet::size_type EdgeSet::erase(key_type key) {
  auto s = get_set();
  if (!s) {
    for (int i = 0; i < kInline; ++i) {
      if (ptrs_[i] == key) {
        size_type n = size();
        ptrs_[i] = ptrs_[n - 1];
        ptrs_[n - 1] = nullptr;
        return 1;
      }
    }

    return 0;
  } else {
    return s->erase(key);
  }
}