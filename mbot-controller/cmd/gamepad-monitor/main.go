package main

import (
	"fmt"
	"log"

	"github.com/veandco/go-sdl2/sdl"
)

func main() {
	// Display introductory message
	printIntro()

	// Initialize SDL game controller subsystem
	if err := sdl.Init(sdl.INIT_GAMECONTROLLER); err != nil {
		log.Fatal(err)
	}
	defer sdl.Quit()

	// Check if a controller is connected
	if sdl.NumJoysticks() < 1 {
		log.Fatal("[GAMEPAD-MONITOR] no game controller detected")
	}

	// Open the first connected controller
	pad := sdl.GameControllerOpen(0)
	if pad == nil {
		log.Fatal("[GAMEPAD-MONITOR] failed to open controller")
	}
	defer pad.Close()

	// Display controller name and table header
	log.Printf("[GAMEPAD-MONITOR] monitoring controller: %s\n", pad.Name())
	log.Printf("%-15s %-18s %-12s\n", "[INPUT TYPE]", "[CONTROL]", "[VALUE]")

	// Main monitoring loop
	for {
		// Process all pending SDL events
		for event := sdl.PollEvent(); event != nil; event = sdl.PollEvent() {
			switch e := event.(type) {
			// Handle button press/release events
			case *sdl.ControllerButtonEvent:
				state := "released"
				if e.State == sdl.PRESSED {
					state = "pressed"
				}
				log.Printf("%-15s %-18s %-12s\n",
					"[Button]",
					sdl.GameControllerGetStringForButton(sdl.GameControllerButton(e.Button)),
					state,
				)

			// Handle analog axis events (triggers, sticks)
			case *sdl.ControllerAxisEvent:
				log.Printf("%-15s %-18s %-12d\n",
					"[Axis]",
					sdl.GameControllerGetStringForAxis(sdl.GameControllerAxis(e.Axis)),
					e.Value,
				)

			// Exit on quit event
			case *sdl.QuitEvent:
				return
			}
		}

		// Brief delay to prevent busy-waiting
		sdl.Delay(20)
	}
}

// printIntro displays an introductory message with the utility description.
func printIntro() {
	const (
		// ANSI color codes for formatting output
		purple = "\u001B[35m"
		reset  = "\u001B[0m"
	)

	fmt.Printf("%-35s %s\n", purple+"[gamepad-monitor]"+reset,
		"Utility for observing game controller input values")
	fmt.Println()
}
