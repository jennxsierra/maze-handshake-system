package ble

import (
	"fmt"

	"tinygo.org/x/bluetooth"
)

type Client struct {
	device bluetooth.Device
	rx     *bluetooth.DeviceCharacteristic
	tx     *bluetooth.DeviceCharacteristic
}

func NewClient(device bluetooth.Device, rx, tx *bluetooth.DeviceCharacteristic) *Client {
	return &Client{device: device, rx: rx, tx: tx}
}

func (c *Client) Write(payload []byte) error {
	if len(payload) == 0 {
		return nil
	}
	if c.tx == nil {
		return fmt.Errorf("write characteristic is not configured")
	}
	_, err := c.tx.WriteWithoutResponse(payload)
	return err
}

func (c *Client) Subscribe(handler func([]byte)) error {
	if c.rx == nil {
		return fmt.Errorf("notification characteristic is not configured")
	}
	return c.rx.EnableNotifications(func(payload []byte) {
		if handler == nil {
			return
		}
		copyPayload := make([]byte, len(payload))
		copy(copyPayload, payload)
		handler(copyPayload)
	})
}

func (c *Client) Close() error {
	return c.device.Disconnect()
}
