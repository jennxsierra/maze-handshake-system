#pragma once
#include <Arduino.h>
#include "../control/CommsUART/CommsUart.h"
#include "../control/DriveSystem/DriveSystem.h"
#include "../control/Indicators/Indicators.h"
#include "../control/Sensors/Sensors.h"

// Navigator implements right-hand rule maze-solving logic
class Navigator {
public:
    Navigator(CommsUart& io, DriveSystem& drive, Indicators& leds, RangeSensor& sonar, LineBarrier& line);

    void tick();
    bool goalReached() const;
    void reset();

private:
    struct Sense {
        uint16_t rightCm;
        bool rightWall;
        bool frontWall;
    };

    struct Rule {
        bool (*when)(Navigator&, const Sense&);
        void (*then)(Navigator&, const Sense&);
    };

    enum class Zone { OUTER, INNER };

    // rules
    static bool rightOpen(Navigator&, const Sense&);
    static void doRightOpen(Navigator&, const Sense&);

    static bool frontBlocked(Navigator&, const Sense&);
    static void doFrontBlocked(Navigator&, const Sense&);

    static bool defaultRule(Navigator&, const Sense&);
    static void doForward(Navigator&, const Sense&);

    Sense sample();

    void nudgeBeforeTurn();
    void nudgeAfterTurn();
    void backoff();

    CommsUart& io;
    DriveSystem& drive;
    Indicators& leds;
    RangeSensor& sonar;
    LineBarrier& line;

    Zone zone{ Zone::OUTER };
    bool goal{ false };
    bool rightCooldown{ false };
    int leftWallRun{ 0 };

    static constexpr int RIGHT_OPEN_CM{ 18 };
    static constexpr int RIGHT_TARGET_CM{ 6 };
    static constexpr float KP{ 3.0f };
    static constexpr int WALL_RUN_LIMIT{ 4 };

    static const Rule outerRules[];
    static const Rule innerRules[];
};
