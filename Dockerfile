FROM debian:bullseye-slim
RUN apt-get update && apt-get install -y g++ make
WORKDIR /app
RUN git clone https://github.com/Mast0/DevOps3.git .
RUN make
RUN g++ -o http_server HTTP_Server.cpp arctan.cpp -lpthread


FROM alpine:latest
RUN apk add --no-cache libstdc++ libc6-compat
WORKDIR /app
COPY --from=build /app/http_server .
EXPOSE 8081
CMD ["./http_server"]
