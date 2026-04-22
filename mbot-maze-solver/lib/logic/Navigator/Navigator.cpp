#include "Navigator.h"

const Navigator::Rule Navigator::outerRules[] = {
    { &Navigator::rightOpen,   &Navigator::doRightOpen },
    { &Navigator::frontBlocked,&Navigator::doFrontBlocked },
    { &Navigator::defaultRule, &Navigator::doForward }
};

const Navigator::Rule Navigator::innerRules[] = {
    { &Navigator::rightOpen,   &Navigator::doRightOpen },
    { &Navigator::frontBlocked,&Navigator::doFrontBlocked },
    { &Navigator::defaultRule, &Navigator::doForward }
};

Navigator::Navigator(CommsUart& io, DriveSystem& drive, Indicators& leds, RangeSensor& sonar, LineBarrier& line)
    : io(io), drive(drive), leds(leds), sonar(sonar), line(line) {}

Navigator::Sense Navigator::sample() {
    Sense s{};
    s.rightCm = sonar.cm();
    s.rightWall = s.rightCm < RIGHT_OPEN_CM;
    s.frontWall = line.blocked();

    if (rightCooldown) {
        rightCooldown = s.rightWall ? false : true;
    }
    return s;
}

void Navigator::tick() {
    if (goal) return;

    Sense s = sample();
    const Rule* rules = (zone == Zone::OUTER) ? outerRules : innerRules;

    for (const auto& r : (zone == Zone::OUTER ? outerRules : innerRules)) {
        if (r.when(*this, s)) {
            r.then(*this, s);
            return;
        }
    }
}

bool Navigator::goalReached() const {
    return goal;
}

void Navigator::reset() {
    zone = Zone::OUTER;
    goal = false;
    rightCooldown = false;
    leftWallRun = 0;
    leds.off();
}

bool Navigator::rightOpen(Navigator& n, const Sense& s) {
    return (!s.rightWall && !n.rightCooldown);
}

void Navigator::doRightOpen(Navigator& n, const Sense&) {
    n.leftWallRun = 0;
    n.rightCooldown = true;

    n.io.write("[AUTO] Right opening\n");
    n.drive.stop();
    n.nudgeBeforeTurn();
    n.drive.turnRight90();
    n.nudgeAfterTurn();
}

bool Navigator::frontBlocked(Navigator& n, const Sense& s) {
    return s.frontWall;
}

void Navigator::doFrontBlocked(Navigator& n, const Sense&) {
    ++n.leftWallRun;

    if (n.zone == Zone::OUTER && n.leftWallRun >= WALL_RUN_LIMIT) {
        n.zone = Zone::INNER;
        n.rightCooldown = true;
        n.leds.red();

        n.io.write("[AUTO] Island detected\n");
        n.drive.stop();
        n.backoff();
        n.drive.turnAround();
        n.nudgeAfterTurn();
        return;
    }

    if (n.zone == Zone::INNER) {
        n.drive.stop();
        n.backoff();
        n.drive.stop();
        n.leds.green();
        n.goal = true;
        return;
    }

    n.io.write("[AUTO] Left turn\n");
    n.drive.stop();
    n.backoff();
    n.drive.turnLeft90();
}

bool Navigator::defaultRule(Navigator&, const Sense&) {
    return true;
}

void Navigator::doForward(Navigator& n, const Sense& s) {
    n.drive.forwardWithCorrection(s.rightCm, RIGHT_TARGET_CM, KP);
}

void Navigator::nudgeBeforeTurn() {
    drive.forward();
    delay(500);
}

void Navigator::nudgeAfterTurn() {
    drive.forward();
    delay(600);
}

void Navigator::backoff() {
    drive.backward();
    delay(100);
}