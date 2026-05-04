#pragma once

namespace maze {
namespace config {

constexpr int kBaseSpeed = 60;

constexpr unsigned long kPauseShortMs = 50;
constexpr unsigned long kPauseMediumMs = 100;
constexpr unsigned long kPauseLongMs = 150;
constexpr unsigned long kPauseIslandEntryMs = 800;
constexpr unsigned long kPauseIslandSettleMs = 1000;
constexpr unsigned long kPauseTurnMs = 210;
constexpr unsigned long kPauseSpinMs = 250;
constexpr unsigned long kPauseTurnAroundTurnMs = 220;
constexpr unsigned long kPauseTurnAroundSpinMs = 260;

constexpr double kRightBlockedMinCm = 2.0;
constexpr double kRightBlockedMaxCm = 12.0;

constexpr double kTooCloseStrongCm = 3.5;
constexpr double kTooCloseLightCm = 4.5;
constexpr double kTooFarLightLowCm = 5.5;
constexpr double kTooFarLightHighCm = 7.0;
constexpr double kTooFarStrongLowCm = 7.0;
constexpr double kTooFarStrongHighCm = 8.5;
constexpr double kTooFarVeryStrongLowCm = 8.5;
constexpr double kTooFarVeryStrongHighCm = 12.0;

} // namespace config
} // namespace maze