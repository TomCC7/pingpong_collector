.PHONY: generate

generate: mcu-interface.ioc Src/main.cpp
	mv Src/main.cpp Src/main.c
	stm32pio generate
	mv Src/main.c Src/main.cpp