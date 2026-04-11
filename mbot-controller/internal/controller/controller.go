package controller

import (
	"context"
	"fmt"
	"log"

	"mbot-controller/internal/commands"
)

type Input interface {
	Start(ctx context.Context, out chan<- byte) error
}

type BLEClient interface {
	Write(payload []byte) error
	Subscribe(handler func([]byte)) error
	Close() error
}

type Controller struct {
	client BLEClient
	input  Input
}

func New(client BLEClient, input Input) *Controller {
	return &Controller{client: client, input: input}
}

func (c *Controller) Run(ctx context.Context) error {
	commandCh := make(chan byte)
	inputErrCh := make(chan error, 1)

	go func() {
		inputErrCh <- c.input.Start(ctx, commandCh)
	}()

	defer func() {
		if err := c.client.Close(); err != nil {
			log.Printf("BLE close: %v", err)
		}
	}()

	if err := c.client.Subscribe(func(payload []byte) {
		if len(payload) == 0 {
			log.Printf("robot <- <empty>")
			return
		}
		log.Printf("robot <- %q [% X]", string(payload), payload)
	}); err != nil {
		return fmt.Errorf("subscribe to BLE notifications: %w", err)
	}

	activeOutput := byte(0)
	hasOutput := false

	for {
		select {
		case <-ctx.Done():
			return ctx.Err()
		case err := <-inputErrCh:
			if err != nil {
				return err
			}
			return nil
		case command := <-commandCh:
			definition, ok := commands.DefinitionByCode(command)
			if !ok {
				log.Printf("ignoring unknown command byte: %d", command)
				continue
			}
			if hasOutput && activeOutput == command {
				continue
			}
			if err := c.client.Write([]byte{command}); err != nil {
				return fmt.Errorf("write command %s: %w", definition.Action, err)
			}
			activeOutput = command
			hasOutput = true
			log.Printf("robot -> %s (%q)", definition.Action, command)
		}
	}
}
