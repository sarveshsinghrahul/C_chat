// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define BUFFER_SIZE 1024

void handleError(const char* errorMessage) {
    printf("Error: %s\n", errorMessage);
    WSACleanup();
    exit(1);
}

int main() {
    WSADATA wsa;
    SOCKET serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    char buffer[BUFFER_SIZE];
    int addrLen = sizeof(clientAddr);
    int recvSize;
    int port;

    // Get the port number from the user
    printf("Enter the port number to listen on: ");
    scanf("%d", &port);

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        handleError("WSAStartup failed.");
    }

    // Create server socket
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        handleError("Socket creation failed.");
    }

    // Prepare server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    // Bind socket
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        handleError("Bind failed.");
    }

    // Listen for incoming connections
    listen(serverSocket, 3);
    printf("Server listening on port %d...\n", port);

    // Accept incoming connection
    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrLen);
    if (clientSocket == INVALID_SOCKET) {
        handleError("Connection acceptance failed.");
    }
    printf("Connection accepted.\n");

    // Chat loop
    while (1) {
        recvSize = recv(clientSocket, buffer, BUFFER_SIZE, 0);
        if (recvSize == SOCKET_ERROR) {
            handleError("recv failed.");
        }

        buffer[recvSize] = '\0';
        printf("Client: %s\n", buffer);

        printf("Server: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        send(clientSocket, buffer, strlen(buffer), 0);
    }

    // Cleanup
    closesocket(serverSocket);
    closesocket(clientSocket);
    WSACleanup();
    return 0;
}
