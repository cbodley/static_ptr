#include <static_ptr/static_ptr.hpp>
#include <gtest/gtest.h>

class base {};
class derived : base { int i; };

using base_ptr = static_ptr::static_ptr<base, sizeof(derived)>;
using derived_ptr = static_ptr::static_ptr<derived, sizeof(derived)>;
using larger_ptr = static_ptr::static_ptr<base, sizeof(derived)+8>;
using string_ptr = static_ptr::static_ptr<std::string>;

TEST(PtrConversion, MoveAssignable)
{
  ASSERT_TRUE((std::is_assignable<larger_ptr, base_ptr&&>::value));
  ASSERT_TRUE((std::is_assignable<base_ptr, base_ptr&&>::value));
  ASSERT_TRUE((std::is_assignable<derived_ptr, base_ptr&&>::value));
  ASSERT_FALSE((std::is_assignable<base_ptr, derived_ptr&&>::value));
  ASSERT_FALSE((std::is_assignable<base_ptr, larger_ptr&&>::value));
  ASSERT_FALSE((std::is_assignable<base_ptr, string_ptr&&>::value));
}

TEST(PtrConversion, MoveConstructible)
{
  ASSERT_TRUE((std::is_constructible<larger_ptr, base_ptr&&>::value));
  ASSERT_TRUE((std::is_constructible<base_ptr, base_ptr&&>::value));
  ASSERT_TRUE((std::is_constructible<derived_ptr, base_ptr&&>::value));
  ASSERT_FALSE((std::is_constructible<base_ptr, derived_ptr&&>::value));
  ASSERT_FALSE((std::is_constructible<base_ptr, larger_ptr&&>::value));
  ASSERT_FALSE((std::is_constructible<base_ptr, string_ptr&&>::value));
}

TEST(PtrConversion, CopyAssignable)
{
  ASSERT_TRUE((std::is_assignable<larger_ptr, const base_ptr&>::value));
  ASSERT_TRUE((std::is_assignable<base_ptr, const base_ptr&>::value));
  ASSERT_TRUE((std::is_assignable<derived_ptr, const base_ptr&>::value));
  ASSERT_FALSE((std::is_assignable<base_ptr, const derived_ptr&>::value));
  ASSERT_FALSE((std::is_assignable<base_ptr, const larger_ptr&>::value));
  ASSERT_FALSE((std::is_assignable<base_ptr, const string_ptr&>::value));
}

TEST(PtrConversion, CopyConstructible)
{
  ASSERT_TRUE((std::is_constructible<larger_ptr, const base_ptr&>::value));
  ASSERT_TRUE((std::is_constructible<base_ptr, const base_ptr&>::value));
  ASSERT_TRUE((std::is_constructible<derived_ptr, const base_ptr&>::value));
  ASSERT_FALSE((std::is_constructible<base_ptr, const derived_ptr&>::value));
  ASSERT_FALSE((std::is_constructible<base_ptr, const larger_ptr&>::value));
  ASSERT_FALSE((std::is_constructible<base_ptr, const string_ptr&>::value));
}
