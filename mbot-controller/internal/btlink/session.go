package btlink

import "tinygo.org/x/bluetooth"

// Session keeps the connected BLE device and its readable/writable endpoints.
type Session struct {
	device bluetooth.Device
	tx     bluetooth.DeviceCharacteristic
	rx     bluetooth.DeviceCharacteristic
}

// Send writes raw bytes to the robot.
func (s *Session) Send(data []byte) (int, error) {
	return s.tx.WriteWithoutResponse(data)
}

// Watch enables notifications from the robot and forwards each packet to the handler.
func (s *Session) Watch(handler func([]byte)) error {
	return s.rx.EnableNotifications(func(buf []byte) {
		handler(buf)
	})
}

// Close disconnects from the BLE peripheral.
func (s *Session) Close() error {
	return s.device.Disconnect()
}
