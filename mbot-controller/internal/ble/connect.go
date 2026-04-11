package ble

import (
	"context"
	"errors"
	"fmt"
	"strings"
	"time"

	"mbot-controller/internal/config"

	"tinygo.org/x/bluetooth"
)

func Connect(ctx context.Context, scanTimeout time.Duration) (*Client, error) {
	adapter := bluetooth.DefaultAdapter
	err := adapter.Enable()
	if err != nil {
		return nil, fmt.Errorf("enable bluetooth adapter: %w", err)
	}

	addr, err := scanForDevice(ctx, scanTimeout, config.BLEModuleLocalName)
	if err != nil {
		return nil, err
	}

	connected, err := adapter.Connect(addr, bluetooth.ConnectionParams{})
	if err != nil {
		return nil, fmt.Errorf("connect to %s: %w", addr, err)
	}

	serviceUUID, err := bluetooth.ParseUUID(config.BLEServiceUUID)
	if err != nil {
		_ = connected.Disconnect()
		return nil, fmt.Errorf("parse BLE service UUID %q: %w", config.BLEServiceUUID, err)
	}
	rxUUID, err := bluetooth.ParseUUID(config.BLERXCharacteristicUUID)
	if err != nil {
		_ = connected.Disconnect()
		return nil, fmt.Errorf("parse BLE RX characteristic UUID %q: %w", config.BLERXCharacteristicUUID, err)
	}
	txUUID, err := bluetooth.ParseUUID(config.BLETXCharacteristicUUID)
	if err != nil {
		_ = connected.Disconnect()
		return nil, fmt.Errorf("parse BLE TX characteristic UUID %q: %w", config.BLETXCharacteristicUUID, err)
	}

	services, err := connected.DiscoverServices([]bluetooth.UUID{serviceUUID})
	if err != nil {
		_ = connected.Disconnect()
		return nil, fmt.Errorf("discover services: %w", err)
	}

	service := findService(services, serviceUUID)
	if service == nil {
		_ = connected.Disconnect()
		return nil, fmt.Errorf("service not found: %s", serviceUUID)
	}

	characteristics, err := service.DiscoverCharacteristics([]bluetooth.UUID{rxUUID, txUUID})
	if err != nil {
		_ = connected.Disconnect()
		return nil, fmt.Errorf("discover characteristics for service %s: %w", serviceUUID, err)
	}

	rx := findCharacteristic(characteristics, rxUUID)
	if rx == nil {
		_ = connected.Disconnect()
		return nil, fmt.Errorf("receive characteristic not found: %s", rxUUID)
	}

	tx := findCharacteristic(characteristics, txUUID)
	if tx == nil {
		_ = connected.Disconnect()
		return nil, fmt.Errorf("transmit characteristic not found: %s", txUUID)
	}

	return NewClient(connected, rx, tx), nil
}

func scanForDevice(ctx context.Context, scanTimeout time.Duration, localName string) (bluetooth.Address, error) {
	adapter := bluetooth.DefaultAdapter
	resultCh := make(chan bluetooth.ScanResult, 1)
	errCh := make(chan error, 1)

	go func() {
		errCh <- adapter.Scan(func(adapter *bluetooth.Adapter, result bluetooth.ScanResult) {
			if !strings.EqualFold(result.LocalName(), localName) {
				return
			}
			select {
			case resultCh <- result:
			default:
			}
			_ = adapter.StopScan()
		})
	}()

	timer := time.NewTimer(scanTimeout)
	defer timer.Stop()

	for {
		select {
		case result := <-resultCh:
			return result.Address, nil
		case err := <-errCh:
			if err == nil {
				return bluetooth.Address{}, fmt.Errorf("device not found: %q", localName)
			}
			return bluetooth.Address{}, fmt.Errorf("scan for %q: %w", localName, err)
		case <-timer.C:
			_ = adapter.StopScan()
			return bluetooth.Address{}, fmt.Errorf("scan timeout while looking for %q", localName)
		case <-ctx.Done():
			_ = adapter.StopScan()
			if errors.Is(ctx.Err(), context.DeadlineExceeded) {
				return bluetooth.Address{}, fmt.Errorf("scan timeout while looking for %q", localName)
			}
			return bluetooth.Address{}, ctx.Err()
		}
	}
}

func findService(services []bluetooth.DeviceService, target bluetooth.UUID) *bluetooth.DeviceService {
	for i := range services {
		if services[i].UUID().String() == target.String() {
			return &services[i]
		}
	}
	return nil
}

func findCharacteristic(characteristics []bluetooth.DeviceCharacteristic, target bluetooth.UUID) *bluetooth.DeviceCharacteristic {
	for i := range characteristics {
		if characteristics[i].UUID().String() == target.String() {
			return &characteristics[i]
		}
	}
	return nil
}
