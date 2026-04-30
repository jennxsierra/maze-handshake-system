package input

import (
	"log"

	"mbot-controller/internal/btlink"
)

// Source is implemented by anything that can generate command bytes.
type Source interface {
	Start(chan<- byte)
}

// Bridge connects a Source to a BLE session.
type Bridge struct {
	session *btlink.Session
	source  Source
}

// NewBridge constructs a Bridge.
func NewBridge(session *btlink.Session, source Source) *Bridge {
	return &Bridge{
		session: session,
		source:  source,
	}
}

// Run starts the source and forwards its bytes to the BLE session.
func (b *Bridge) Run() error {
	cmdCh := make(chan byte, 10)

	go b.source.Start(cmdCh)

	if err := b.session.Watch(func(data []byte) {
		log.Printf("[ROBOT BLE RX] %s", string(data))
	}); err != nil {
		log.Fatalf("[BRIDGE] notification subscription failed: %v", err)
	}

	for cmd := range cmdCh {
		n, err := b.session.Send([]byte{cmd})
		if err != nil {
			log.Fatalf("[BRIDGE] send failed: %v", err)
			continue
		}

		log.Printf("[BRIDGE] command sent: %s (%d bytes)", string(cmd), n)
	}

	return nil
}
