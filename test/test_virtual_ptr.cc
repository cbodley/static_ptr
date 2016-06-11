#include <static_ptr/static_ptr.hpp>
#include <gtest/gtest.h>

struct base {
  virtual ~base() = default;
  virtual const char* get_name() const { return "base"; }
};

struct derived : base {
  const char* get_name() const override { return "derived"; }
};

struct derived2 : base {
  const char* get_name() const override { return "derived2"; }
};

using base_ptr = static_ptr::static_ptr<base, sizeof(derived)>;

TEST(VirtualPtr, Factory)
{
  ASSERT_EQ("base", base_ptr::make()->get_name());
  ASSERT_EQ("derived", base_ptr::make<derived>()->get_name());
  ASSERT_EQ("derived2", base_ptr::make<derived2>()->get_name());
}

TEST(VirtualPtr, MoveAssignable)
{
  auto a = base_ptr::make<derived>();
  ASSERT_EQ("derived", a->get_name());
  base_ptr b;
  b = std::move(a);
  ASSERT_EQ("derived", b->get_name());
  // overwrite
  b = base_ptr::make<derived>();
  ASSERT_EQ("derived", b->get_name());
  // converting overwrite
  b = base_ptr::make<derived2>();
  ASSERT_EQ("derived2", b->get_name());
}

TEST(VirtualPtr, MoveConstructible)
{
  auto a = base_ptr::make<derived>();
  ASSERT_EQ("derived", a->get_name());
  base_ptr b{std::move(a)};
  ASSERT_EQ("derived", b->get_name());
}

TEST(VirtualPtr, CopyAssignable)
{
  auto a = base_ptr::make<derived>();
  ASSERT_EQ("derived", a->get_name());
  base_ptr b;
  b = a;
  ASSERT_EQ("derived", a->get_name());
  ASSERT_EQ("derived", b->get_name());
  // overwrite
  auto a1 = base_ptr::make<derived>();
  b = a1;
  ASSERT_EQ("derived", b->get_name());
  // converting overwrite
  auto a2 = base_ptr::make<derived2>();
  b = a2;
  ASSERT_EQ("derived2", b->get_name());
}

TEST(VirtualPtr, CopyConstructible)
{
  auto a = base_ptr::make<derived>();
  ASSERT_EQ("derived", a->get_name());
  base_ptr b{a};
  ASSERT_EQ("derived", a->get_name());
  ASSERT_EQ("derived", b->get_name());
}
