#include <static_ptr/static_ptr.hpp>
#include <gtest/gtest.h>

using string_ptr = static_ptr::static_ptr<std::string>;
constexpr auto in_place = static_ptr::in_place_t<std::string>{};

TEST(StringPtr, Factory)
{
  ASSERT_EQ("", *string_ptr::make());
  ASSERT_EQ("hello", *string_ptr::make("hello"));
  ASSERT_EQ("QQ", *string_ptr::make(2, 'Q'));
}

TEST(StringPtr, MoveAssignable)
{
  string_ptr a{in_place, "hello"};
  ASSERT_TRUE(a);
  string_ptr b;
  b = std::move(a);
  ASSERT_FALSE(a);
  ASSERT_TRUE(b);
  ASSERT_EQ("hello", *b);
}

TEST(StringPtr, MoveConstructible)
{
  string_ptr a{in_place, "hello"};
  ASSERT_TRUE(a);
  string_ptr b{std::move(a)};
  ASSERT_FALSE(a);
  ASSERT_TRUE(b);
  ASSERT_EQ("hello", *b);
}

TEST(StringPtr, CopyAssignable)
{
  string_ptr a{in_place, "hello"};
  ASSERT_TRUE(a);
  string_ptr b;
  b = a;
  ASSERT_TRUE(b);
  ASSERT_EQ("hello", *b);
  ASSERT_TRUE(a);
  ASSERT_EQ("hello", *a);
}

TEST(StringPtr, CopyConstructible)
{
  string_ptr a{in_place, "hello"};
  ASSERT_TRUE(a);
  string_ptr b{a};
  ASSERT_TRUE(b);
  ASSERT_EQ("hello", *b);
  ASSERT_TRUE(a);
  ASSERT_EQ("hello", *a);
}
