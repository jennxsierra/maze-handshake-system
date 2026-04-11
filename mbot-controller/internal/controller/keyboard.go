package controller

import (
	"bufio"
	"context"
	"fmt"
	"io"
	"os"
	"unicode/utf8"

	"mbot-controller/internal/commands"
)

type KeyboardInput struct {
	reader *bufio.Reader
	writer io.Writer
}

func NewKeyboardInput(writer io.Writer) *KeyboardInput {
	if writer == nil {
		writer = os.Stdout
	}
	return &KeyboardInput{reader: bufio.NewReader(os.Stdin), writer: writer}
}

func (k *KeyboardInput) Start(ctx context.Context, out chan<- byte) error {
	fmt.Fprintln(k.writer, "Keyboard controls:")
	for _, line := range commands.KeyboardHelpLines() {
		fmt.Fprintln(k.writer, " ", line)
	}

	for {
		select {
		case <-ctx.Done():
			return ctx.Err()
		default:
		}

		line, err := k.reader.ReadString('\n')
		if line != "" {
			r, _ := utf8.DecodeRuneInString(line)
			if r != '\n' && r != '\r' {
				definition, ok := commands.KeyboardCommandForRune(r)
				if !ok {
					fmt.Fprintf(k.writer, "Ignoring unknown keyboard input %q\n", r)
				} else {
					select {
					case out <- definition.Key:
					case <-ctx.Done():
						return ctx.Err()
					}
				}
			}
		}
		if err != nil {
			if err == io.EOF {
				return nil
			}
			return fmt.Errorf("read keyboard input: %w", err)
		}
	}
}
