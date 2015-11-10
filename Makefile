BOARD=arduino:avr:uno
SKETCH=ExpressCarduino.ino

all: upload

verify: $(SKETCH)
	arduino --verify --board $(BOARD) $(SKETCH)

upload: verify
	arduino --upload --board $(BOARD) $(SKETCH)

.PHONY: all verify upload
