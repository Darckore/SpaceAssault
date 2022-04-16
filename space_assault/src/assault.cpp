#include "assault/assault.hpp"

namespace neko_game
{
  // Special members

  space_assault::~space_assault() noexcept = default;

  space_assault::space_assault() noexcept
  {
    run();
  }

  // Private members

  bool space_assault::load() noexcept
  {
    return true;
  }
  void space_assault::on_update(time_type dt) noexcept
  {
    utils::unused(dt);
  }
  void space_assault::on_render() noexcept
  {

  }

  game_ptr make_game()
  {
    return std::make_unique<space_assault>();
  }
}