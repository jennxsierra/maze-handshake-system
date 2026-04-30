package btlink

import (
	"errors"
	"fmt"
	"log"
	"time"

	"mbot-controller/internal/settings"

	"tinygo.org/x/bluetooth"
)

var defaultAdapter = bluetooth.DefaultAdapter

// OpenDevice searches for the named BLE peripheral, connects to it, and
// locates the UART-like characteristics used to exchange command bytes.
func OpenDevice(name string, timeout time.Duration) (*Session, error) {
	// Enable the Bluetooth adapter
	log.Println("[BLUETOOTH] enabling adapter...")
	if err := defaultAdapter.Enable(); err != nil {
		return nil, fmt.Errorf("[BLUETOOTH] adapter enable failed: %w", err)
	}

	// Scan for the BLE peripheral with the specified name
	log.Printf("[BLUETOOTH] scanning for %s...\n", name)

	var foundAddr bluetooth.Address
	done := make(chan struct{})
	var scanErr error

	go func() {
		scanErr = defaultAdapter.Scan(func(a *bluetooth.Adapter, result bluetooth.ScanResult) {
			if result.LocalName() == name {
				foundAddr = result.Address
				a.StopScan()
				close(done)
			}
		})
	}()

	// Wait for the device to be found or timeout
	select {
	case <-done:
	case <-time.After(timeout):
		defaultAdapter.StopScan()
		return nil, fmt.Errorf("[BLUETOOTH] %s not found before timeout", name)
	}

	if scanErr != nil {
		return nil, scanErr
	}

	// Connect to the found device
	log.Printf("[BLUETOOTH] connecting to %s...\n", name)
	device, err := defaultAdapter.Connect(foundAddr, bluetooth.ConnectionParams{})
	if err != nil {
		return nil, fmt.Errorf("[BLUETOOTH] connect failed: %w", err)
	}
	log.Printf("[BLUETOOTH] connected to %s\n", name)

	// Discover available services and characteristics
	log.Println("[BLUETOOTH] discovering service and characteristics...")

	services, err := device.DiscoverServices(nil)
	if err != nil {
		return nil, err
	}

	// Find the RX and TX UART characteristics
	var rx, tx bluetooth.DeviceCharacteristic

	for _, svc := range services {
		if svc.UUID().String() != settings.ServiceUUID {
			continue
		}

		chars, err := svc.DiscoverCharacteristics(nil)
		if err != nil {
			return nil, err
		}

		for _, ch := range chars {
			switch ch.UUID().String() {
			case settings.RXUUID:
				rx = ch
			case settings.TXUUID:
				tx = ch
			}
		}
	}

	// Validate that both required characteristics were found
	if tx.UUID() == (bluetooth.UUID{}) || rx.UUID() == (bluetooth.UUID{}) {
		return nil, errors.New("[BLUETOOTH] required UART characteristics not found")
	}

	log.Println("[BLUETOOTH] service and characteristics ready")

	// Return the session with established device and characteristics
	return &Session{
		device: device,
		tx:     tx,
		rx:     rx,
	}, nil
}
