package main

import (
	"context"
	"log"
	"os"
	"os/signal"
	"syscall"

	"mbot-controller/internal/controller"
)

func main() {
	ctx, stop := signal.NotifyContext(context.Background(), os.Interrupt, syscall.SIGINT, syscall.SIGTERM)
	defer stop()

	if err := controller.RunGamepadDiagnostic(ctx, os.Stdout); err != nil && err != context.Canceled {
		log.Fatalf("gamepad diagnostic stopped: %v", err)
	}
}
