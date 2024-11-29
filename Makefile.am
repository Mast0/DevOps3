bin_PROGRAMS = arctan_approx
arctan_approx_SOURCES = main.cpp arctan.cpp arctan.h

check_PROGRAMS = arctan_test
arctan_test_SOURCES = test.cpp arctan.cpp arctan.h

check_PROGRAMS = test_server
test_server_SOURCES = test_server.cpp
test_server_CPPFLAGS = -I/usr/include
test_server_LDADD = -lcurl

DOCKER_IMAGE = mast00/http_server_image
DOCKER_TAG = latest
DOCKER_REGISTRY = docker.io

install-exec-hook:
	mkdir -p $(DESTDIR)/deb/DEBIAN
	echo "Package: arctan-approx" > $(DESTDIR)/deb/DEBIAN/control
	echo "Version: 1.0" >> $(DESTDIR)/deb/DEBIAN/control
	echo "Section: utils" >> $(DESTDIR)/deb/DEBIAN/control
	echo "Priority: optional" >> $(DESTDIR)/deb/DEBIAN/control
	echo "Architecture: any" >> $(DESTDIR)/deb/DEBIAN/control
	echo "Maintainer: Bohdan Remyha bogdanremuga@gmail.com" >> $(DESTDIR)/deb/DEBIAN/control
	echo "Description: A program to approximate arctangent values" >> $(DESTDIR)/deb/DEBIAN/control

dockerfile: Dockerfile

Dockerfile: Makefile.am
	echo 'FROM debian:bullseye-slim' > Dockerfile
	echo 'RUN apt-get update && apt-get install -y g++ make' >> Dockerfile
	echo 'COPY . /app' >> Dockerfile
	echo 'WORKDIR /app' >> Dockerfile
	echo 'RUN make' >> Dockerfile
	echo 'RUN g++ -o http_server HTTP_Server.cpp arctan.cpp -lpthread' >> Dockerfile
	echo 'EXPOSE 8081' >> Dockerfile
	echo 'CMD ["./http_server"]' >> Dockerfile


docker-push:
	@echo "Building Docker image $(DOCKER_IMAGE):$(DOCKER_TAG)..."
	docker build -t $(DOCKER_IMAGE):$(DOCKER_TAG) .
	@echo "Pushing Docker image to registry $(DOCKER_REGISTRY)..."
	docker push $(DOCKER_IMAGE):$(DOCKER_TAG)
	@echo "Docker image pushed successfully!"

.PHONY: docker-push
