#include "config/conf.hpp"
using engine::config::cfg;
using engine::config::cfg_file;

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

}