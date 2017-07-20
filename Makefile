.PHONY: all install bin/scriptvisor

DESTDIR=
PREFIX=/usr/local

all: bin/scriptvisor

install: bin/scriptvisor scriptvisor-autostart.sh scriptvisor-autostart.desktop
	install -Dm755 bin/scriptvisor $(DESTDIR)$(PREFIX)/bin/scriptvisor
	install -Dm755 scriptvisor-autostart.sh $(DESTDIR)$(PREFIX)/bin/scriptvisor-autostart
	install -Dm644 scriptvisor-autostart.desktop $(DESTDIR)/etc/xdg/autostart/scriptvisor-autostart.desktop

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/scriptvisor
	rm -f $(DESTDIR)$(PREFIX)/bin/scriptvisor-autostart
	rm -f $(DESTDIR)/etc/xdg/autostart/scriptvisor-autostart.desktop

bin/scriptvisor:
	mkdir -p build
	qmake-qt5 -o build/Makefile
	$(MAKE) -C build
	install -Dm755 build/scriptvisor $@

clean:
	rm -rf bin/scriptvisor build
