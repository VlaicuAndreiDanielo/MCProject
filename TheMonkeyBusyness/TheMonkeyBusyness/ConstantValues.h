#pragma once

using MapPosition = std::pair<int, int>;

// Math Constants
namespace MathConfig {
    constexpr float kPi = 3.14159265358979323846f;
}

// Player Configuration
namespace PlayerConfig {
    constexpr int kDefaultPlayerId = -1;    // Default ID when uninitialized
    constexpr int kPlayerSize = 30;         // Size of the player
    constexpr float kDefaultPositionX = 0.0f;
    constexpr float kDefaultPositionY = 0.0f;
    constexpr const char* kDefaultPlayerName = "Mario";
}

// Weapon Configuration
namespace WeaponConfig {
    constexpr float kBasicSpeed = 250.0f;
    constexpr int kBasicDamage = 10;
    constexpr float kBasicFireRate = 0.4f;  //fire rate of 1 means 1 shot every 1s. Fire rate of 0.1 means 10 shots every 1s
    constexpr int kMaxBasicBullets = 100;

    constexpr float kSpeedPowerupIncreasePercent = 100.0f; // Percent increase
    constexpr float kDamagePowerupIncreasePercent = 100.0f; // Percent increase
}

// Game Configuration
namespace GameConfig {
    constexpr int kMinLobbyPlayers = 1;      //TEMPORARY FOR DEBUGGING, LET THE GAME START WITH 1 PLAYER NOW. CHANGE TO 2 PLAYERS WHEN WORKING
    constexpr int kMaxLobbyPlayers = 4;
    constexpr int kFrameDurationMs = 16;     // ~60 FPS
    constexpr int kRaycastRange = 15;
    constexpr int kBulletRaycastRange = 5;

    constexpr int kScreenWidth = 800;        // Default screen width
    constexpr int kScreenHeight = 600;       // Default screen height
    constexpr int kTileSize = 40;          // Size of a single grid tile
    constexpr float kDefaultRotationOffset = 90.0f; // Offset for rotation calculations

    constexpr float kfirstLobbyId = 1;   // Lobby unique IDs start from 1
    constexpr float kfirstGameId = 1;    // Game bby unique IDs start from 1
}
