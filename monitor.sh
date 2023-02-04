#!/usr/bin/env zsh

arduino-cli monitor --fqbn teensy:avr:teensy41 -p "$(./find-port.sh)"