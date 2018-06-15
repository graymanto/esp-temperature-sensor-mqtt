#SHELL := /bin/bash
#PATH := /usr/local/bin:$(PATH)

all:
	platformio -f -c vim run

build-esp01:
	platformio -f -c vim run -e esp01

build-nmcu:
	platformio -f -c vim run -e nodemcuv2

upload:
	platformio -f -c vim run --target upload

upload-nmcu:
	platformio run -e nodemcuv2 -t upload

upload-esp01:
	platformio run -e esp01 -t upload

clean:
	platformio -f -c vim run --target clean

program:
	platformio -f -c vim run --target program

uploadfs:
	platformio -f -c vim run --target uploadfs

update:
	platformio -f -c vim update
