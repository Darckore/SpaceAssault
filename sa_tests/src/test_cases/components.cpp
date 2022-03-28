#include "game/component_store.hpp"
#include "game/objects/gameobj.hpp"
#include "game/components/cm_transform.hpp"
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

      virtual void update(time_type) noexcept override{ }
    };

    class cmp2 : public base_component<cmp2>
    {
    public:
      CLASS_SPECIALS_NOCOPY_CUSTOM(cmp2);
      cmp2() noexcept :
        derived_from{ nullptr }
      { }

      virtual void update(time_type) noexcept override{ }
    };

    class cmp3 : public base_component<cmp3>
    {
    public:
      CLASS_SPECIALS_NOCOPY_CUSTOM(cmp3);
      cmp3() noexcept :
        derived_from{ nullptr }
      { }

      virtual void update(time_type) noexcept override{ }
    };
  }

  TEST(cmp, t_collection)
  {
    component_store store;
    game_object fake;
    using utils::vecd2;
    using components::transform;

    auto&& c_tr = store.add<transform>(&fake, vecd2{}, vecd2::axis_norm<1>());
    EXPECT_EQ(c_tr.id(), transform::type_id());
    EXPECT_EQ((fake.get_component<transform>()), &c_tr);
    store.remove<transform>(&fake);
    EXPECT_FALSE((fake.get_component<transform>()));
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

  TEST(cmp, t_transform)
  {
    game_object fake;
    using ct = components::transform;
    using vt = ct::vector_type;
    ct t{ &fake };

    auto&& pos = t.position();
    auto&& dir = t.heading();
    EXPECT_TRUE((pos == vt{}));
    EXPECT_TRUE((dir == vt::axis_norm<0>()));

    constexpr auto newPos = vt{ 1, 0 };
    t.reposition(newPos);
    EXPECT_TRUE((pos == newPos));
    EXPECT_TRUE((dir == vt::axis_norm<0>()));

    constexpr auto delta = vt{ -1, 3 };
    t.shift(delta);
    EXPECT_TRUE((pos == newPos + delta));

    const auto newDir = (vt{} - pos).get_normalised();
    t.head_to(vt{});
    EXPECT_TRUE((dir == newDir));
  }
}