#include <mywr.hpp>

extern void showcase_address();

// Define a simple class to represent a player in the game.
// In a real-world scenario, this class might be defined in a separate header
// file.
class Player {
public:
  float m_health;
};

void showcase_address() {
  // Define a constant for a really big health value.
  // This value will be used to demonstrate the manipulation of the player's
  // health.
  constexpr float kReallyBigHealth = 99999.F;

  // Declare an instance of the Player class.
  // In a real-world scenario, this instance might be created in the game's
  // memory.
  Player player{};
  player.m_health = 100.F;

  // Simulate a situation where a plugin (DLL) is being injected into the game
  // process.
  // In a real-world scenario, this might be done using a technique like DLL
  // injection. Here, we're using the mywr library to create an address object
  // that points to the player instance.
  mywr::address player_address{&player};

  // Now, we can use the player_address object to safely access and manipulate
  // the player instance's data.
  // In this case, we're using it to set the player's health to a really big
  // value.
  if (player_address) {
    Player* local_player{player_address};
    local_player->m_health = kReallyBigHealth;
  }
}
