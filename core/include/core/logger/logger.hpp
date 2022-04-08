#pragma once

namespace engine
{
  class logger
  {
  public:
    enum class level : std::uint8_t
    {
      lOff,
      lFatal,
      lWarning,
      lNote
    };

    using enum level;

    using fmt_type = std::string_view;

  public:
    CLASS_SPECIALS_NONE(logger);

  private:
    static auto& prologue(level lvl) noexcept
    {
      constexpr auto initialSize = 256ull;
      static std::string buf;
      buf.reserve(initialSize);

      constexpr std::array severities {
        "fatal"sv,
        "warning"sv,
        "note"sv
      };

      constexpr auto fmt = "{1:%F, %H:%M:%OS}: [{0:}]: "sv;
      const auto idx = static_cast<std::size_t>(lvl) - 1;
      const auto zt = std::chrono::zoned_time{
        std::chrono::current_zone(),
        std::chrono::system_clock::now()
      };

      std::format_to(std::back_inserter(buf), fmt,
                     severities[idx],
                     zt);

      return buf;
    }

    template <typename ...Args>
    static void message(level lvl, fmt_type fmt, Args&& ...args) noexcept
    {
      if (lvl > m_lvl)
      {
        return;
      }

      auto&& buf = prologue(lvl);
      
      std::format_to(std::back_inserter(buf), fmt, std::forward<Args>(args)...);
      std::cout << buf << std::endl;

      buf.clear();
    }

  public:
    static void set_severity_level(level lvl) noexcept
    {
      m_lvl = lvl;
    }

    template <typename ...Args>
    static void note(fmt_type fmt, Args&& ...args) noexcept
    {
      message(lNote, fmt, std::forward<Args>(args)...);
    }

    template <typename ...Args>
    static void warning(fmt_type fmt, Args&& ...args) noexcept
    {
      message(lWarning, fmt, std::forward<Args>(args)...);
    }

    template <typename ...Args>
    static void error(fmt_type fmt, Args&& ...args) noexcept
    {
      message(lFatal, fmt, std::forward<Args>(args)...);
    }

  private:
    inline static level m_lvl{
    #ifndef NDEBUG
      lNote
    #else
      lFatal
    #endif
    };
  };
}