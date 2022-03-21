#include "config/conf.hpp"
#include "parser/lex.hpp"
using namespace config;
using token = lex::token;

namespace test_detail
{
  void check_token(lex& l, std::string_view str, token::token_id id)
  {
    auto tok = l.next();
    ASSERT_TRUE(tok.is(id));
    ASSERT_EQ(tok.value, str);
  }
}

TEST(conf, t_option)
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

TEST(conf, t_section_add_get)
{
  section s{ "glob"sv };

  constexpr auto name1 = "dummy"sv;
  auto&& dummy = s.add_section(name1);
  EXPECT_TRUE(s.has_section(name1));

  auto&& anotherDummy = s.add_section(name1);
  EXPECT_TRUE(&dummy == &anotherDummy);

  auto dummyLookup = s.get_section(name1);
  EXPECT_TRUE(dummyLookup == &dummy);

  const auto& sref = s;
  auto dummyConstLookup = sref.get_section(name1);
  EXPECT_TRUE(dummyConstLookup == &dummy);

  EXPECT_TRUE(s.is_root());
  EXPECT_FALSE(dummy.is_root());
  
  EXPECT_TRUE(dummy.parent() == &s);
}

TEST(conf, t_option_add_get)
{
  section s{ "glob"sv };

  constexpr auto optName  = "neko"sv;
  constexpr auto intVal   = 10;
  constexpr auto boolVal  = false;
  constexpr auto floatVal = 42.69;
  constexpr auto strVal   = "kitty"sv;
  
  auto&& opt = s.add_option(optName);
  EXPECT_TRUE(s.has_option(optName));

  opt.add_value(intVal);
  opt.add_value(boolVal);
  opt.add_value(floatVal);
  opt.add_value(strVal);

  const auto& optAdd = s.add_option(optName);
  ASSERT_EQ(optAdd.size(), 4);

  auto ip = optAdd[0].try_get<value::int_val>();
  ASSERT_TRUE(ip);
  EXPECT_EQ(*ip, intVal);

  auto bp = optAdd[1].try_get<value::bool_val>();
  ASSERT_TRUE(bp);
  EXPECT_EQ(*bp, boolVal);

  auto fp = optAdd[2].try_get<value::float_val>();
  ASSERT_TRUE(fp);
  EXPECT_DOUBLE_EQ(*fp, floatVal);

  auto sp = optAdd[3].try_get<value::str_val>();
  ASSERT_TRUE(sp);
  EXPECT_EQ(*sp, strVal);

  EXPECT_EQ((opt[0].get<value::int_val>()), intVal);
  EXPECT_EQ((opt[1].get<value::bool_val>()), boolVal);
  EXPECT_DOUBLE_EQ((opt[2].get<value::float_val>()), floatVal);
  EXPECT_EQ((opt[3].get<value::str_val>()), strVal);
}

TEST(conf_file, t_bad)
{
  constexpr auto fname  = "derp.txt"sv;

  cfg_file f{ fname };
  EXPECT_FALSE(f);
  EXPECT_TRUE(f.line().empty());
}

TEST(conf_file, t_good)
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
  ASSERT_TRUE(f);

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

TEST(conf_lex, t_good)
{
  constexpr auto fname = "data/test_set/lex_good.txt"sv;

  // .section { option1 { 1, -42, +6.9, 'string value', true, false } }

  cfg_file f{ fname };
  ASSERT_TRUE(f);

  lex l{ f };

  test_detail::check_token(l, ".section"sv,       token::section);
  test_detail::check_token(l, "{"sv,              token::curlyOpen);
  test_detail::check_token(l, "option1"sv,        token::identifier);
  test_detail::check_token(l, "{"sv,              token::curlyOpen);
  test_detail::check_token(l, "1"sv,              token::intNum);
  test_detail::check_token(l, ","sv,              token::comma);
  test_detail::check_token(l, "-42"sv,            token::intNum);
  test_detail::check_token(l, ","sv,              token::comma);
  test_detail::check_token(l, "+6.9"sv,           token::floatNum);
  test_detail::check_token(l, ","sv,              token::comma);
  test_detail::check_token(l, "'string value'"sv, token::str);
  test_detail::check_token(l, ","sv,              token::comma);
  test_detail::check_token(l, "true"sv,           token::boolTrue);
  test_detail::check_token(l, ","sv,              token::comma);
  test_detail::check_token(l, "false"sv,          token::boolFalse);
  test_detail::check_token(l, "}"sv,              token::curlyClose);
  test_detail::check_token(l, "}"sv,              token::curlyClose);

  EXPECT_TRUE(!l);
}

TEST(conf_lex, t_bad1)
{
  constexpr auto fname = "data/test_set/lex_bad1.txt"sv;

  cfg_file f{ fname };
  ASSERT_TRUE(f);

  lex l{ f };

  test_detail::check_token(l, {}, token::unknown);
  EXPECT_TRUE(!l);
}

TEST(conf_lex, t_bad2)
{
  constexpr auto fname = "data/test_set/lex_bad2.txt"sv;

  cfg_file f{ fname };
  ASSERT_TRUE(f);

  lex l{ f };

  test_detail::check_token(l, ".section"sv, token::section);
  test_detail::check_token(l, {},           token::unknown);
  EXPECT_TRUE(!l);
}

TEST(conf_lex, t_bad3)
{
  constexpr auto fname = "data/test_set/lex_bad3.txt"sv;

  cfg_file f{ fname };
  ASSERT_TRUE(f);

  lex l{ f };

  test_detail::check_token(l, ".section"sv,       token::section);
  test_detail::check_token(l, "{"sv,              token::curlyOpen);
  test_detail::check_token(l, "option"sv,         token::identifier);
  test_detail::check_token(l, "{"sv,              token::curlyOpen);
  test_detail::check_token(l, "1"sv,              token::intNum);
  test_detail::check_token(l, ","sv,              token::comma);
  test_detail::check_token(l, {},                 token::unknown);
  EXPECT_TRUE(!l);
}

TEST(conf_parser, t_parse)
{
  //constexpr auto fname = "data/test_set/parse.txt"sv;

  //cfg c{ fname };
  //EXPECT_TRUE(c);
}