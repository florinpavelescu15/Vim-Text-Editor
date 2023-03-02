all: build

build: editor.c
	gcc editor.c -o editor

clean:
	rm editor
	