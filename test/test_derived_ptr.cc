#include <static_ptr/static_ptr.hpp>
#include <gtest/gtest.h>

// use boost::intrusive_ptr<int> in both base and derived to detect object
// slicing and validate the copy and move semantics
void intrusive_ptr_add_ref(int* p) { ++*p; }
void intrusive_ptr_release(int* p) { --*p; }

#include <boost/intrusive_ptr.hpp>

using ref = boost::intrusive_ptr<int>;

struct base {
  ref a;
  explicit base(ref a) : a(a) {}
  base() = default;
  base(base&&) = default;
  ~base() = default;
  base& operator=(base&&) = default;
  base(const base&) = default;
  base& operator=(const base&) = default;
};

struct derived : base {
  ref b;
  derived(ref a, ref b) : base(a), b(b) {}
  derived() = default;
  ~derived() = default;
  derived(derived&&) = default;
  derived& operator=(derived&&) = default;
  derived(const derived&) = default;
  derived& operator=(const derived&) = default;
};

struct derived2 : derived {
  using derived::derived;
};

using base_ptr = static_ptr::static_ptr<base, sizeof(derived)>;
// for testing conversions to smaller ptrs
using larger_ptr = static_ptr::static_ptr<base, sizeof(derived)+8>;

template <typename T>
using in_place_t = static_ptr::in_place_t<T>;

TEST(DerivedPtr, MoveAssignable)
{
  int num_base{0};
  int num_derived{0};
  {
    larger_ptr a{in_place_t<derived>{}, ref{&num_base}, ref{&num_derived}};
    ASSERT_EQ(1, num_base);
    ASSERT_EQ(1, num_derived);
    {
      larger_ptr b;
      b = std::move(a);
      ASSERT_EQ(1, num_base);
      ASSERT_EQ(1, num_derived);
      // overwrite
      b = larger_ptr::make<derived>(ref{&num_base}, ref{&num_derived});
      ASSERT_EQ(1, num_base);
      ASSERT_EQ(1, num_derived);
      // converting overwrite from smaller
      b = base_ptr::make<derived2>(ref{&num_base}, ref{&num_derived});
      ASSERT_EQ(1, num_base);
      ASSERT_EQ(1, num_derived);
    }
    ASSERT_EQ(0, num_base);
    ASSERT_EQ(0, num_derived);
  }
  ASSERT_EQ(0, num_base);
  ASSERT_EQ(0, num_derived);
}

TEST(DerivedPtr, MoveConstructible)
{
  ASSERT_TRUE((std::is_constructible<larger_ptr, base_ptr&&>::value));
  ASSERT_TRUE((std::is_constructible<base_ptr, base_ptr&&>::value));
  ASSERT_FALSE((std::is_constructible<base_ptr, larger_ptr&&>::value));
  ASSERT_FALSE((std::is_constructible<base_ptr, std::string&&>::value));
  int num_base{0};
  int num_derived{0};
  {
    larger_ptr a{in_place_t<derived>{}, ref{&num_base}, ref{&num_derived}};
    ASSERT_EQ(1, num_base);
    ASSERT_EQ(1, num_derived);
    {
      larger_ptr b{std::move(a)};
      ASSERT_EQ(1, num_base);
      ASSERT_EQ(1, num_derived);
    }
    ASSERT_EQ(0, num_base);
    ASSERT_EQ(0, num_derived);
    // converting move construct
    {
      larger_ptr b = base_ptr::make<derived>(ref{&num_base}, ref{&num_derived});
      ASSERT_EQ(1, num_base);
      ASSERT_EQ(1, num_derived);
    }
    ASSERT_EQ(0, num_base);
    ASSERT_EQ(0, num_derived);
  }
  ASSERT_EQ(0, num_base);
  ASSERT_EQ(0, num_derived);
}

TEST(DerivedPtr, CopyAssignable)
{
  int num_base{0};
  int num_derived{0};
  {
    larger_ptr a{in_place_t<derived>{}, ref{&num_base}, ref{&num_derived}};
    ASSERT_EQ(1, num_base);
    ASSERT_EQ(1, num_derived);
    {
      larger_ptr b;
      b = a;
      ASSERT_EQ(2, num_base);
      ASSERT_EQ(2, num_derived);
      // overwrite
      auto a1 = larger_ptr::make<derived>(ref{&num_base}, ref{&num_derived});
      b = a1;
      ASSERT_EQ(3, num_base);
      ASSERT_EQ(3, num_derived);
      // converting overwrite from smaller
      auto a2 = base_ptr::make<derived2>(ref{&num_base}, ref{&num_derived});
      b = a2;
      ASSERT_EQ(4, num_base);
      ASSERT_EQ(4, num_derived);
    }
    ASSERT_EQ(1, num_base);
    ASSERT_EQ(1, num_derived);
  }
  ASSERT_EQ(0, num_base);
  ASSERT_EQ(0, num_derived);
}

TEST(DerivedPtr, CopyConstructible)
{
  int num_base{0};
  int num_derived{0};
  {
    larger_ptr a{in_place_t<derived>{}, ref{&num_base}, ref{&num_derived}};
    ASSERT_EQ(1, num_base);
    ASSERT_EQ(1, num_derived);
    {
      larger_ptr b{a};
      ASSERT_EQ(2, num_base);
      ASSERT_EQ(2, num_derived);
    }
    ASSERT_EQ(1, num_base);
    ASSERT_EQ(1, num_derived);
    // converting copy construct
    {
      larger_ptr b = base_ptr::make<derived>(ref{&num_base}, ref{&num_derived});
      ASSERT_EQ(2, num_base);
      ASSERT_EQ(2, num_derived);
    }
    ASSERT_EQ(1, num_base);
    ASSERT_EQ(1, num_derived);
  }
  ASSERT_EQ(0, num_base);
  ASSERT_EQ(0, num_derived);
}
