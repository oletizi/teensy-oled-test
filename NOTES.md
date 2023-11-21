## USB MIDI

To get USB MIDI to work, you have to set --build-property build.usbtype=USB_MIDI_SERIAL or --build-property
build.usbtype=USB_MIDI option in arduino-cli.

If you __change__ the build.subtype, you have to pass --clean to arduino-cli or it will use whatever it was set to
previously and likely won't build.
