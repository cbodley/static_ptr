#include <static_ptr/static_ptr.hpp>
#include <gtest/gtest.h>

template <typename T>
using in_place_t = static_ptr::in_place_t<T>;

// base class with no implicit copy
struct base_no_copy {
  base_no_copy() = default;
  base_no_copy(base_no_copy&&) = default;
  base_no_copy& operator=(base_no_copy&&) = default;
};

using base_no_copy_ptr = static_ptr::static_ptr<base_no_copy>;

struct derived_no_copy : base_no_copy {};

TEST(PtrNoCopy, MoveAssignable)
{
  base_no_copy_ptr a{in_place_t<derived_no_copy>{}};
  base_no_copy_ptr b;
  b = std::move(a);
  b = base_no_copy_ptr::make<derived_no_copy>();
}

TEST(PtrNoCopy, MoveConstructible)
{
  base_no_copy_ptr a{in_place_t<derived_no_copy>{}};
  base_no_copy_ptr b{std::move(a)};
}

// base class with no implicit copy constructor
struct base_no_copy_construct {
  base_no_copy_construct() = default;
  base_no_copy_construct& operator=(const base_no_copy_construct&) = default;
  base_no_copy_construct(base_no_copy_construct&&) = default;
  base_no_copy_construct& operator=(base_no_copy_construct&&) = default;
};

using base_no_copy_construct_ptr = static_ptr::static_ptr<base_no_copy_construct>;

struct derived_no_copy_construct : base_no_copy_construct {};

TEST(PtrNoCopyConstruct, CopyAssignable)
{
  base_no_copy_construct_ptr a{in_place_t<derived_no_copy_construct>{}};
  base_no_copy_construct_ptr b;
  b = a;
  b = std::move(a);
  b = base_no_copy_construct_ptr::make<derived_no_copy_construct>();
}

TEST(PtrNoCopyConstruct, MoveAssignable)
{
  base_no_copy_construct_ptr a{in_place_t<derived_no_copy_construct>{}};
  base_no_copy_construct_ptr b;
  b = std::move(a);
  b = base_no_copy_construct_ptr::make<derived_no_copy_construct>();
}

TEST(PtrNoCopyConstruct, MoveConstructible)
{
  base_no_copy_construct_ptr a{in_place_t<derived_no_copy_construct>{}};
  base_no_copy_construct_ptr b{std::move(a)};
}

// base class with no implicit copy assignment
struct base_no_copy_assign {
  base_no_copy_assign() = default;
  base_no_copy_assign(const base_no_copy_assign&) = default;
  base_no_copy_assign(base_no_copy_assign&&) = default;
  base_no_copy_assign& operator=(base_no_copy_assign&&) = default;
};

using base_no_copy_assign_ptr = static_ptr::static_ptr<base_no_copy_assign>;

struct derived_no_copy_assign : base_no_copy_assign {};

TEST(PtrNoCopyAssign, CopyConstructible)
{
  base_no_copy_assign_ptr a{in_place_t<derived_no_copy_assign>{}};
  base_no_copy_assign_ptr b{a};
  base_no_copy_assign_ptr c{std::move(b)};
}

TEST(PtrNoCopyAssign, MoveAssignable)
{
  base_no_copy_assign_ptr a{in_place_t<derived_no_copy_assign>{}};
  base_no_copy_assign_ptr b;
  b = std::move(a);
  b = base_no_copy_assign_ptr::make<derived_no_copy_assign>();
}

TEST(PtrNoCopyAssign, MoveConstructible)
{
  base_no_copy_assign_ptr a{in_place_t<derived_no_copy_assign>{}};
  base_no_copy_assign_ptr b{std::move(a)};
}

// base class with no implicit move
struct base_no_move {
  base_no_move() = default;
  base_no_move(const base_no_move&) = default;
  base_no_move& operator=(const base_no_move&) = default;
};

using base_no_move_ptr = static_ptr::static_ptr<base_no_move>;

struct derived_no_move : base_no_move {};

TEST(PtrNoMove, CopyAssignable)
{
  base_no_move_ptr a{in_place_t<derived_no_move>{}};
  base_no_move_ptr b;
  b = a;
  b = std::move(a);
  b = base_no_move_ptr::make<derived_no_move>();
}

TEST(PtrNoMove, CopyConstructible)
{
  base_no_move_ptr a{in_place_t<derived_no_move>{}};
  base_no_move_ptr b{a};
  base_no_move_ptr c{std::move(b)};
}

// base class with move deleted
struct base_deleted_move {
  base_deleted_move() = default;
  base_deleted_move(const base_deleted_move&) = default;
  base_deleted_move& operator=(const base_deleted_move&) = default;
  base_deleted_move(base_deleted_move&&) = delete;
  base_deleted_move& operator=(base_deleted_move&&) = delete;
};

using base_deleted_move_ptr = static_ptr::static_ptr<base_deleted_move>;

struct derived_deleted_move : base_deleted_move {};

TEST(PtrDeletedMove, CopyAssignable)
{
  base_deleted_move_ptr a{in_place_t<derived_deleted_move>{}};
  base_deleted_move_ptr b;
  b = a;
#if 0
  ASSERT_FALSE(std::is_move_assignable<base_deleted_move_ptr>::value);
  b = std::move(a);
  b = base_deleted_move_ptr::make<derived_deleted_move>();
#endif
}

TEST(PtrDeletedMove, CopyConstructible)
{
  base_deleted_move_ptr a{in_place_t<derived_deleted_move>{}};
  base_deleted_move_ptr b{a};
#if 0
  ASSERT_FALSE(std::is_move_constructible<base_deleted_move_ptr>::value);
  base_deleted_move_ptr c{std::move(b)};
#endif
}

// base class with no implicit move constructor
struct base_no_move_construct {
  base_no_move_construct() = default;
  base_no_move_construct(const base_no_move_construct&) = default;
  base_no_move_construct& operator=(const base_no_move_construct&) = default;
  base_no_move_construct& operator=(base_no_move_construct&&) = default;
};

using base_no_move_construct_ptr = static_ptr::static_ptr<base_no_move_construct>;

struct derived_no_move_construct : base_no_move_construct {};

TEST(PtrNoMoveConstruct, CopyAssignable)
{
  base_no_move_construct_ptr a{in_place_t<derived_no_move_construct>{}};
  base_no_move_construct_ptr b;
  b = a;
  b = std::move(a);
  b = base_no_move_construct_ptr::make<derived_no_move_construct>();
}

TEST(PtrNoMoveConstruct, CopyConstructible)
{
  base_no_move_construct_ptr a{in_place_t<derived_no_move_construct>{}};
  base_no_move_construct_ptr b{a};
  base_no_move_construct_ptr c{std::move(b)};
}

TEST(PtrNoMoveConstruct, MoveAssignable)
{
  base_no_move_construct_ptr a{in_place_t<derived_no_move_construct>{}};
  base_no_move_construct_ptr b;
  b = std::move(a);
  b = base_no_move_construct_ptr::make<derived_no_move_construct>();
}

// base class with move constructor deleted
struct base_deleted_move_construct {
  base_deleted_move_construct() = default;
  base_deleted_move_construct(const base_deleted_move_construct&) = default;
  base_deleted_move_construct& operator=(const base_deleted_move_construct&) = default;
  base_deleted_move_construct(base_deleted_move_construct&&) = delete;
  base_deleted_move_construct& operator=(base_deleted_move_construct&&) = default;
};

using base_deleted_move_construct_ptr = static_ptr::static_ptr<base_deleted_move_construct>;

struct derived_deleted_move_construct : base_deleted_move_construct {};

TEST(PtrDeletedMoveConstruct, CopyAssignable)
{
  base_deleted_move_construct_ptr a{in_place_t<derived_deleted_move_construct>{}};
  base_deleted_move_construct_ptr b;
  b = a;
  b = std::move(a);
  b = base_deleted_move_construct_ptr::make<derived_deleted_move_construct>();
}

TEST(PtrDeletedMoveConstruct, CopyConstructible)
{
  base_deleted_move_construct_ptr a{in_place_t<derived_deleted_move_construct>{}};
  base_deleted_move_construct_ptr b{a};
#if 0
  base_deleted_move_construct_ptr c{std::move(b)};
#endif
}

TEST(PtrDeletedMoveConstruct, MoveAssignable)
{
  base_deleted_move_construct_ptr a{in_place_t<derived_deleted_move_construct>{}};
  base_deleted_move_construct_ptr b;
  b = std::move(a);
  b = base_deleted_move_construct_ptr::make<derived_deleted_move_construct>();
}

// base class with no implicit move assignment
struct base_no_move_assign {
  base_no_move_assign() = default;
  base_no_move_assign(const base_no_move_assign&) = default;
  base_no_move_assign& operator=(const base_no_move_assign&) = default;
  base_no_move_assign(base_no_move_assign&&) = default;
};

using base_no_move_assign_ptr = static_ptr::static_ptr<base_no_move_assign>;

struct derived_no_move_assign : base_no_move_assign {};

TEST(PtrNoMoveAssign, CopyAssignable)
{
  base_no_move_assign_ptr a{in_place_t<derived_no_move_assign>{}};
  base_no_move_assign_ptr b;
  b = a;
  b = std::move(a);
  b = base_no_move_assign_ptr::make<derived_no_move_assign>();
}

TEST(PtrNoMoveAssign, CopyConstructible)
{
  base_no_move_assign_ptr a{in_place_t<derived_no_move_assign>{}};
  base_no_move_assign_ptr b{a};
  base_no_move_assign_ptr c{std::move(b)};
}

TEST(PtrNoMoveAssign, MoveConstructible)
{
  base_no_move_assign_ptr a{in_place_t<derived_no_move_assign>{}};
  base_no_move_assign_ptr b{std::move(a)};
}

// base class with move assignment deleted
struct base_deleted_move_assign {
  base_deleted_move_assign() = default;
  base_deleted_move_assign(const base_deleted_move_assign&) = default;
  base_deleted_move_assign& operator=(const base_deleted_move_assign&) = default;
  base_deleted_move_assign(base_deleted_move_assign&&) = default;
  base_deleted_move_assign& operator=(base_deleted_move_assign&&) = delete;
};

using base_deleted_move_assign_ptr = static_ptr::static_ptr<base_deleted_move_assign>;

struct derived_deleted_move_assign : base_deleted_move_assign {};

TEST(PtrDeletedMoveAssign, CopyAssignable)
{
  base_deleted_move_assign_ptr a{in_place_t<derived_deleted_move_assign>{}};
  base_deleted_move_assign_ptr b;
  b = a;
#if 0
  b = std::move(a);
  b = base_deleted_move_assign_ptr::make<derived_deleted_move_assign>();
#endif
}

TEST(PtrDeletedMoveAssign, CopyConstructible)
{
  base_deleted_move_assign_ptr a{in_place_t<derived_deleted_move_assign>{}};
  base_deleted_move_assign_ptr b{a};
  base_deleted_move_assign_ptr c{std::move(b)};
}

TEST(PtrDeletedMoveAssign, MoveConstructible)
{
  base_deleted_move_assign_ptr a{in_place_t<derived_deleted_move_assign>{}};
  base_deleted_move_assign_ptr b{std::move(a)};
}
