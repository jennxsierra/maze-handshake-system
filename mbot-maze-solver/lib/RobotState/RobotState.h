#pragma once

// RobotState identifies the active control mode.
enum class RobotState {
    MANUAL,     // Robot is under manual control via Bluetooth.
    AUTONOMOUS  // Robot is autonomously navigating the maze via right-hand rule.
};