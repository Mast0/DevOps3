bin_PROGRAMS = arctan_approx
arctan_approx_SOURCES = main.cpp arctan.cpp arctan.h

check_PROGRAMS = arctan_test
arctan_test_SOURCES = test.cpp arctan.cpp arctan.h

install-exec-hook:
	mkdir -p $(DESTDIR)/deb/DEBIAN
	echo "Package: arctan-approx" > $(DESTDIR)/deb/DEBIAN/control
	echo "Version: 1.0" >> $(DESTDIR)/deb/DEBIAN/control
	echo "Section: utils" >> $(DESTDIR)/deb/DEBIAN/control
	echo "Priority: optional" >> $(DESTDIR)/deb/DEBIAN/control
	echo "Architecture: any" >> $(DESTDIR)/deb/DEBIAN/control
	echo "Maintainer: Bohdan Remyha bogdanremuga@gmail.com" >> $(DESTDIR)/deb/DEBIAN/control
	echo "Description: A program to approximate arctangent values" >> $(DESTDIR)/deb/DEBIAN/control
