package controller

import (
	"context"
	"fmt"
	"io"
	"log"
	"os"
	"strings"

	"mbot-controller/internal/commands"

	"github.com/veandco/go-sdl2/sdl"
)

const triggerThreshold = 16000

type GamepadInput struct {
	writer io.Writer
}

func NewGamepadInput(writer io.Writer) *GamepadInput {
	if writer == nil {
		writer = os.Stdout
	}
	return &GamepadInput{writer: writer}
}

func (g *GamepadInput) Start(ctx context.Context, out chan<- byte) error {
	if err := sdl.Init(sdl.INIT_GAMECONTROLLER | sdl.INIT_EVENTS); err != nil {
		return fmt.Errorf("initialize SDL2 game controller subsystem: %w", err)
	}
	defer sdl.Quit()

	controller, instanceID, err := openFirstController()
	if err != nil {
		return err
	}
	defer controller.Close()

	joystick := controller.Joystick()
	if joystick == nil {
		return fmt.Errorf("game controller did not expose a joystick")
	}

	fmt.Fprintf(g.writer, "Gamepad controller: %s\n", controller.Name())
	fmt.Fprintf(g.writer, "Instance ID: %d\n", joystick.InstanceID())
	fmt.Fprintln(g.writer, "Gamepad controls:")
	for _, line := range commands.GamepadHelpLines() {
		fmt.Fprintln(g.writer, " ", line)
	}

	activeByCommand := map[string]int{}
	activeByControl := map[string]bool{}
	currentCommand := "stop"

	sendCommand := func(commandName string) error {
		definition, ok := commands.DefinitionByName(commandName)
		if !ok {
			return nil
		}
		if currentCommand == commandName {
			return nil
		}
		select {
		case out <- definition.Key:
		case <-ctx.Done():
			return ctx.Err()
		}
		currentCommand = commandName
		return nil
	}

	activate := func(controlName string) error {
		commandName, definition, ok := commands.GamepadMappingForControl(controlName)
		if !ok {
			return nil
		}
		if activeByControl[controlName] {
			return nil
		}
		activeByControl[controlName] = true
		activeByCommand[commandName]++
		if currentCommand == commandName {
			return nil
		}
		if err := sendCommand(commandName); err != nil {
			return err
		}
		log.Printf("gamepad -> %s (%q)", definition.Action, definition.Key)
		return nil
	}

	release := func(controlName string) error {
		commandName, _, ok := commands.GamepadMappingForControl(controlName)
		if !ok {
			return nil
		}
		if !activeByControl[controlName] {
			return nil
		}
		activeByControl[controlName] = false
		count := activeByCommand[commandName] - 1
		if count <= 0 {
			delete(activeByCommand, commandName)
		} else {
			activeByCommand[commandName] = count
		}
		if currentCommand != commandName {
			return nil
		}
		for _, candidate := range commands.CommandPriority {
			if activeByCommand[candidate] > 0 {
				if err := sendCommand(candidate); err != nil {
					return err
				}
				if candidateDefinition, ok := commands.DefinitionByName(candidate); ok {
					log.Printf("gamepad -> %s (%q)", candidateDefinition.Action, candidateDefinition.Key)
				}
				return nil
			}
		}
		if err := sendCommand("stop"); err != nil {
			return err
		}
		if stopDefinition, ok := commands.DefinitionByName("stop"); ok {
			log.Printf("gamepad -> %s (%q)", stopDefinition.Action, stopDefinition.Key)
		}
		return nil
	}

	for {
		for event := sdl.PollEvent(); event != nil; event = sdl.PollEvent() {
			switch ev := event.(type) {
			case *sdl.QuitEvent:
				return nil
			case *sdl.ControllerDeviceEvent:
				if ev.Type == sdl.CONTROLLERDEVICEREMOVED && ev.Which == instanceID {
					return fmt.Errorf("game controller disconnected")
				}
			case *sdl.ControllerButtonEvent:
				buttonName := normalizeSDLControlName(sdl.GameControllerGetStringForButton(sdl.GameControllerButton(ev.Button)))
				if ev.State == sdl.PRESSED {
					if err := activate(buttonName); err != nil {
						return err
					}
				} else {
					if err := release(buttonName); err != nil {
						return err
					}
				}
			case *sdl.ControllerAxisEvent:
				axisName := normalizeSDLControlName(sdl.GameControllerGetStringForAxis(sdl.GameControllerAxis(ev.Axis)))
				if !axisIsMapped(axisName) {
					continue
				}
				pressed := ev.Value > triggerThreshold
				if pressed && !activeByControl[axisName] {
					if err := activate(axisName); err != nil {
						return err
					}
				}
				if !pressed && activeByControl[axisName] {
					if err := release(axisName); err != nil {
						return err
					}
				}
			}
		}

		select {
		case <-ctx.Done():
			return ctx.Err()
		default:
			sdl.Delay(10)
		}
	}
}

func RunGamepadDiagnostic(ctx context.Context, writer io.Writer) error {
	if writer == nil {
		writer = os.Stdout
	}
	if err := sdl.Init(sdl.INIT_GAMECONTROLLER | sdl.INIT_EVENTS); err != nil {
		return fmt.Errorf("initialize SDL2 game controller subsystem: %w", err)
	}
	defer sdl.Quit()

	controller, instanceID, err := openFirstController()
	if err != nil {
		return err
	}
	defer controller.Close()

	fmt.Fprintf(writer, "Controller: %s\n", controller.Name())
	fmt.Fprintf(writer, "Instance ID: %d\n", instanceID)
	fmt.Fprintln(writer, "Control map:")
	for _, line := range commands.GamepadHelpLines() {
		fmt.Fprintln(writer, " ", line)
	}

	for {
		for event := sdl.PollEvent(); event != nil; event = sdl.PollEvent() {
			switch ev := event.(type) {
			case *sdl.QuitEvent:
				return nil
			case *sdl.ControllerDeviceEvent:
				if ev.Type == sdl.CONTROLLERDEVICEREMOVED && ev.Which == instanceID {
					return fmt.Errorf("game controller disconnected")
				}
			case *sdl.ControllerButtonEvent:
				buttonName := sdl.GameControllerGetStringForButton(sdl.GameControllerButton(ev.Button))
				state := "released"
				if ev.State == sdl.PRESSED {
					state = "pressed"
				}
				fmt.Fprintf(writer, "button %s %s\n", buttonName, state)
			case *sdl.ControllerAxisEvent:
				axisName := sdl.GameControllerGetStringForAxis(sdl.GameControllerAxis(ev.Axis))
				triggerState := "resting"
				if axisIsTrigger(axisName) && ev.Value > triggerThreshold {
					triggerState = "pressed"
				}
				fmt.Fprintf(writer, "axis %s value=%d trigger=%s\n", axisName, ev.Value, triggerState)
			}
		}

		select {
		case <-ctx.Done():
			return ctx.Err()
		default:
			sdl.Delay(10)
		}
	}
}

func openFirstController() (*sdl.GameController, sdl.JoystickID, error) {
	total := sdl.NumJoysticks()
	for i := 0; i < total; i++ {
		if !sdl.IsGameController(i) {
			continue
		}
		controller := sdl.GameControllerOpen(i)
		if controller == nil {
			return nil, 0, fmt.Errorf("open game controller at index %d: %s", i, sdl.GetError())
		}
		joystick := controller.Joystick()
		if joystick == nil {
			controller.Close()
			return nil, 0, fmt.Errorf("open joystick for game controller at index %d", i)
		}
		return controller, joystick.InstanceID(), nil
	}
	return nil, 0, fmt.Errorf("no compatible SDL game controller detected")
}

func normalizeSDLControlName(name string) string {
	name = strings.ToLower(strings.TrimSpace(name))
	name = strings.TrimPrefix(name, "button:")
	name = strings.TrimPrefix(name, "axis:")
	return name
}

func axisIsTrigger(name string) bool {
	switch normalizeSDLControlName(name) {
	case "lefttrigger", "righttrigger":
		return true
	default:
		return false
	}
}

func axisIsMapped(name string) bool {
	_, ok := commands.GamepadCommandForControl(name)
	return ok
}
