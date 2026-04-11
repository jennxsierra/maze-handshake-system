package main

import (
	"context"
	"flag"
	"fmt"
	"log"
	"os"
	"os/signal"
	"strings"
	"syscall"
	"time"

	"mbot-controller/internal/ble"
	"mbot-controller/internal/controller"
)

func main() {
	mode := flag.String("mode", "keyboard", "input mode: keyboard or gamepad")
	flag.Parse()

	selectedMode := strings.ToLower(strings.TrimSpace(*mode))
	if selectedMode == "" {
		selectedMode = "keyboard"
	}
	if selectedMode != "keyboard" && selectedMode != "gamepad" {
		log.Fatalf("unsupported mode %q", selectedMode)
	}

	fmt.Println("mBot controller")
	fmt.Printf("starting in %s mode\n", selectedMode)
	fmt.Println("connecting to BLE robot...")

	ctx, stop := signal.NotifyContext(context.Background(), os.Interrupt, syscall.SIGINT, syscall.SIGTERM)
	defer stop()

	bleClient, err := ble.Connect(ctx, 15*time.Second)
	if err != nil {
		log.Fatalf("BLE connection failed: %v", err)
	}
	fmt.Println("connected")

	var input controller.Input
	switch selectedMode {
	case "keyboard":
		input = controller.NewKeyboardInput(os.Stdout)
	case "gamepad":
		input = controller.NewGamepadInput(os.Stdout)
	}

	ctrl := controller.New(bleClient, input)
	if err := ctrl.Run(ctx); err != nil && err != context.Canceled {
		log.Fatalf("controller stopped: %v", err)
	}
}
