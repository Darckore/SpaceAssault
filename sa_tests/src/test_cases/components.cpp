#include "game/components/component.hpp"
using namespace engine::world;

namespace engine_tests
{
  namespace detail
  {
    class cmp1 : public base_component<cmp1>
    {
    public:
      CLASS_SPECIALS_NOCOPY_CUSTOM(cmp1);
      cmp1() noexcept :
        derived_from{ nullptr }
      { }
    };

    class cmp2 : public base_component<cmp2>
    {
    public:
      CLASS_SPECIALS_NOCOPY_CUSTOM(cmp2);
      cmp2() noexcept :
        derived_from{ nullptr }
      { }
    };

    class cmp3 : public base_component<cmp3>
    {
    public:
      CLASS_SPECIALS_NOCOPY_CUSTOM(cmp3);
      cmp3() noexcept :
        derived_from{ nullptr }
      { }
    };
  }

  TEST(cmp, t_id)
  {
    detail::cmp1 c1_1;
    detail::cmp2 c2_1;
    const auto id1_1 = c1_1.id();
    const auto id2_1 = c2_1.id();
    const auto ti1_1 = detail::cmp1::type_id();
    const auto ti2_1 = detail::cmp2::type_id();
    EXPECT_NE(id1_1, id2_1);
    EXPECT_EQ(id1_1, ti1_1);
    EXPECT_EQ(id2_1, ti2_1);

    detail::cmp1 c1_2;
    const auto id1_2 = c1_2.id();
    const auto ti1_2 = detail::cmp1::type_id();
    EXPECT_EQ(id1_2, id1_1);
    EXPECT_EQ(ti1_2, ti1_1);

    detail::cmp2 c2_2;
    const auto id2_2 = c2_2.id();
    const auto ti2_2 = detail::cmp2::type_id();
    EXPECT_EQ(id2_2, id2_1);
    EXPECT_EQ(ti2_2, ti2_1);

    detail::cmp3 c3;
    const auto id3 = c3.id();
    const auto ti3 = detail::cmp3::type_id();
    EXPECT_EQ(id3, ti3);
    EXPECT_NE(id3, id1_1);
    EXPECT_NE(id3, id2_1);
    EXPECT_NE(ti3, ti1_1);
    EXPECT_NE(ti3, ti2_1);
  }
}