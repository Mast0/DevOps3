FROM debian:bullseye-slim
RUN apt-get update && apt-get install -y g++ make
COPY . /app
WORKDIR /app
RUN make
RUN g++ -o http_server HTTP_Server.cpp arctan.cpp -lpthread
EXPOSE 8081
CMD ["./http_server"]
