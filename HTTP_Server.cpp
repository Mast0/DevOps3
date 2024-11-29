#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <cstring>
#include <random>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include "arctan.h"

#define PORT 8081

char HTTP_200HEADER[] = "HTTP/1.1 200 Ok\r\n";
char HTTP_404HEADER[] = "HTTP/1.1 404 Not Found\r\n";
char HTTP_500HEADER[] = "HTTP/1.1 500 Internal Server Error\r\n";

void handleGET(int clientSocket, const char* path);

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(serverSocket, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Socket bind failed");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    if (listen(serverSocket, 10) < 0) {
        perror("Socket listen failed");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    std::cout << "HTTP Server started on port " << PORT << "\n";

    while (true) {
        std::cout << "Waiting for a new connection...\n";

        if ((clientSocket = accept(serverSocket, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
            perror("Connection accept failed");
            continue;
        }

        char buffer[30000] = {0};
        int bytesRead = read(clientSocket, buffer, sizeof(buffer));
        if (bytesRead <= 0) {
            perror("Error reading client request");
            close(clientSocket);
            continue;
        }

        std::cout << "Client request:\n" << buffer << "\n";

        char method[10] = {0};
        char path[256] = {0};
        sscanf(buffer, "%s %s", method, path);

        if (strcmp(method, "GET") == 0) {
            handleGET(clientSocket, path);
        } else {
            const char* response = "HTTP/1.1 405 Method Not Allowed\r\n\r\n";
            write(clientSocket, response, strlen(response));
        }

        close(clientSocket);
    }

    close(serverSocket);
    return 0;
}

void handleGET(int clientSocket, const char* path) {
    if (strcmp(path, "/compute") == 0) {
        auto start = std::chrono::high_resolution_clock::now();

        std::vector<double> values;
        int terms = 100;
        for (double x = 0.1; x <= 5000.0; x += 0.1) {
            values.push_back(arctan(x, terms));
        }

        for (int i = 0; i < 500; ++i) {
            std::sort(values.begin(), values.end());
            std::reverse(values.begin(), values.end());
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        char response[1024];
        snprintf(response, sizeof(response), "%sContent-Type: text/html\r\nContent-Length: %ld\r\n\r\nElapsed time: %ld ms\n",
                 HTTP_200HEADER, 25 + std::to_string(duration.count()).length(), duration.count());

        write(clientSocket, response, strlen(response));
        std::cout << "Response sent:\n" << response;
    } else {
        const char* response = "HTTP/1.1 404 Not Found\r\n\r\n";
        write(clientSocket, response, strlen(response));
    }
}
