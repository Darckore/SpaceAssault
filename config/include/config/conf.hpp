#pragma once
#include "config/file.hpp"

namespace engine::config
{
  class cfg
  {
  public:
    using fname_type = cfg_file::name_type;

  public:
    CLASS_SPECIALS_NODEFAULT_NOCOPY(cfg);

    explicit cfg(fname_type fname) noexcept;

    explicit operator bool() const noexcept;

  private:
    void read() noexcept;

  private:
    cfg_file m_file;
  };
}