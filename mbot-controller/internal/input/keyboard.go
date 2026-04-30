package input

import (
	"bufio"
	"fmt"
	"log"
	"os"

	"mbot-controller/internal/commands"
)

// TextKeyboard reads command characters from stdin.
type TextKeyboard struct{}

// Start reads keyboard input and emits recognized command bytes.
func (k *TextKeyboard) Start(out chan<- byte) {
	// Create a buffered reader for stdin
	reader := bufio.NewReader(os.Stdin)

	// Display available commands
	k.printHelp()

	// Main loop - continuously read and process keyboard input
	for {
		// Read a line from stdin
		line, err := reader.ReadString('\n')
		if err != nil {
			log.Fatalf("[KEYBOARD] input read error: %v", err)
			continue
		}

		// Skip empty lines
		if len(line) == 0 {
			continue
		}

		// Extract the first character as the command key
		key := line[0]
		// Look up the key in the command registry
		entry, ok := commands.Registry[key]
		if !ok {
			log.Printf("[KEYBOARD] unknown key: %s\n", string(key))
			continue
		}

		// Send the command code to the output channel
		out <- entry.Code
	}
}

// printHelp displays the available command keys and their descriptions.
func (k *TextKeyboard) printHelp() {
	fmt.Println("[KEYBOARD MODE] available commands")
	for _, key := range commands.KeyboardLayout {
		entry := commands.Registry[key]
		log.Printf("\t%c : %s\n", key, entry.Help)
	}
}
