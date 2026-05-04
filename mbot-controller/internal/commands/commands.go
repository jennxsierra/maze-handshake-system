package commands

// Entry describes a single robot command.
type Entry struct {
	Code byte
	Help string
}

// Registry lists every supported command byte and its meaning.
var Registry = map[byte]Entry{
	'X': {Code: 'X', Help: "Toggle RGB LED"},
	'F': {Code: 'F', Help: "Move robot forward"},
	'B': {Code: 'B', Help: "Move robot backward"},
	'L': {Code: 'L', Help: "Rotate robot 90 degrees left"},
	'R': {Code: 'R', Help: "Rotate robot 90 degrees right"},
	'1': {Code: '1', Help: "Nudge robot left"},
	'2': {Code: '2', Help: "Nudge robot right"},
	'S': {Code: 'S', Help: "Stop the robot"},
	'A': {Code: 'A', Help: "Switch robot to autonomous mode"},
	'M': {Code: 'M', Help: "Return robot to manual mode"},
}

// KeyboardLayout controls the order used when showing keyboard help.
var KeyboardLayout = []byte{
	'X',
	'F',
	'B',
	'L',
	'R',
	'1',
	'2',
	'S',
	'A',
	'M',
}

// PadBindings maps an SDL control name to a command byte.
var PadBindings = map[string]byte{
	"x":             'X',
	"a":             'F',
	"b":             'B',
	"leftshoulder":  'L',
	"rightshoulder": 'R',
	"lefttrigger":   '1',
	"righttrigger":  '2',
	"start":         'A',
	"back":          'M',
}

// PadLabels maps control names to a display-friendly label.
var PadLabels = map[string]string{
	"x":             "X Button",
	"a":             "A Button",
	"b":             "B Button",
	"leftshoulder":  "Left Shoulder",
	"rightshoulder": "Right Shoulder",
	"lefttrigger":   "Left Trigger",
	"righttrigger":  "Right Trigger",
	"start":         "Start Button",
	"back":          "Back Button",
}

// PadLayout defines the order used when printing controller help.
var PadLayout = []string{
	"x",
	"a",
	"b",
	"leftshoulder",
	"rightshoulder",
	"lefttrigger",
	"righttrigger",
	"start",
	"back",
}
