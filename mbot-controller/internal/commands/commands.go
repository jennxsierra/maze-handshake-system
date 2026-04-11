package commands

import (
	"fmt"
	"strings"
)

type Definition struct {
	Key    byte
	Action string
}

type KeyboardBinding struct {
	Key         rune
	Command     string
	Description string
}

type GamepadBinding struct {
	Control     string
	Command     string
	Description string
}

var Definitions = map[string]Definition{
	"forward":  {Key: 'F', Action: "forward"},
	"backward": {Key: 'B', Action: "backward"},
	"left":     {Key: 'L', Action: "left turn"},
	"right":    {Key: 'R', Action: "right turn"},
	"stop":     {Key: 'S', Action: "stop"},
	"auto":     {Key: 'A', Action: "autonomous mode"},
	"manual":   {Key: 'M', Action: "manual mode"},
}

var KeyboardBindings = []KeyboardBinding{
	{Key: 'w', Command: "forward", Description: "Move forward while held"},
	{Key: 's', Command: "backward", Description: "Move backward while held"},
	{Key: 'a', Command: "left", Description: "Turn left while held"},
	{Key: 'd', Command: "right", Description: "Turn right while held"},
	{Key: 'x', Command: "stop", Description: "Stop movement"},
	{Key: 'o', Command: "auto", Description: "Enable autonomous mode"},
	{Key: 'm', Command: "manual", Description: "Return to manual mode"},
}

var GamepadBindings = []GamepadBinding{
	{Control: "a", Command: "forward", Description: "Move forward while held"},
	{Control: "b", Command: "backward", Description: "Move backward while held"},
	{Control: "leftshoulder", Command: "left", Description: "Turn left while held"},
	{Control: "rightshoulder", Command: "right", Description: "Turn right while held"},
	{Control: "lefttrigger", Command: "left", Description: "Analog left turn when pulled past the threshold"},
	{Control: "righttrigger", Command: "right", Description: "Analog right turn when pulled past the threshold"},
	{Control: "back", Command: "manual", Description: "Return to manual mode"},
	{Control: "start", Command: "auto", Description: "Enable autonomous mode"},
}

var CommandPriority = []string{"forward", "backward", "left", "right", "auto", "manual"}

var (
	keyboardLookup = map[rune]string{}
	gamepadLookup  = map[string]string{}
	codeLookup     = map[byte]string{}
)

func init() {
	for name, definition := range Definitions {
		codeLookup[definition.Key] = name
	}
	for _, binding := range KeyboardBindings {
		keyboardLookup[normalizeRune(binding.Key)] = binding.Command
	}
	for _, binding := range GamepadBindings {
		gamepadLookup[normalizeControl(binding.Control)] = binding.Command
	}
}

func DefinitionByName(name string) (Definition, bool) {
	definition, ok := Definitions[normalizeName(name)]
	return definition, ok
}

func DefinitionByCode(code byte) (Definition, bool) {
	name, ok := codeLookup[code]
	if !ok {
		return Definition{}, false
	}
	return Definitions[name], true
}

func KeyboardCommandForRune(key rune) (Definition, bool) {
	command, ok := keyboardLookup[normalizeRune(key)]
	if !ok {
		return Definition{}, false
	}
	return DefinitionByName(command)
}

func GamepadCommandForControl(control string) (Definition, bool) {
	command, ok := gamepadLookup[normalizeControl(control)]
	if !ok {
		return Definition{}, false
	}
	return DefinitionByName(command)
}

func GamepadMappingForControl(control string) (string, Definition, bool) {
	command, ok := gamepadLookup[normalizeControl(control)]
	if !ok {
		return "", Definition{}, false
	}
	definition, ok := DefinitionByName(command)
	if !ok {
		return "", Definition{}, false
	}
	return command, definition, true
}

func KeyboardHelpLines() []string {
	lines := make([]string, 0, len(KeyboardBindings))
	for _, binding := range KeyboardBindings {
		definition := Definitions[binding.Command]
		lines = append(lines, fmt.Sprintf("%s -> %s (%s)", printableKey(binding.Key), definition.Action, binding.Description))
	}
	return lines
}

func GamepadHelpLines() []string {
	lines := make([]string, 0, len(GamepadBindings))
	for _, binding := range GamepadBindings {
		definition := Definitions[binding.Command]
		lines = append(lines, fmt.Sprintf("%s -> %s (%s)", binding.Control, definition.Action, binding.Description))
	}
	return lines
}

func CommandNameForCode(code byte) string {
	definition, ok := DefinitionByCode(code)
	if !ok {
		return fmt.Sprintf("unknown(%q)", code)
	}
	return definition.Action
}

func printableKey(key rune) string {
	switch key {
	case ' ':
		return "[space]"
	case '\t':
		return "[tab]"
	default:
		return fmt.Sprintf("%c", key)
	}
}

func normalizeName(name string) string {
	return strings.ToLower(strings.TrimSpace(name))
}

func normalizeControl(control string) string {
	control = strings.ToLower(strings.TrimSpace(control))
	control = strings.TrimPrefix(control, "button:")
	control = strings.TrimPrefix(control, "axis:")
	return control
}

func normalizeRune(key rune) rune {
	if key >= 'A' && key <= 'Z' {
		return key + ('a' - 'A')
	}
	return key
}
