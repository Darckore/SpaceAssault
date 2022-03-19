#include "config/conf.hpp"
using namespace engine::config;

TEST(config, t_option)
{
  section s{ "empty"sv };

  option opt{ "dummy"sv, s };
  EXPECT_EQ(opt.name(), "dummy"sv);
  EXPECT_EQ(&opt.parent(), &s);
  EXPECT_EQ(opt.size(), 0);

  opt.add_value(10);
  EXPECT_EQ(opt.size(), 1);

  auto val = opt[0];
  EXPECT_FALSE((val.try_get<value::bool_val>()));
  EXPECT_FALSE((val.try_get<value::float_val>()));
  
  ASSERT_TRUE((val.try_get<value::int_val>()));
  EXPECT_EQ(10, (val.get<value::int_val>()));
}

TEST(config, t_file_bad)
{
  constexpr auto fname  = "derp.txt"sv;

  cfg_file f{ fname };
  EXPECT_FALSE(f);
  EXPECT_TRUE(f.line().empty());
}

TEST(config, t_file_good)
{
  constexpr auto fname = "data/test_set/section.txt"sv;

  /****************************************
  * 
  *      |.section
  *      |  option{ op1 }
  *      |  option{ op2, 1, true }
  *      |
  *      |  option{ after_empty }
  * 
  *****************************************/
  
  cfg_file f{ fname };
  EXPECT_TRUE(f);

  auto line = f.line();
  EXPECT_EQ(line, ".section");
  line = f.line();
  EXPECT_EQ(line, "  option{ op1 }");
  line = f.line();
  EXPECT_EQ(line, "  option{ op2, 1, true }");
  line = f.line();
  EXPECT_EQ(line, "  option{ after_empty }");

  EXPECT_FALSE(f);
}

TEST(config, t_parse)
{
  constexpr auto fname = "data/test_set/parse.txt"sv;

  cfg c{ fname };
  EXPECT_TRUE(c);
}