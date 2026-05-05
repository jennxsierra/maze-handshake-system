#pragma once

// Project-wide configuration values used by motion and algorithm modules.
// Units: speeds in motor units, distances in centimeters, pauses in ms.
namespace maze
{
    namespace config
    {
        // Core drive settings
        constexpr int kSpeed = 60; // base motor speed

        // Timing (milliseconds)
        constexpr unsigned long kPauseShort = 50;           // short, per-action pause
        constexpr unsigned long kPauseTurn = 210;           // initial turn duration
        constexpr unsigned long kPauseSpin = 250;           // spin to complete turn
        constexpr unsigned long kPauseTurnAroundTurn = 220; // turnaround initial turn
        constexpr unsigned long kPauseTurnAroundSpin = 260; // turnaround spin
        constexpr unsigned long kPauseMedium = 100;         // medium pause
        constexpr unsigned long kPauseLong = 150;           // long pause
        constexpr unsigned long kPauseIslandEntry = 800;    // wait after island entry
        constexpr unsigned long kPauseIslandSettle = 1000;  // extra settle time on island

        // Right-side distance thresholds (cm)
        constexpr double kRightMinCm = 2.0;  // sensor min considered
        constexpr double kRightMaxCm = 12.0; // sensor max considered for blocked

        // Wall-following distance bands (cm)
        constexpr double kCloseStrongCm = 4.5;        // too close (strong adjust)
        constexpr double kCloseLightCm = 5.0;         // slightly close (light adjust)
        constexpr double kFarLightLowCm = 5.5;        // slightly far lower bound
        constexpr double kFarLightHighCm = 7.0;       // slightly far upper bound
        constexpr double kFarStrongLowCm = 7.0;       // far strong lower bound
        constexpr double kFarStrongHighCm = 8.5;      // far strong upper bound
        constexpr double kFarVeryStrongLowCm = 8.5;   // very far lower bound
        constexpr double kFarVeryStrongHighCm = 12.0; // very far upper bound

    }
}
