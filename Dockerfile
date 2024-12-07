FROM debian:bullseye-slim AS build
RUN apt-get update && apt-get install -y g++ make git autoconf
WORKDIR /app
RUN git clone https://github.com/Mast0/DevOps3.git .
RUN git checkout branchHTTPserverMulti
RUN autoreconf -i && ./configure
RUN make
RUN g++ -o http_server HTTP_Server.cpp arctan.cpp -lpthread


FROM alpine:latest
RUN apk add --no-cache libstdc++ libc6-compat
WORKDIR /app
COPY --from=build /app/http_server .
EXPOSE 8081
CMD ["./http_server"]
