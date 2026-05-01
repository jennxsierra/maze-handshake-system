# Makefile

ECHO_PREFIX = [make]

# Run the controller in keyboard mode.
.PHONY: run/keyboard
run/keyboard:
	go run ./cmd/robot-link -mode=keyboard

# Run the controller in gamepad mode.
.PHONY: run/gamepad
run/gamepad:
	go run ./cmd/robot-link -mode=gamepad

# Launch the standalone gamepad monitor.
.PHONY: run/test/gamepad-monitor
run/test/gamepad-monitor:
	go run ./cmd/gamepad-monitor

# Build the robotlink binary.
.PHONY: build/controller
build/controller:
	@echo '${ECHO_PREFIX} building robotlink...'
	go build -ldflags='-s' -o=./bin/robotlink ./cmd/robot-link
