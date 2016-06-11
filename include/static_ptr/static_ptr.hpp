#pragma once

#include <memory>
#include <type_traits>

namespace static_ptr {

/// Type tag for static_ptr constructor
template <typename>
struct in_place_t {};

namespace _ {

// template specializations for move and copy operations
template <typename T, bool CanMoveConstruct,
          bool CanDefaultConstruct, bool CanMoveAssign>
struct move_constructer_impl {
  static constexpr bool enabled{false};
  static constexpr bool is_noexcept{false};
  static void call(T* lhs, T* rhs) {
    throw std::logic_error("move constructor disabled");
  }
  move_constructer_impl() = default;
  move_constructer_impl(move_constructer_impl&&) = delete;
  move_constructer_impl& operator=(move_constructer_impl&&) = default;
  move_constructer_impl(const move_constructer_impl&) = default;
  move_constructer_impl& operator=(const move_constructer_impl&) = default;
};
template <typename T, bool CanDefaultConstruct, bool CanMoveAssign>
struct move_constructer_impl<T, true, CanDefaultConstruct, CanMoveAssign> {
  static constexpr bool enabled{true};
  static constexpr bool is_noexcept = std::is_nothrow_move_constructible<T>::value;
  static void call(T* lhs, T* rhs) noexcept(is_noexcept) {
    new (lhs) T(std::move(*rhs));
  }
};
// if not move constructible, fall back to default construct + move assign
template <typename T>
struct move_constructer_impl<T, false, true, true> {
  static constexpr bool enabled{true};
  static constexpr bool is_noexcept = std::is_nothrow_default_constructible<T>::value &&
      std::is_nothrow_move_assignable<T>::value;
  static void call(T* lhs, T* rhs) noexcept(is_noexcept) {
    new (lhs) T();
    *lhs = std::move(*rhs);
  }
};

template <typename T, typename DecayT = typename std::decay<T>::type>
using move_constructer = move_constructer_impl<DecayT,
      std::is_move_constructible<DecayT>::value,
      std::is_move_assignable<DecayT>::value,
      std::is_default_constructible<DecayT>::value>;

template <typename T, bool CanMoveAssign, bool CanMoveConstruct>
struct move_assigner_impl {
  static constexpr bool enabled{false};
  static constexpr bool is_noexcept{false};
  static void call(T* lhs, T* rhs) {
    throw std::logic_error("move assignment disabled");
  }
  move_assigner_impl() = default;
  move_assigner_impl(move_assigner_impl&&) = default;
  move_assigner_impl& operator=(move_assigner_impl&&) = delete;
  move_assigner_impl(const move_assigner_impl&) = default;
  move_assigner_impl& operator=(const move_assigner_impl&) = default;
};
template <typename T, bool CanMoveConstruct>
struct move_assigner_impl<T, true, CanMoveConstruct> {
  static constexpr bool enabled{true};
  static constexpr bool is_noexcept = std::is_nothrow_move_assignable<T>::value;
  static void call(T* lhs, T* rhs) noexcept(is_noexcept) {
    *lhs = std::move(*rhs);
  }
};
// if not move assignable, fall back to destruct + move construct
template <typename T>
struct move_assigner_impl<T, false, true> {
  static constexpr bool enabled{true};
  static constexpr bool is_noexcept = std::is_nothrow_destructible<T>::value &&
      std::is_nothrow_move_constructible<T>::value;
  static void call(T* lhs, T* rhs) noexcept(is_noexcept) {
    lhs->~T();
    new (lhs) T(std::move(*rhs));
  }
};

template <typename T, typename DecayT = typename std::decay<T>::type>
using move_assigner = move_assigner_impl<DecayT,
      std::is_move_assignable<DecayT>::value,
      std::is_move_constructible<DecayT>::value>;

template <typename T, bool CanCopyConstruct,
          bool CanDefaultConstruct, bool CanCopyAssign>
struct copy_constructer_impl {
  static constexpr bool enabled{false};
  static constexpr bool is_noexcept{false};
  static void call(T* lhs, const T* rhs) {
    throw std::logic_error("copy constructor disabled");
  }
  copy_constructer_impl() = default;
  copy_constructer_impl(copy_constructer_impl&&) = default;
  copy_constructer_impl& operator=(copy_constructer_impl&&) = default;
  copy_constructer_impl(const copy_constructer_impl&) = delete;
  copy_constructer_impl& operator=(const copy_constructer_impl&) = default;
};
template <typename T, bool CanDefaultConstruct, bool CanCopyAssign>
struct copy_constructer_impl<T, true, CanDefaultConstruct, CanCopyAssign> {
  static constexpr bool enabled{true};
  static constexpr bool is_noexcept = std::is_nothrow_copy_constructible<T>::value;
  static void call(T* lhs, const T* rhs) noexcept(is_noexcept) {
    new (lhs) T(*rhs);
  }
};
// if not copy constructible, fall back to default construct + copy assign
template <typename T>
struct copy_constructer_impl<T, false, true, true> {
  static constexpr bool enabled{true};
  static constexpr bool is_noexcept = std::is_nothrow_default_constructible<T>::value &&
      std::is_nothrow_copy_assignable<T>::value;
  static void call(T* lhs, T* rhs) noexcept(is_noexcept) {
    new (lhs) T();
    *lhs = *rhs;
  }
};

template <typename T, typename DecayT = typename std::decay<T>::type>
using copy_constructer = copy_constructer_impl<DecayT,
      std::is_copy_constructible<DecayT>::value,
      std::is_copy_assignable<DecayT>::value,
      std::is_default_constructible<DecayT>::value>;

template <typename T, bool CanCopyAssign>
struct copy_assigner_impl {
  static constexpr bool enabled{false};
  static constexpr bool is_noexcept{false};
  static void call(T* lhs, const T* rhs) {
    throw std::logic_error("copy assignment disabled");
  }
  copy_assigner_impl() = default;
  copy_assigner_impl(copy_assigner_impl&&) = default;
  copy_assigner_impl& operator=(copy_assigner_impl&&) = default;
  copy_assigner_impl(const copy_assigner_impl&) = default;
  copy_assigner_impl& operator=(const copy_assigner_impl&) = delete;
};
template <typename T>
struct copy_assigner_impl<T, true> {
  static constexpr bool enabled{true};
  static constexpr bool is_noexcept = std::is_nothrow_copy_assignable<T>::value;
  static void call(T* lhs, const T* rhs) noexcept(is_noexcept) {
    *lhs = *rhs;
  }
};

template <typename T, typename DecayT = typename std::decay<T>::type>
using copy_assigner = copy_assigner_impl<DecayT,
      std::is_copy_assignable<DecayT>::value>;

/// wrapper class for any deleted move/copy operations, to be inherited at
/// the same level as basic_static_ptr. this allows static_ptr to conditionally
/// disable operations even though basic_static_ptr provides implementations for
/// all of them
template <typename T>
class deleted_ops : move_constructer<T>, move_assigner<T>,
                    copy_constructer<T>, copy_assigner<T> {};

/// helper template that checks whether a Derived class provides all of the
/// move and copy operations exposed by its Base class. this is required
/// because static_ptr enables these operations based on the Base class
template <typename Base, typename Derived>
class supports_same_ops {
  // evaluates to false if Base supports the operation and Derived does not
  template <template<typename> typename Op>
  struct pred_t {
    static constexpr bool value{!Op<Base>::value || Op<Derived>::value};
  };

  static constexpr bool v1 = pred_t<std::is_default_constructible>::value;
  static constexpr bool n1 = pred_t<std::is_nothrow_default_constructible>::value;
  static constexpr bool v2 = pred_t<std::is_move_constructible>::value;
  static constexpr bool n2 = pred_t<std::is_nothrow_move_constructible>::value;
  static constexpr bool v3 = pred_t<std::is_move_assignable>::value;
  static constexpr bool n3 = pred_t<std::is_nothrow_move_assignable>::value;
  static constexpr bool v4 = pred_t<std::is_copy_constructible>::value;
  static constexpr bool n4 = pred_t<std::is_nothrow_copy_constructible>::value;
  static constexpr bool v5 = pred_t<std::is_copy_assignable>::value;
  static constexpr bool n5 = pred_t<std::is_nothrow_copy_assignable>::value;
  static constexpr bool v6 = pred_t<std::is_destructible>::value;
  static constexpr bool n6 = pred_t<std::is_nothrow_destructible>::value;

 public:
  static constexpr bool value{v1 && v2 && v3 && v4 && v5 && v6 &&
                              n1 && n2 && n3 && n4 && n5 && n6};
};
template <typename T>
class supports_same_ops<T, T> {
 public:
  static constexpr bool value{true};
};

/// determine whether static_ptr<U, S2> is convertible to static_ptr<T, S>
template <typename T, size_t S, typename U, size_t S2>
class is_convertible {
  static constexpr bool v1 = S2 <= S;
  static constexpr bool v2 = std::is_base_of<U, T>::value;
  static constexpr bool v3 = _::supports_same_ops<U, T>::value;
 public:
  static constexpr bool value{v1 && v2 && v3};
};

/// type erasure implementation that operates through a pointer to a
/// function templated on the actual type T
class type_erasure_ops {
 protected:
  enum class operation {
    move_construct,
    move_assign,
    copy_construct,
    copy_assign,
    destruct,
  };

  template <typename T>
  static void void_operate(operation op, void* dst, void* src) {
    auto lhs = static_cast<T*>(dst);
    auto rhs = static_cast<T*>(src);
    switch (op) {
      case operation::move_construct:
        move_constructer<T>::call(lhs, rhs);
        break;
      case operation::move_assign:
        move_assigner<T>::call(lhs, rhs);
        break;
      case operation::copy_construct:
        copy_constructer<T>::call(lhs, rhs);
        break;
      case operation::copy_assign:
        copy_assigner<T>::call(lhs, rhs);
        break;
      case operation::destruct:
        lhs->~T();
        break;
    }
  }

 public:
  using op_fn = void(*)(operation op, void* dst, void* src);

  template <typename T, typename DecayT = typename std::decay<T>::type>
  static constexpr op_fn get_operate() { return void_operate<DecayT>; }

  void move_construct(void* buffer, op_fn& operate,
                      void* other, op_fn& other_op) {
    if (other_op) {
      other_op(operation::move_construct, buffer, other);
      other_op(operation::destruct, other, nullptr);
      std::swap(operate, other_op);
    }
  }

  void move_assign(void* buffer, op_fn& operate,
                   void* other, op_fn& other_op) {
    if (operate) {
      if (operate == other_op) {
        // already constructed and same type T as other
        other_op(operation::move_assign, buffer, other);
        other_op(operation::destruct, other, nullptr);
        other_op = nullptr;
        return;
      }
      operate(operation::destruct, buffer, nullptr);
      operate = nullptr;
    }
    move_construct(buffer, operate, other, other_op);
  }

  void copy_construct(void* buffer, op_fn& operate,
                      const void* other, op_fn other_op) {
    if (other_op) {
      other_op(operation::copy_construct, buffer, const_cast<void*>(other));
      operate = other_op;
    }
  }

  void copy_assign(void* buffer, op_fn& operate,
                   const void* other, op_fn other_op) {
    if (operate) {
      if (operate == other_op) {
        // already constructed and same type T as other
        other_op(operation::copy_assign, buffer, const_cast<void*>(other));
        return;
      }
      operate(operation::destruct, buffer, nullptr);
      operate = nullptr;
    }
    copy_construct(buffer, operate, other, other_op);
  }

  void destruct(void* buffer, op_fn& operate) {
    operate(operation::destruct, buffer, nullptr);
  }
};

template <typename T, size_t S>
class basic_static_ptr : protected type_erasure_ops {
 protected:
  /// static storage for placement new
  typename std::aligned_storage<S, alignof(T)>::type buffer;

  /// function pointer for operations, null while no object is constructed
  op_fn operate{nullptr};

 public:
  basic_static_ptr() = default;
  ~basic_static_ptr() = default;
  explicit basic_static_ptr(op_fn operate) : operate(operate) {}

  // move operations
  basic_static_ptr(basic_static_ptr&& o)
      noexcept(move_constructer<T>::is_noexcept) {
    static_assert(move_constructer<T>::enabled,
                  "must be MoveConstructible");
    move_construct(&buffer, operate, &o.buffer, o.operate);
  }
  basic_static_ptr& operator=(basic_static_ptr&& o)
      noexcept(move_assigner<T>::is_noexcept) {
    static_assert(move_assigner<T>::enabled,
                  "must be MoveAssignable");
    move_assign(&buffer, operate, &o.buffer, o.operate);
    return *this;
  }

  // copy operations
  basic_static_ptr(const basic_static_ptr& o)
      noexcept(copy_constructer<T>::is_noexcept) {
    static_assert(copy_constructer<T>::enabled,
                  "must be CopyConstructible");
    copy_construct(&buffer, operate, &o.buffer, o.operate);
  }
  basic_static_ptr& operator=(const basic_static_ptr& o)
      noexcept(copy_assigner<T>::is_noexcept) {
    static_assert(copy_assigner<T>::enabled,
                  "must be CopyAssignable");
    copy_assign(&buffer, operate, &o.buffer, o.operate);
    return *this;
  }
};

} // namespace _

template <typename T, size_t S = sizeof(T)>
class static_ptr : public _::basic_static_ptr<T, S>, _::deleted_ops<T> {
  static_assert(sizeof(T) <= S, "S is too small for T");

  using Base = _::basic_static_ptr<T, S>;
  using Base::buffer;
  using Base::destruct;
  using Base::operate;

  /// support conversions of type and size
  template <typename U, size_t S2> friend class static_ptr;

 public:
  static_ptr() = default;
  ~static_ptr() {
    if (operate) {
      destruct(&buffer, operate);
    }
  }

  /// initializing constructor
  template <typename U, typename ...Args>
  static_ptr(in_place_t<U>, Args... args) noexcept(std::is_nothrow_constructible<U, Args...>::value)
    : Base(_::type_erasure_ops::get_operate<U>()) {
    static_assert(sizeof(U) <= S,
                  "size of type is larger than static size");
    static_assert(std::is_base_of<T, U>::value,
                  "initializing with incompatible type");
    static_assert(_::supports_same_ops<T, U>::value,
                  "move into basic_static_ptr with incompatible type");
    new (&buffer) U(std::forward<Args>(args)...);
  }

  /// in-place (re)initialization
  template <typename U = T, typename ...Args>
  void emplace(Args... args) noexcept(std::is_nothrow_constructible<U, Args...>::value) {
    static_assert(sizeof(U) <= S,
                  "size of type is larger than static size");
    static_assert(std::is_base_of<T, U>::value,
                  "initializing with incompatible type");
    static_assert(_::supports_same_ops<T, U>::value,
                  "move into basic_static_ptr with incompatible type");
    reset();
    new (&buffer) U(std::forward<Args>(args)...);
    operate = typename Base::template get_operate<U>();
  }

  // converting move operations
  template <typename U, size_t S2,
            typename = typename std::enable_if<
                _::is_convertible<T, S, U, S2>::value>::type>
  static_ptr(static_ptr<U, S2>&& o)
      noexcept(_::move_constructer<U>::is_noexcept &&
               std::is_nothrow_destructible<T>::value) {
    static_assert(S2 <= S,
                  "move into static_ptr with less static size");
    static_assert(std::is_base_of<T, U>::value,
                  "move into static_ptr with incompatible type");
    static_assert(_::supports_same_ops<T, U>::value,
                  "move into static_ptr with incompatible type");
    static_assert(_::move_constructer<T>::enabled,
                  "must be MoveConstructible");
    this->move_construct(&buffer, operate, &o.buffer, o.operate);
  }
  template <typename U, size_t S2,
            typename = typename std::enable_if<
                _::is_convertible<T, S, U, S2>::value>::type>
  static_ptr& operator=(static_ptr<U, S2>&& o)
      noexcept(_::move_assigner<U>::is_noexcept &&
               std::is_nothrow_destructible<T>::value) {
    static_assert(S2 <= S,
                  "move into static_ptr with less static size");
    static_assert(std::is_base_of<T, U>::value,
                  "move into static_ptr with incompatible type");
    static_assert(_::supports_same_ops<T, U>::value,
                  "move into static_ptr with incompatible type");
    static_assert(_::move_assigner<T>::enabled,
                  "must be MoveAssignable");
    this->move_assign(&buffer, operate, &o.buffer, o.operate);
    return *this;
  }

  // converting copy operations
  template <typename U, size_t S2,
            typename = typename std::enable_if<
                _::is_convertible<T, S, U, S2>::value>::type>
  static_ptr(const static_ptr<U, S2>& o)
      noexcept(_::copy_constructer<U>::is_noexcept &&
               std::is_nothrow_destructible<T>::value) {
    static_assert(S2 <= S,
                  "copy into static_ptr with less static size");
    static_assert(std::is_base_of<T, U>::value,
                  "copy into static_ptr with incompatible type");
    static_assert(_::supports_same_ops<T, U>::value,
                  "move into static_ptr with incompatible type");
    static_assert(_::copy_constructer<U>::enabled,
                  "must be CopyConstructible");
    this->copy_construct(&buffer, operate, &o.buffer, o.operate);
  }
  template <typename U, size_t S2,
            typename = typename std::enable_if<
                _::is_convertible<T, S, U, S2>::value>::type>
  static_ptr& operator=(const static_ptr<U, S2>& o)
      noexcept(_::copy_assigner<U>::is_noexcept &&
               std::is_nothrow_destructible<T>::value) {
    static_assert(S2 <= S,
                  "copy into static_ptr with less static size");
    static_assert(std::is_base_of<T, U>::value,
                  "copy into static_ptr with incompatible type");
    static_assert(_::supports_same_ops<T, U>::value,
                  "move into static_ptr with incompatible type");
    static_assert(_::copy_assigner<U>::enabled,
                  "must be CopyAssignable");
    this->copy_assign(&buffer, operate, &o.buffer, o.operate);
    return *this;
  }

  /// destruct an existing instance
  void reset() noexcept(std::is_nothrow_destructible<T>::value) {
    if (operate) {
      destruct(&buffer, operate);
      operate = nullptr;
    }
  }

  /// base pointer accessors
  T* get() noexcept {
    return operate ? reinterpret_cast<T*>(&buffer) : nullptr;
  }
  const T* get() const noexcept {
    return operate ? reinterpret_cast<const T*>(&buffer) : nullptr;
  }

  T& operator*() noexcept { return *get(); }
  const T& operator*() const noexcept { return *get(); }

  T* operator->() noexcept { return get(); }
  const T* operator->() const noexcept { return get(); }

  operator bool() const noexcept { return operate != nullptr; }

  /// member factory function
  /// easier to use through typedef, i.e:
  /// using base_ptr = static_ptr<base, sizeof(derived)>;
  /// auto p = base_ptr::make<derived>();
  template <typename U = T, typename ...Args>
  static static_ptr make(Args... args) {
    return {in_place_t<U>{}, std::forward<Args>(args)...};
  }
};

/// free factory function
template <typename B, size_t S, typename T, typename ...Args>
inline static_ptr<B, S> make_static_ptr(Args... args)
{
  return {in_place_t<T>{}, std::forward<Args>(args)...};
}

} // namespace static_ptr
