#ifndef CONSTANTS_H  // Include guard to prevent double inclusion
#define CONSTANTS_H

constexpr int SQUARE_SIZE = 40;
constexpr int kPlayerSize = 30;
constexpr float PI = 3.1415;

namespace WeaponConfig
{
    constexpr float kBasicSpeed = 25;
    constexpr int kBasicDamage = 10;
    constexpr float kBasicFireRate = 1;  //fire rate of 1 means 1 shot every 1s. Fire rate of 0.1 means 10 shots every 1s
    constexpr float kMaxBasicBullets = 100;

    constexpr float kSpeedPowerupIncreasePercent = 100;
    constexpr float kDamagePowerupIncreasePercent = 100;

}

namespace PlayerConfig
{
    constexpr int kDefaultPlayeId = -1;
}

namespace GameConfig
{
    constexpr int kMinLobbyPlayers = 1;  //TEMPORARY FOR DEBUGGING, LET THE GAME START WITH 1 PLAYER NOW. CHANGE TO 2 PLAYERS WHEN WORKING
    constexpr int kMaxLobbyPlayers = 4;
}

#endif // CONSTANTS_H