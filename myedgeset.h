#include <cstddef>
#include <set>
#include <iostream>

class Edge {};

class EdgeSet {
 public:
  typedef const Edge* value_type;
  typedef const Edge* key_type;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;

  EdgeSet();
  ~EdgeSet();

  class const_iterator;
  typedef const_iterator iterator;
  const_iterator begin() const;
  const_iterator end() const;

  bool empty() const;
  std::pair<const_iterator, bool> insert(value_type value);
  size_type size() const;
  size_type erase(key_type key);
  void clear();

 private:
  static const int kInline = 2;
  const void* ptrs_[kInline];
  unsigned int mutations_ = 0;

  std::set<const Edge*>* get_set() const {
    if (ptrs_[0] == this) {
      return static_cast<std::set<const Edge*>*>(const_cast<void*>(ptrs_[1]));
    }

    return nullptr;
  }
};

class EdgeSet::const_iterator {
 public:
  typedef typename EdgeSet::value_type value_type;
  typedef const typename EdgeSet::value_type& reference;
  typedef const typename EdgeSet::value_type* pointer;
  typedef typename EdgeSet::difference_type difference_type;
  typedef std::forward_iterator_tag iterator_category;
  value_type operator*() const;
  pointer operator->() const;
  bool operator==(const const_iterator& other) const;
  bool operator!=(const const_iterator& other) const {
    return !(*this == other);
  }

  const_iterator& operator++();
  const_iterator operator++(int);

  const_iterator() = default;

 private:
  friend class EdgeSet;
  void const* const* array_iter_ = nullptr;
  typename std::set<const Edge*>::const_iterator tree_iter_;
};

inline EdgeSet::EdgeSet() {
  for (int i = 0; i < kInline; ++i) {
    ptrs_[i] = nullptr;
  }
}

inline EdgeSet::~EdgeSet() { delete get_set(); }

inline EdgeSet::const_iterator::value_type EdgeSet::const_iterator::operator*()
    const {
  if (array_iter_) {
    return static_cast<value_type>(*array_iter_);
  } else {
    return *tree_iter_;
  }
}

inline EdgeSet::const_iterator::pointer EdgeSet::const_iterator::operator->()
    const {
  if (array_iter_) {
    return reinterpret_cast<pointer>(array_iter_);
  } else {
    return tree_iter_.operator->();
  }
}

inline bool EdgeSet::const_iterator::operator==(
    const const_iterator& other) const {
  if ((array_iter_ == nullptr) != (other.array_iter_ == nullptr)) {
    return false;
  }

  if (array_iter_) {
    return array_iter_ == other.array_iter_;
  } else {
    return tree_iter_ == other.tree_iter_;
  }
}

inline EdgeSet::const_iterator& EdgeSet::const_iterator::operator++() {
  if (array_iter_) {
    ++array_iter_;
  } else {
    ++tree_iter_;
  }

  return *this;
}

inline EdgeSet::const_iterator EdgeSet::const_iterator::operator++(int) {
  const_iterator tmp = *this;
  operator++();
  return tmp;
}

inline bool EdgeSet::empty() const { return size() == 0; }

inline EdgeSet::size_type EdgeSet::size() const {
  auto s = get_set();
  if (s) {
    return s->size();
  } else {
    size_type result = 0;
    for (int i = 0; i < kInline; ++i) {
      if (ptrs_[i]) ++result;
    }

    return result;
  }
}

inline void EdgeSet::clear() {
  auto s = get_set();
  delete s;

  for (int i = 0; i < kInline; ++i) {
    ptrs_[i] = nullptr;
  }
}

inline EdgeSet::const_iterator EdgeSet::begin() const {
  const_iterator itr;
  auto s = get_set();
  if (s) {
    itr.tree_iter_ = s->begin();
  } else {
    itr.array_iter_ = &ptrs_[0];
  }

  return itr;
}

inline EdgeSet::const_iterator EdgeSet::end() const {
  const_iterator itr;
  auto s = get_set();
  if (s) {
    itr.tree_iter_ = s->end();
  } else {
    itr.array_iter_ = &ptrs_[size()];
  }
  
  return itr;
}
