.PHONY: all build upload monitor

PORT := $(shell ./find-port.sh)

all: build upload

build:
	arduino-cli compile --fqbn teensy:avr:teensy41 .

upload:
	arduino-cli upload -p $(PORT) --fqbn teensy:avr:teensy41 . #&& arduino-cli monitor -p $(PORT)


board:
	arduino-cli board list