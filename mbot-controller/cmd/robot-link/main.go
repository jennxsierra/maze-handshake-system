package main

import (
	"flag"
	"fmt"
	"log"
	"time"

	"mbot-controller/internal/btlink"
	"mbot-controller/internal/input"
	"mbot-controller/internal/settings"
)

func main() {
	printIntro()

	// Select the input source used to generate robot commands.
	inputMode := flag.String("mode", "keyboard", "Input mode (keyboard|gamepad)")
	flag.Parse()

	// Connect to the BLE module used by the robot.
	link, err := btlink.OpenDevice(
		settings.DeviceLocalName,
		10*time.Second,
	)
	if err != nil {
		log.Fatal(err)
	}
	defer link.Close()

	// Build the chosen input source.
	var source input.Source
	switch *inputMode {
	case "keyboard":
		source = &input.TextKeyboard{}
	case "gamepad":
		source = &input.GamepadPad{}
	default:
		log.Fatal("unsupported input mode")
	}

	app := input.NewBridge(link, source)

	// Run until the input source stops or the BLE connection fails.
	if err := app.Run(); err != nil {
		log.Fatal(err)
	}
}

// printIntro displays an introductory message with the utility description.
func printIntro() {
	// ANSI color codes for formatting output
	const (
		purple = "\u001B[35m"
		reset  = "\u001B[0m"
	)

	fmt.Printf("%-35s %s\n", purple+"[robot-link]"+reset,
		"Desktop controller that sends robot commands through BLE")
	fmt.Println()
}
