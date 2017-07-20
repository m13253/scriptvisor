.PHONY: all bin/Scriptvisor

all: bin/Scriptvisor

bin/Scriptvisor:
	mkdir -p build
	qmake-qt5 -o build/Makefile
	$(MAKE) -C build
	install -Dm755 build/Scriptvisor $@

clean:
	rm -rf bin/Scriptvisor build
