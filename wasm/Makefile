CC=wat2wasm
SRC=./
FILES=$(wildcard ${SRC}*.wast) $(wildcard ${SRC}operations/*.wast)
OBJS=$(subst .wast,.wasm,$(FILES))

%.wasm: %.wast
	$(CC) $<

all: $(OBJS)

clean:
	rm *.wasm	
