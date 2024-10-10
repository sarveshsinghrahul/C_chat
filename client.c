// client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define BUFFER_SIZE 1024

void handleError(const char* errorMessage) {
    printf("Error: %s (Error code: %d)\n", errorMessage, WSAGetLastError());
    WSACleanup();
    system("pause");  // Pause to view the error before the program exits
    exit(1);
}

// Function to clear input buffer
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

int main() {
    WSADATA wsa;
    SOCKET clientSocket;
    struct sockaddr_in serverAddr;
    char serverAddress[100];
    char buffer[BUFFER_SIZE];
    int port, recvSize;

    // Get server address and port from the user
    printf("Enter the server address (e.g., 127.0.0.1 or ngrok URL): ");
    scanf("%s", serverAddress);
    printf("Enter the port number: ");
    scanf("%d", &port);

    // Clear input buffer after scanf to prevent issues with fgets
    clearInputBuffer();

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        handleError("WSAStartup failed.");
    }

    // Create client socket
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        handleError("Socket creation failed.");
    }

    // Prepare server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(serverAddress);  // Convert address string to binary
    serverAddr.sin_port = htons(port);  // Convert port to network byte order

    // Connect to server
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        handleError("Connection failed.");
    }
    printf("Connected to server at %s:%d\n", serverAddress, port);

    // Chat loop
    while (1) {
        printf("Client: ");
        fgets(buffer, BUFFER_SIZE, stdin);  // Get user input
        send(clientSocket, buffer, strlen(buffer), 0);  // Send input to server

        recvSize = recv(clientSocket, buffer, BUFFER_SIZE, 0);  // Receive server response
        if (recvSize == SOCKET_ERROR) {
            handleError("recv failed.");
        }

        buffer[recvSize] = '\0';  // Null-terminate the received string
        printf("Server: %s\n", buffer);
    }

    // Cleanup
    closesocket(clientSocket);
    WSACleanup();
    return 0;
}
