/**
 * ekuvector, std::vector clone.
 * @author Gerardo Puga
 * */

#pragma once

// Standard library
#include <algorithm>
#include <cstdint>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <type_traits>
#include <utility>

namespace ekustd {

template <class, class Enable = void> struct is_iterator : std::false_type {};

template <typename T>
struct is_iterator<T, typename std::enable_if<
                          std::is_base_of<std::input_iterator_tag, typename std::iterator_traits<T>::iterator_category
                      >::value
                  >::type> : std::true_type {};

template <class Type, class Allocator = std::allocator<Type>> class ekuvector {
public:
  using type = Type;
  using reference = Type &;
  using const_reference = const Type &;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using value_type = Type;
  using allocator_type = Allocator;

  using pointer = Type *;
  using const_pointer = const Type *;

  using iterator = pointer;
  using const_iterator = const_pointer;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  /** @brief Default constructor. Constructs an empty container with a
   *         default-constructed allocator_. */
  ekuvector();

  /** @brief Constructs an empty container with the given allocator alloc. */
  explicit ekuvector(const Allocator &alloc);

  /** @brief Constructs the container with count default-inserted instances of
   *         Type. No copies are made. */
  ekuvector(size_type count);

  /** @brief Constructs the container with count copies of elements with value
   *         value. */
  ekuvector(size_type count, const Type &value,
            const Allocator &alloc = Allocator());

  /** @brief  Constructs the container with the contents of the range [first,
   *          last).
   *
   * This overload only participates in overload resolution if InputIt satisfies
   * LegacyInputIterator, to avoid ambiguity with the overload (3).
   * */
  template <class InputIt>
  ekuvector(
      InputIt first,
      typename std::enable_if<is_iterator<InputIt>::value, InputIt>::type last,
      const Allocator &alloc = Allocator());

  /** @brief Copy constructor. Constructs the container with the copy of the
   *         contents of other.
   *
   * If alloc is not provided, allocator is obtained as if by calling
   * std::allocator_traits<allocator_type>::select_on_container_copy_construction(other.get_allocator()).
   */
  ekuvector(const ekuvector &other);
  ekuvector(const ekuvector &other, const Allocator &alloc);

  /** @brief Move constructor.
   *
   * Constructs the container with the contents of other using move semantics.
   * Allocator is obtained by move-construction from the allocator belonging to
   * other. After the move, other is guaranteed to be empty(). */
  ekuvector(ekuvector &&other);

  /** @brief Allocator-extended move constructor.
   *
   * Using alloc as the allocator for the new container, moving the contents
   * from other; if alloc != other.get_allocator(), this results in an
   * element-wise move. (in that case, other is not guaranteed to be empty after
   * the move) */
  ekuvector(ekuvector &&other, const Allocator &alloc);

  /** @brief Constructs the container with the contents of the initializer list
   *         init.  */
  ekuvector(std::initializer_list<Type> init,
            const Allocator &alloc = Allocator());

  /** @brief Destructor. */
  ~ekuvector();

  /** @brief Copy assignment operator.
   *
   * Replaces the contents with a copy of the contents of other.If
   * std::allocator_traits<allocator_type>::propagate_on_container_copy_assignment::value
   * is true, the target allocator is replaced by a copy of the source
   * allocator. If the target and the source allocators do not compare equal,
   * the target (*this) allocator is used to deallocate the memory, then other's
   * allocator is used to allocate it before copying the elements. (since
   * C++11). */
  ekuvector &operator=(const ekuvector &other);

  /** @brief Move assignment operator.
   *
   * Replaces the contents with those of other using move semantics (i.e. the
   * data in other is moved from other into this container). other is in a valid
   * but unspecified state afterwards. If
   * std::allocator_traits<allocator_type>::propagate_on_container_move_assignment::value
   * is true, the target allocator is replaced by a copy of the source
   * allocator. If it is false and the source and the target allocators do not
   * compare equal, the target cannot take ownership of the source memory and
   * must move-assign each element individually, allocating additional memory
   * using its own allocator as needed. In any case, all elements originally
   * present in *this are either destroyed or replaced by elementwise
   * move-assignment. */
  ekuvector &operator=(ekuvector &&other);

  /** @brief Replaces the contents with those identified by initializer list
   *         ilist. */
  ekuvector &operator=(std::initializer_list<Type> ilist);

  /** @brief Replaces the contents with count copies of value value */
  void assign(size_type count, const Type &value);

  /** @brief Replaces the contents with copies of those in the range [first,
   *         last).
   *
   * The behavior is undefined if either argument is an iterator into *this.
   * This overload only participates in overload resolution if InputIt satisfies
   * LegacyInputIterator. */
  template <class InputIt>
  void assign(
      InputIt first,
      typename std::enable_if<is_iterator<InputIt>::value, InputIt>::type last);

  /** @brief Replaces the contents with the elements from the initializer list
   *         ilist. */
  void assign(std::initializer_list<Type> ilist);

  /** @brief Returns the allocator associated with the container. */
  allocator_type get_allocator() const;

  /** @brief Returns a reference to the element at specified location pos, with
   *         bounds checking.
   *
   * If pos is not within the range of the container, an exception of type
   * std::out_of_range is thrown. */
  reference at(size_type pos);
  const_reference at(size_type pos) const;

  /** @brief Returns a reference to the element at specified location pos. No
   *         bounds checking is performed.  */
  reference operator[](size_type pos);
  const_reference operator[](size_type pos) const;

  /** @brief Returns a reference to the first element in the container. Calling
   *         front on an empty container is undefined. */
  reference front();
  const_reference front() const;

  /** @brief Returns reference to the last element in the container. Calling
   *         back on an empty container is undefined. */
  reference back();
  const_reference back() const;

  /** @brief Returns pointer to the underlying array serving as element storage.
   *
   * The pointer is such that range [data(); data() + size()) is always a valid
   * range, even if the container is empty (data() is not dereferenceable in
   * that case). */
  Type *data() noexcept;
  const Type *data() const noexcept;

  /** @brief Returns an iterator to the first element of the container.
   *
   * If the container is empty, the returned iterator will be equal to end(). */
  iterator begin() noexcept;
  const_iterator begin() const noexcept;
  const_iterator cbegin() const noexcept;

  /** @brief Returns an iterator to the element following the last element of
   *         the container.
   *
   * This element acts as a placeholder; attempting to access it results in
   * undefined behavior. */
  iterator end() noexcept;
  const_iterator end() const noexcept;
  const_iterator cend() const noexcept;

  /** @brief Returns a reverse iterator to the first element of the reversed
   * container.
   *
   * It corresponds to the last element of the non-reversed container. If the
   * container is empty, the returned iterator is equal to rend(). */
  reverse_iterator rbegin() noexcept;
  const_reverse_iterator rbegin() const noexcept;
  const_reverse_iterator crbegin() const noexcept;

  /** @brief Returns a reverse iterator to the element following the last
   *         element of the reversed container.
   *
   * It corresponds to the element preceding the first element of the
   * non-reversed container. This element acts as a placeholder, attempting to
   * access it results in undefined behavior.  */
  reverse_iterator rend() noexcept;
  const_reverse_iterator rend() const noexcept;
  const_reverse_iterator crend() const noexcept;

  /** @brief Checks if the container has no elements, i.e. whether begin() ==
   * end(). */
  bool empty() const noexcept;

  /** @brief Returns the number of elements in the container, i.e.
   * std::distance(begin(), end()). */
  size_type size() const noexcept;

  /** @brief Returns the maximum number of elements the container is able to
   * hold due to system or library implementation limitations, i.e.
   * std::distance(begin(), end()) for the largest container. */
  size_type max_size() const noexcept;

  /** @brief Increase the capacity of the ekuvector to a value that's greater or
   *         equal to new_cap.
   *
   * If new_cap is greater than the current capacity(), new storage is
   * allocated, otherwise the method does nothing. reserve() does not change the
   * size of the ekuvector. If new_cap is greater than capacity(), all
   * iterators,
   * including the past-the-end iterator, and all references to the elements are
   * invalidated. Otherwise, no iterators or references are invalidated. */
  void reserve(size_type new_cap);

  /** @brief Returns the number of elements that the container has currently
   * allocated space for. */
  size_type capacity() const noexcept;

  /** @brief Requests the removal of unused capacity.
   *
   * It is a non-binding request to reduce capacity() to size(). It depends on
   * the implementation whether the request is fulfilled. If reallocation
   * occurs, all iterators, including the past the end iterator, and all
   * references to the elements are invalidated. If no reallocation takes place,
   * no iterators or references are invalidated. */
  void shrink_to_fit();

  /** @brief Erases all elements from the container. After this call, size()
   * returns zero.
   *
   * Invalidates any references, pointers, or iterators referring to contained
   * elements. Any past-the-end iterators are also invalidated. Leaves the
   * capacity() of the ekuvector unchanged (note: the standard's restriction on
   * the
   * changes to capacity is in the specification of ekuvector::reserve)  */
  void clear() noexcept;

  /** @brief Inserts value before pos */
  iterator insert(const_iterator pos, const Type &value);
  iterator insert(const_iterator pos, Type &&value);

  /** @brief Inserts count copies of the value before pos */
  iterator insert(const_iterator pos, size_type count, const Type &value);

  /** @brief inserts elements from range [first, last) before pos.
   *
   * This overload only participates in overload resolution if InputIt qualifies
   * as LegacyInputIterator, to avoid ambiguity with the overload (3). The
   * behavior is undefined if first and last are iterators into *this. */
  template <class InputIt>
  iterator insert(
      const_iterator pos, InputIt first,
      typename std::enable_if<is_iterator<InputIt>::value, InputIt>::type last);

  /** @brief Inserts elements from initializer list ilist before pos. */
  iterator insert(const_iterator pos, std::initializer_list<Type> ilist);

  /** @brief Inserts a new element into the container directly before pos. The
   * element is constructed through std::allocator_traits::construct, which
   * typically uses placement-new to construct the element in-place at a
   * location provided by the container. The arguments args... are forwarded to
   * the constructor as std::forward<Args>(args).... If the new size() is
   * greater than capacity(), all iterators and references are invalidated.
   * Otherwise, only the iterators and references before the insertion point
   * remain valid. The past-the-end iterator is also invalidated. */
  template <class... Args>
  iterator emplace(const_iterator pos, Args &&... args);

  /** @brief Erases the specified elements from the container.
   *
   * Removes the element at pos. Invalidates iterators and references at or
   * after the point of the erase, including the end() iterator. The iterator
   * pos must be valid and dereferenceable. Thus the end() iterator (which is
   * valid, but is not dereferencable) cannot be used as a value for pos. The
   * iterator first does not need to be dereferenceable if first==last: erasing
   * an empty range is a no-op. */
  iterator erase(const_iterator pos);

  /** @brief Erases the specified elements from the container.
   *
   * Removes the elements in the range [first, last). Invalidates iterators and
   * references at or after the point of the erase, including the end()
   * iterator. The iterator pos must be valid and dereferenceable. Thus the
   * end() iterator (which is valid, but is not dereferencable) cannot be used
   * as a value for pos. The iterator first does not need to be dereferenceable
   * if first==last: erasing an empty range is a no-op. */
  iterator erase(const_iterator first, const_iterator last);

  /** @brief Appends the given element value to the end of the container.
 *
 * The new element is initialized as a copy of value. If the new size() is
 * greater than capacity() then all iterators and references (including the
 * past-the-end iterator) are invalidated. Otherwise only the past-the-end
 * iterator is invalidated. */
  void push_back(const Type &value);

  /** @brief Appends the given element value to the end of the container.
   *
   * Value is moved into the new element. If the new size() is greater than
   * capacity() then all iterators and references (including the past-the-end
   * iterator) are invalidated. Otherwise only the past-the-end iterator is
   * invalidated. */
  void push_back(Type &&value);

  /** @brief Appends a new element to the end of the container.
   *
   * The element is constructed through std::allocator_traits::construct, which
   * typically uses placement-new to construct the element in-place at the
   * location provided by the container. The arguments args... are forwarded to
   * the constructor as std::forward<Args>(args).... If the new size() is
   * greater than capacity() then all iterators and references (including the
   * past-the-end iterator) are invalidated. Otherwise only the past-the-end
   * iterator is invalidated.  */
  template <class... Args> void emplace_back(Args &&... args);

  /** @brief Removes the last element of the container.
   *
   * Calling pop_back on an empty container is undefined. Iterators and
   * references to the last element, as well as the end() iterator, are
   * invalidated. */
  void pop_back();

  /** @brief Resizes the container to contain count elements.
   *
   * If the current size is greater than count, the container is reduced to its
   * first count elements. If the current size is less than count, additional
   * elements are appended and initialized with copies of value. If the current
   * size is less than count, additional default-inserted elements are appended
   */
  void resize(size_type count);

  /** @brief Resizes the container to contain count elements.
   *
   * If the current size is greater than count, the container is reduced to its
   * first count elements. If the current size is less than count, additional
   * elements are appended and initialized with copies of value. If the current
   * size is less than count, additional copies of value are appended */
  void resize(size_type count, const value_type &value);

  /** @brief Exchanges the contents of the container with those of other.
   *
   * Does not invoke any move, copy, or swap operations on individual elements.
   * All iterators and references remain valid. The past-the-end iterator is
   * invalidated. */
  void swap(ekuvector &other);

private:
  Allocator allocator_;
  size_t capacity_;
  size_t size_;
  pointer data_;

  void preallocate_capacity(size_type new_cap);
};

template <class Type, class Allocator>
ekuvector<Type, Allocator>::ekuvector() : ekuvector(Allocator()) {}

template <class Type, class Allocator>
ekuvector<Type, Allocator>::ekuvector(const Allocator &alloc)
    : allocator_{alloc}, capacity_{0}, size_{0}, data_{nullptr} {}

template <class Type, class Allocator>
ekuvector<Type, Allocator>::ekuvector(size_type count, const Type &value,
                                      const Allocator &alloc)
    : ekuvector(alloc) {
  if (count) {
    /* preallocate memory space for count elements */
    preallocate_capacity(count);
    /* call a copy constructor on each */
    while (size_ < count) {
      emplace_back(value);
    }
  }
}

template <class Type, class Allocator>
ekuvector<Type, Allocator>::ekuvector(size_type count) : ekuvector() {
  if (count) {
    /* preallocate memory space for count elements */
    preallocate_capacity(count);
    /* call a default constructor on each new item */
    while (size_ < count) {
      emplace_back();
    }
  }
}

template <class Type, class Allocator>
template <class InputIt>
ekuvector<Type, Allocator>::ekuvector(
    InputIt first,
    typename std::enable_if<is_iterator<InputIt>::value, InputIt>::type last,
    const Allocator &alloc)
    : ekuvector(alloc) {
  for (auto it = first; it != last; ++it) {
    push_back(*it);
  }
}

template <class Type, class Allocator>
ekuvector<Type, Allocator>::ekuvector(const ekuvector &other)
    : ekuvector(other, std::allocator_traits<allocator_type>::
                           select_on_container_copy_construction(
                               other.get_allocator())) {}

template <class Type, class Allocator>
ekuvector<Type, Allocator>::ekuvector(const ekuvector &other,
                                      const Allocator &alloc)
    : ekuvector(alloc) {
  /* preallocate memory space for count elements */
  preallocate_capacity(other.size());
  /* call a copy constructor on each item of the original vector*/
  while (size_ < other.size()) {
    emplace_back(other[size_]);
  }
}

template <class Type, class Allocator>
ekuvector<Type, Allocator>::ekuvector(ekuvector &&other) {
  /* move ownership of the contents to destination */
  allocator_ = other.allocator_;
  capacity_ = other.capacity_;
  size_ = other.size_;
  data_ = other.data_;
  /* empty source object, leaving in a safe state */
  other.size_ = 0;
  other.capacity_ = 0;
  other.data_ = nullptr;
}

template <class Type, class Allocator>
ekuvector<Type, Allocator>::ekuvector(ekuvector &&other, const Allocator &alloc)
    : ekuvector(alloc) {
  /* the rest of the move operation changes if source and
     destination have equivalent allocators */
  auto other_alloc = std::allocator_traits<allocator_type>::
      select_on_container_copy_construction(other.get_allocator());

  if (alloc == other_alloc) {
    // relocate assets from the source to the destination
    capacity_ = other.capacity_;
    size_ = other.size_;
    data_ = other.data_;
    // empty source and leave in a safe state
    other.size_ = 0;
    other.capacity_ = 0;
    other.data_ = nullptr;
  } else {
    // preallocate a new memory block and move construct the elements in the
    // original container into the destination
    preallocate_capacity(other.size());
    // move-construct items at the end of this list
    while (size_ < other.size_) {
      emplace_back(std::move(other[size_]));
    }
  }
}

template <class Type, class Allocator>
ekuvector<Type, Allocator>::ekuvector(std::initializer_list<Type> init,
                                      const Allocator &alloc)
    : ekuvector(alloc) {
  /* preallocate memory space */
  preallocate_capacity(init.size());
  /* copy constructo elements at the end of this vector */
  for (auto it = init.begin(); it != init.end(); ++it) {
    emplace_back(*it);
  }
}

template <class Type, class Allocator>
ekuvector<Type, Allocator>::~ekuvector() {
  /* make sure all destructors get called before I release the memory block */
  clear();
  /* release memory */
  if (capacity_) {
    allocator_.deallocate(data_, capacity_);
    capacity_ = 0;
  }
}

template <class Type, class Allocator>
void ekuvector<Type, Allocator>::reserve(size_type new_cap) {
  /* if the new capacity is smaller than the current size, don't do anything */
  if (new_cap <= capacity_) {
    return;
  }

  auto new_data_ptr = allocator_.allocate(new_cap);
  auto new_capacity = new_cap;

  for (size_t index = 0; index < size_; ++index) {
    allocator_.construct(new_data_ptr + index, std::move(*(data_ + index)));
  }

  /* destroy the empty shells left in the old buffer, and then release it */
  clear();
  if (capacity_) {
    allocator_.deallocate(data_, capacity_);
  }

  /* replace with the new block */
  data_ = new_data_ptr;
  capacity_ = new_capacity;
}

template <class Type, class Allocator>
void ekuvector<Type, Allocator>::preallocate_capacity(size_type new_cap) {
  /* if the new capacity is smaller than the current size, don't do anything */
  auto rounded_size = ((new_cap + 1023) / 1024) * 1024;
  reserve(rounded_size);
}

template <class Type, class Allocator>
ekuvector<Type, Allocator> &ekuvector<Type, Allocator>::
operator=(const ekuvector &other) {
  /* clear up the current contents */
  clear();
  preallocate_capacity(other.size());
  /* copy construct the elements in place */
  for (auto it = other.begin(); it != other.end(); ++it) {
    emplace_back(*it);
  }
  return *this;
}

template <class Type, class Allocator>
ekuvector<Type, Allocator> &ekuvector<Type, Allocator>::
operator=(ekuvector &&other) {
  /* clear up the current contents */
  clear();
  preallocate_capacity(other.size());
  /* move-insert the element from the source here*/
  std::move(other.begin(), other.end(), std::back_inserter(*this));
  return *this;
}

template <class Type, class Allocator>
ekuvector<Type, Allocator> &ekuvector<Type, Allocator>::
operator=(std::initializer_list<Type> ilist) {
  /* clear up the current contents */
  clear();
  preallocate_capacity(ilist.size());
  /* copy the contents for the initialization list */
  for (auto it = ilist.begin(); it != ilist.end(); ++it) {
    emplace_back(*it);
  }
  return *this;
}

template <class Type, class Allocator>
void ekuvector<Type, Allocator>::assign(size_type count, const Type &value) {
  if (count) {
    /* allocate memory space for at least count elements */
    preallocate_capacity(count);
    /* call a copy constructor on each */
    for (size_t index = 0; index < count; ++index) {
      /* if the element was previously constructed, destroy it first */
      if (index < size_) {
        allocator_.destroy(data_ + index);
      } else {
        ++size_;
      }
      /* copy construct in place */
      allocator_.construct(data_ + index, value);
    }
  }
}

template <class Type, class Allocator>
template <class InputIt>
void ekuvector<Type, Allocator>::assign(
    InputIt first,
    typename std::enable_if<is_iterator<InputIt>::value, InputIt>::type last) {
  /* call a copy constructor on each */
  size_t index = 0;
  for (auto it = first; it != last; ++it, ++index) {
    /* make sure we have enough capacity */
    if (capacity_ < index + 1) {
      preallocate_capacity(index + 1);
    }
    /* if the element was previously constructed, destroy  it first */
    if (index < size_) {
      allocator_.destroy(data_ + index);
    } else {
      ++size_;
    }
    /* copy construct in place */
    allocator_.construct(data_ + index, *it);
  }
}

template <class Type, class Allocator>
void ekuvector<Type, Allocator>::assign(std::initializer_list<Type> ilist) {
  /* call a copy constructor on each */
  size_t index = 0;
  for (auto it = ilist.begin(); it != ilist.end(); ++it, ++index) {
    /* make sure we have enough capacity */
    if (capacity_ < index + 1) {
      preallocate_capacity(index + 1);
    }
    /* if the element was previously constructed, destroy first */
    if (index < size_) {
      allocator_.destroy(data_ + index);
    } else {
      ++size_;
    }
    /* copy construct in place */
    allocator_.construct(data_ + index, *it);
  }
}

template <class Type, class Allocator>
typename ekuvector<Type, Allocator>::allocator_type
ekuvector<Type, Allocator>::get_allocator() const {
  return allocator_;
}

template <class Type, class Allocator>
typename ekuvector<Type, Allocator>::reference
ekuvector<Type, Allocator>::at(size_type pos) {
  if (pos >= size_) {
    throw std::out_of_range("vector index out of range");
  }
  return *(data_ + pos);
}

template <class Type, class Allocator>
typename ekuvector<Type, Allocator>::const_reference
ekuvector<Type, Allocator>::at(size_type pos) const {
  if (pos >= size_) {
    throw std::out_of_range("vector index out of range");
  }
  return *(data_ + pos);
}

template <class Type, class Allocator>
typename ekuvector<Type, Allocator>::reference ekuvector<Type, Allocator>::
operator[](size_type pos) {
  return *(data_ + pos);
}

template <class Type, class Allocator>
typename ekuvector<Type, Allocator>::const_reference
    ekuvector<Type, Allocator>::operator[](size_type pos) const {
  return *(data_ + pos);
}

template <class Type, class Allocator>
typename ekuvector<Type, Allocator>::reference
ekuvector<Type, Allocator>::front() {
  return *data_;
}

template <class Type, class Allocator>
typename ekuvector<Type, Allocator>::const_reference
ekuvector<Type, Allocator>::front() const {
  return *data_;
}

template <class Type, class Allocator>
typename ekuvector<Type, Allocator>::reference
ekuvector<Type, Allocator>::back() {
  return (*this)[size_ - 1];
}

template <class Type, class Allocator>
typename ekuvector<Type, Allocator>::const_reference
ekuvector<Type, Allocator>::back() const {
  return (*this)[size_ - 1];
}

template <class Type, class Allocator>
Type *ekuvector<Type, Allocator>::data() noexcept {
  return data_;
}

template <class Type, class Allocator>
const Type *ekuvector<Type, Allocator>::data() const noexcept {
  return data_;
}

/* *** */

template <class Type, class Allocator>
typename ekuvector<Type, Allocator>::iterator
ekuvector<Type, Allocator>::begin() noexcept {
  return data_;
}

template <class Type, class Allocator>
typename ekuvector<Type, Allocator>::const_iterator
ekuvector<Type, Allocator>::begin() const noexcept {
  return data_;
}

template <class Type, class Allocator>
typename ekuvector<Type, Allocator>::const_iterator
ekuvector<Type, Allocator>::cbegin() const noexcept {
  return data_;
}

/* *** */

template <class Type, class Allocator>
typename ekuvector<Type, Allocator>::iterator
ekuvector<Type, Allocator>::end() noexcept {
  return data_ + size_;
}

template <class Type, class Allocator>
typename ekuvector<Type, Allocator>::const_iterator
ekuvector<Type, Allocator>::end() const noexcept {
  return data_ + size_;
}

template <class Type, class Allocator>
typename ekuvector<Type, Allocator>::const_iterator
ekuvector<Type, Allocator>::cend() const noexcept {
  return data_ + size_;
}

/* *** */

template <class Type, class Allocator>
typename ekuvector<Type, Allocator>::reverse_iterator
ekuvector<Type, Allocator>::rbegin() noexcept {
  return reverse_iterator(end());
}

template <class Type, class Allocator>
typename ekuvector<Type, Allocator>::const_reverse_iterator
ekuvector<Type, Allocator>::rbegin() const noexcept {
  return const_reverse_iterator(end());
}

template <class Type, class Allocator>
typename ekuvector<Type, Allocator>::const_reverse_iterator
ekuvector<Type, Allocator>::crbegin() const noexcept {
  return const_reverse_iterator(end());
}

/* *** */

template <class Type, class Allocator>
typename ekuvector<Type, Allocator>::reverse_iterator
ekuvector<Type, Allocator>::rend() noexcept {
  return reverse_iterator{begin()};
}

template <class Type, class Allocator>
typename ekuvector<Type, Allocator>::const_reverse_iterator
ekuvector<Type, Allocator>::rend() const noexcept {
  return const_reverse_iterator{begin()};
}

template <class Type, class Allocator>
typename ekuvector<Type, Allocator>::const_reverse_iterator
ekuvector<Type, Allocator>::crend() const noexcept {
  return const_reverse_iterator{begin()};
}

/* *** */

template <class Type, class Allocator>
bool ekuvector<Type, Allocator>::empty() const noexcept {
  return (size_ == 0);
}

template <class Type, class Allocator>
typename ekuvector<Type, Allocator>::size_type
ekuvector<Type, Allocator>::size() const noexcept {
  return size_;
}

template <class Type, class Allocator>
typename ekuvector<Type, Allocator>::size_type
ekuvector<Type, Allocator>::max_size() const noexcept {
  return INT32_MAX;
}

template <class Type, class Allocator>
typename ekuvector<Type, Allocator>::size_type
ekuvector<Type, Allocator>::capacity() const noexcept {
  return capacity_;
}

template <class Type, class Allocator>
void ekuvector<Type, Allocator>::shrink_to_fit() {
  /* the standard allows to ignore this request */
}

template <class Type, class Allocator>
void ekuvector<Type, Allocator>::clear() noexcept {
  while (size()) {
    pop_back();
  }
}

/* *** */

template <class Type, class Allocator>
typename ekuvector<Type, Allocator>::iterator
ekuvector<Type, Allocator>::insert(const_iterator pos, const Type &value) {
  const auto pos_ordinal = empty() ? 0 : std::distance(cbegin(), pos);
  push_back(value); // this can invalidate pos
  auto new_pos = begin() + pos_ordinal;
  auto index = end() - 1;
  while (index != new_pos) {
    std::swap(*index, *(index - 1));
    --index;
  }
  return new_pos;
}

template <class Type, class Allocator>
typename ekuvector<Type, Allocator>::iterator
ekuvector<Type, Allocator>::insert(const_iterator pos, Type &&value) {
  const auto pos_ordinal = empty() ? 0 : std::distance(cbegin(), pos);
  push_back(std::move(value)); // this can invalidate pos
  auto new_pos = begin() + pos_ordinal;
  auto index = end() - 1;
  while (index != new_pos) {
    std::swap(*index, *(index - 1));
    --index;
  }
  return new_pos;
}

template <class Type, class Allocator>
typename ekuvector<Type, Allocator>::iterator
ekuvector<Type, Allocator>::insert(const_iterator pos, size_type count,
                                   const Type &value) {
  const auto &elements_to_insert = count;
  iterator new_pos;
  if (elements_to_insert > 0) {
    const auto pos_ordinal = empty() ? 0 : std::distance(cbegin(), pos);
    preallocate_capacity(size() +
                         elements_to_insert); // this can invalidate pos
    new_pos = begin() + pos_ordinal;
    // move everything past pos towards the end of the vector
    {
      const auto end_mark = new_pos - 1;
      auto dst = end() + elements_to_insert - 1;
      auto src = dst - elements_to_insert;
      while (src != end_mark) {
        allocator_.construct(dst, std::move(*src));
        allocator_.destroy(src); // destruct residual object
        --src;
        --dst;
      }
    }
    // copy construct the new contents in place. The places are currently empty
    {
      auto dst = new_pos;
      for (size_t i = 0; i < count; ++i, ++dst) {
        allocator_.construct(dst, value);
      }
    }
    size_ += elements_to_insert;
  }
  return new_pos;
}

template <class Type, class Allocator>
template <class InputIt>
typename ekuvector<Type, Allocator>::iterator
ekuvector<Type, Allocator>::insert(
    const_iterator pos, InputIt first,
    typename std::enable_if<is_iterator<InputIt>::value, InputIt>::type last) {
  const auto elements_to_insert = std::distance(first, last);
  iterator new_pos;
  if (elements_to_insert > 0) {
    const auto pos_ordinal = empty() ? 0 : std::distance(cbegin(), pos);
    preallocate_capacity(size() +
                         elements_to_insert); // this can invalidate pos
    new_pos = begin() + pos_ordinal;
    // move everything past pos towards the end of the vector
    {
      const auto end_mark = new_pos - 1;
      auto dst = end() + elements_to_insert - 1;
      auto src = dst - elements_to_insert;
      while (src != end_mark) {
        allocator_.construct(dst, std::move(*src));
        allocator_.destroy(src); // destruct residual object
        --src;
        --dst;
      }
    }
    // copy construct the new contents in place. The places are currently empty
    {
      auto dst = new_pos;
      for (auto it = first; it != last; ++it, ++dst) {
        allocator_.construct(dst, *it);
      }
    }
    size_ += elements_to_insert;
  }
  return new_pos;
}

template <class Type, class Allocator>
typename ekuvector<Type, Allocator>::iterator
ekuvector<Type, Allocator>::insert(const_iterator pos,
                                   std::initializer_list<Type> ilist) {
  return insert(pos, ilist.begin(), ilist.end());
}

template <class Type, class Allocator>
template <class... Args>
typename ekuvector<Type, Allocator>::iterator
ekuvector<Type, Allocator>::emplace(const_iterator pos, Args &&... args) {
  const auto pos_ordinal = empty() ? 0 : std::distance(cbegin(), pos);
  emplace_back(std::forward<Args>(args)...);
  const auto new_pos = begin() + pos_ordinal;
  auto index = end() - 1;
  while (index != new_pos) {
    std::swap(*index, *(index - 1));
    --index;
  }
  return new_pos;
}

template <class Type, class Allocator>
typename ekuvector<Type, Allocator>::iterator
ekuvector<Type, Allocator>::erase(const_iterator pos) {
  /* Move the element at pos towards the end of the vector, then remove it */
  iterator non_const_pos = begin() + std::distance(cbegin(), pos);
  auto index = non_const_pos;
  while ((index + 1) != end()) {
    std::swap(*index, *(index + 1));
    ++index;
  }
  pop_back();
  return non_const_pos;
}

template <class Type, class Allocator>
typename ekuvector<Type, Allocator>::iterator
ekuvector<Type, Allocator>::erase(const_iterator first, const_iterator last) {
  /* Move the elements within both iterators towards the end, then remove them */
  auto head = begin() + std::distance(cbegin(), first);
  auto tail = begin() + std::distance(cbegin(), last);
  auto return_it = head;

  while (tail != end()) {
    std::swap(*head, *tail);
    ++head;
    ++tail;
  }

  while (head != end()) {
    pop_back();
  }

  return return_it;
}

template <class Type, class Allocator>
void ekuvector<Type, Allocator>::push_back(const Type &value) {
  /* make sure there's enough storage */
  preallocate_capacity(size_ + 1);
  /* copy construct the new element */
  allocator_.construct(data_ + size_, value);
  ++size_;
}

template <class Type, class Allocator>
void ekuvector<Type, Allocator>::push_back(Type &&value) {
  /* make sure there's enough storage */
  preallocate_capacity(size_ + 1);
  /* move construct the new element */
  allocator_.construct(data_ + size_, std::move(value));
  ++size_;
}

template <class Type, class Allocator>
template <class... Args>
void ekuvector<Type, Allocator>::emplace_back(Args &&... args) {
  /* make sure there's enough storage */
  preallocate_capacity(size_ + 1);
  /* copy construct the new element */
  allocator_.construct(data_ + size_, std::forward<Args>(args)...);
  ++size_;
}

template <class Type, class Allocator>
void ekuvector<Type, Allocator>::pop_back() {
  if (size_) {
    --size_;
    allocator_.destroy(data_ + size_);
  }
}

template <class Type, class Allocator>
void ekuvector<Type, Allocator>::resize(size_type count) {
  resize(count, Type{});
}

template <class Type, class Allocator>
void ekuvector<Type, Allocator>::resize(size_type count,
                                        const value_type &value) {
  while (size_ < count) {
    push_back(value);
  }
  while (size_ > count) {
    pop_back();
  }
}

template <class Type, class Allocator>
void ekuvector<Type, Allocator>::swap(ekuvector &other) {
  std::swap(allocator_, other.allocator_);
  std::swap(data_, other.data_);
  std::swap(size_, other.size_);
  std::swap(capacity_, other.capacity_);
}

/*
 * *** NON MEMBERS ***
 * */

template <class Type, class Alloc>
bool operator==(const ekuvector<Type, Alloc> &lhs,
                const ekuvector<Type, Alloc> &rhs) {
  if (lhs.size() != rhs.size()) {
    return false;
  }

  auto lit = lhs.begin();
  auto rit = rhs.begin();
  for (; lit != lhs.end(); ++lit, ++rit) {
    if (*lit != *rit) {
      return false;
    }
  }

  return true;
}

template <class Type, class Alloc>
bool operator!=(const ekuvector<Type, Alloc> &lhs,
                const ekuvector<Type, Alloc> &rhs) {
  return !(lhs == rhs);
}

template <class Type, class Alloc>
bool operator<(const ekuvector<Type, Alloc> &lhs,
               const ekuvector<Type, Alloc> &rhs) {
  auto lit = lhs.cbegin();
  auto rit = rhs.cbegin();

  while ((lit != lhs.cend()) && (rit != rhs.cend())) {
    if (*lit < *rit) {
      return true;
    }
    if (*rit < *lit) {
      return false;
    }

    ++lit;
    ++rit;
  }
  return (lit == lhs.cend()) && (rit != rhs.cend());
}

template <class Type, class Alloc>
bool operator<=(const ekuvector<Type, Alloc> &lhs,
                const ekuvector<Type, Alloc> &rhs) {
  return ((lhs < rhs) || (lhs == rhs));
}

template <class Type, class Alloc>
bool operator>(const ekuvector<Type, Alloc> &lhs,
               const ekuvector<Type, Alloc> &rhs) {
  return !(lhs <= rhs);
}

template <class Type, class Alloc>
bool operator>=(const ekuvector<Type, Alloc> &lhs,
                const ekuvector<Type, Alloc> &rhs) {
  return !(lhs < rhs);
}

template <class Type, class Alloc>
void swap(ekuvector<Type, Alloc> &lhs, ekuvector<Type, Alloc> &rhs) {
  lhs.swap(rhs);
}

}; // namespace ekustd
