#include <mywr.hpp>

using namespace mywr;

extern void showcase_llmo();

void showcase_llmo() {
  // Define constants for full health points and minimum health points.
  // These values are used to compare and modify the player's health.
  constexpr float kFullHealthPoints = 100.F;
  constexpr float kMinimumHealth    = 10.F;

  // Declare a variable to represent the player's current health.
  // In a real-world scenario, this value would be located in the game's memory.
  float player_health = kMinimumHealth;

  // Simulate a situation where the player's health is being managed by a
  // plugin. In a real-world scenario, this might be a separate DLL or shared
  // library. Here, we're using the llmo library to read and write to the
  // player's health memory. The llmo::read function is used to safely read the
  // player's health from memory. We use a range-based if statement (also known
  // as a structured binding) to declare and initialize
  // a variable 'health' within the scope of the if statement.
  // If the llmo::read function returns a valid value, the value is assigned to
  // 'health'. If the llmo::read function returns an error (i.e., if there was
  // an issue reading the memory), the if statement will not execute the block
  // of code inside the curly braces.
  if (auto health = llmo::read<float>(&player_health)) {
    // Check if the player's health is less than or equal to the minimum health.
    if (*health <= kMinimumHealth) {
      // If the player's health is at or below the minimum health,
      // use the llmo::write function to safely write the full health points to
      // the player's health memory.
      llmo::write<float>(&player_health, kFullHealthPoints);
    }
  }
}
