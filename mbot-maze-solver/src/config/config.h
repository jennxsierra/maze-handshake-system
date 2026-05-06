#pragma once

// Project-wide configuration values used by motion and algorithm modules.
// Units: speeds in motor units, distances in centimeters, pauses in ms.
namespace maze
{
    namespace config
    {
        // Core drive settings
        constexpr int kSpeed = 67; // base motor speed

        // Timing (milliseconds)
        constexpr unsigned long kPauseShort = 50;           // short, per-action pause
        constexpr unsigned long kPauseTurn = 245;           // initial turn duration
        constexpr unsigned long kPauseSpin = 285;           // spin to complete turn
        constexpr unsigned long kPauseTurnAroundTurn = 260; // turnaround initial turn
        constexpr unsigned long kPauseTurnAroundSpin = 300; // turnaround spin
        constexpr unsigned long kPauseMedium = 100;         // medium pause
        constexpr unsigned long kPauseLong = 150;           // long pause
        constexpr unsigned long kPauseIslandEntry = 800;    // wait after island entry
        constexpr unsigned long kPauseIslandSettle = 1000;  // extra settle time on island

        // Right-side distance thresholds (cm)
        constexpr double kRightMinCm = 2.0;  // sensor min considered
        constexpr double kRightMaxCm = 15.0; // sensor max considered for blocked

        // Wall-following distance bands (cm)
        constexpr double kCloseStrongCm = 3.5;        // too close (strong adjust)
        constexpr double kCloseLightCm = 5.0;         // slightly close (light adjust)
        constexpr double kFarLightLowCm = 5.75;       // slightly far lower bound
        constexpr double kFarLightHighCm = 6.75;      // slightly far upper bound
        constexpr double kFarStrongLowCm = 6.75;      // far strong lower bound
        constexpr double kFarStrongHighCm = 7.75;     // far strong upper bound
        constexpr double kFarVeryStrongLowCm = 7.75;  // very far lower bound
        constexpr double kFarVeryStrongHighCm = 14.0; // very far upper bound

        // Sensor sentinel value present in sample (e.g. 400). Solver not yet using this.
        constexpr int kRightInvalidReading = 400;

    }
}
