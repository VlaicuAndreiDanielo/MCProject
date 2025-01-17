#pragma once
#include <iostream>
using Position = std::pair<float, float>;
using Direction = std::pair<float, float>;

    extern std::string gameserverUrl;
// Rendering and Dimensions
namespace RenderConfig {
    constexpr int kTileSize = 40;          // Size of a single grid tile
    constexpr int kPlayerSize = 40;        // Size of the player
    constexpr int kBulletSize = 25;        // Size of bullets
    constexpr int kWindowWidth = 800;      // Default game window width
    constexpr int kWindowHeight = 600;     // Default game window height
}

// Player Configuration
namespace PlayerConfig {
    constexpr int kDefaultHealth = 100;    // Default health for a player
    constexpr int kNoGameId = -1;          // Indicates no game is assigned
    constexpr int kNoLobbyId = -1;         // Indicates no lobby is assigned
    constexpr float kDefaultPositionX = 0.0f; // Default X position
    constexpr float kDefaultPositionY = 0.0f; // Default Y position
    constexpr float kDefaultDirectionX = 0.0f; // Default X direction
    constexpr float kDefaultDirectionY = 0.0f; // Default Y direction
}

// Timing and Updates
namespace TimingConfig {
    constexpr int kGameLoopIntervalMs = 16; // Game loop interval (milliseconds)
}