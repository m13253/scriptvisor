.PHONY: all install bin/scriptvisor

DESTDIR=
PREFIX=/usr/local

all: bin/scriptvisor

install: bin/scriptvisor assets/icon.png scriptvisor-autostart.desktop scriptvisor-autostart.sh
	install -Dm755 bin/scriptvisor $(DESTDIR)$(PREFIX)/bin/scriptvisor
	install -Dm644 assets/icon.png $(DESTDIR)/usr/share/icons/hicolor/256x256/apps/scriptvisor.png
	install -Dm644 assets/icon.png $(DESTDIR)/usr/share/pixmaps/scriptvisor.png
	install -Dm644 scriptvisor-autostart.desktop $(DESTDIR)/etc/xdg/autostart/scriptvisor-autostart.desktop
	install -Dm755 scriptvisor-autostart.sh $(DESTDIR)$(PREFIX)/bin/scriptvisor-autostart

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/scriptvisor
	rm -f $(DESTDIR)$(PREFIX)/bin/scriptvisor-autostart
	rm -f $(DESTDIR)/etc/xdg/autostart/scriptvisor-autostart.desktop
	rm -f $(DESTDIR)/usr/share/icons/hicolor/256x256/apps/scriptvisor.png
	rm -f $(DESTDIR)/usr/share/pixmaps/scriptvisor.png

bin/scriptvisor:
	mkdir -p build
	qmake-qt5 -o build/Makefile
	$(MAKE) -C build
	install -Dm755 build/scriptvisor $@

clean:
	rm -rf bin/scriptvisor build
