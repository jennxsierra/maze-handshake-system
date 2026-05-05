package input

import (
	"log"

	"mbot-controller/internal/commands"

	"github.com/veandco/go-sdl2/sdl"
)

// GamepadPad reads SDL game controller events and converts them to commands.
type GamepadPad struct{}

// Start initializes SDL, opens the first controller, and streams command bytes.
func (g *GamepadPad) Start(out chan<- byte) {
	// Initialize SDL game controller subsystem
	if err := sdl.Init(sdl.INIT_GAMECONTROLLER); err != nil {
		log.Fatal(err)
	}
	defer sdl.Quit()

	// Check if any controllers are connected
	if sdl.NumJoysticks() < 1 {
		log.Fatal("[GAMEPAD] no controllers found")
	}

	// Open the first connected controller
	pad := sdl.GameControllerOpen(0)
	if pad == nil {
		log.Fatal("[GAMEPAD] could not open controller")
	}
	defer pad.Close()

	// Log connection success and display control bindings
	log.Println("[GAMEPAD] connected to controller successfully")
	g.printHelp(pad.Name())

	// Main event loop - continuously poll for controller input
	for {
		// Process all pending SDL events
		for event := sdl.PollEvent(); event != nil; event = sdl.PollEvent() {
			switch e := event.(type) {
			// Handle button presses and releases
			case *sdl.ControllerButtonEvent:
				buttonName := sdl.GameControllerGetStringForButton(sdl.GameControllerButton(e.Button))

				switch buttonName {
				case "x":
					if e.State == sdl.PRESSED {
						out <- commands.Registry['X'].Code
					}
				case "a":
					if e.State == sdl.PRESSED {
						out <- commands.Registry['F'].Code
					} else if e.State == sdl.RELEASED {
						out <- commands.Registry['S'].Code
					}
				case "b":
					if e.State == sdl.PRESSED {
						out <- commands.Registry['B'].Code
					} else if e.State == sdl.RELEASED {
						out <- commands.Registry['S'].Code
					}
				case "leftshoulder":
					if e.State == sdl.PRESSED {
						out <- commands.Registry['L'].Code
					}
				case "rightshoulder":
					if e.State == sdl.PRESSED {
						out <- commands.Registry['R'].Code
					}
				case "start":
					if e.State == sdl.PRESSED {
						out <- commands.Registry['A'].Code
					}
				case "back":
					if e.State == sdl.PRESSED {
						out <- commands.Registry['M'].Code
					}
				}

			// Handle analog triggers and sticks
			case *sdl.ControllerAxisEvent:
				axis := sdl.GameControllerAxis(e.Axis)

				switch axis {
				case sdl.CONTROLLER_AXIS_TRIGGERLEFT:
					if e.Value > 20000 {
						out <- commands.Registry['1'].Code
					} else {
						out <- commands.Registry['S'].Code
					}
				case sdl.CONTROLLER_AXIS_TRIGGERRIGHT:
					if e.Value > 20000 {
						out <- commands.Registry['2'].Code
					} else {
						out <- commands.Registry['S'].Code
					}
				}
			}
		}

		// Brief delay to prevent busy-waiting
		sdl.Delay(5)
	}
}

// printHelp logs the supported controls for the gamepad.
func (g *GamepadPad) printHelp(name string) {
	log.Printf("[GAMEPAD MODE - %s] supported controls\n", name)
	for _, key := range commands.PadLayout {
		cmdKey := commands.PadBindings[key]
		entry := commands.Registry[cmdKey]
		label := commands.PadLabels[key]
		log.Printf("  %-18s : %s\n", label, entry.Help)
	}
}
