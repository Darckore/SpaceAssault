#pragma once
#include "core/entry.hpp"

namespace neko_game
{
  class space_assault : public neko::base_game
  {
  public:
    CLASS_SPECIALS_NONE_CUSTOM(space_assault);

    ~space_assault() noexcept;

    space_assault() noexcept;

  private:
    virtual bool load() noexcept override;
    virtual void on_update(time_type dt) noexcept override;
    virtual void on_render() noexcept override;
  };

  game_ptr make_game();
}