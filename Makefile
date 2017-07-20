all: bin/Scriptvisor

bin/Scriptvisor:
	mkdir -p build
	qbs setup-qt --detect
	qbs setup-toolchains --detect
	#qbs config defaultProfile qt-5
	qbs build -d build Scriptvisor.qbs release
	install -Dm755 build/Scriptvisor.qbs/install-root/Scriptvisor bin/Scriptvisor

clean:
	rm -rf build bin/Scriptvisor
