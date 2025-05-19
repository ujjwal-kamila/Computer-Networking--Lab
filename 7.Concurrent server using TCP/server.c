#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

typedef struct {
    int sock;
    int id;
    char name[32];
} client_t;

client_t *clients[MAX_CLIENTS];
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void *handle_client(void *arg);
void handle_chat(client_t *client);
void send_message(int sender_id, int receiver_id, char *message);
void broadcast_message(int sender_id, char *message);
void list_clients(int client_id);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        return 1;
    }

    int port = atoi(argv[1]);
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    pthread_t tid;

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("Socket creation failed");
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        return 1;
    }

    if (listen(server_sock, MAX_CLIENTS) < 0) {
        perror("Listen failed");
        return 1;
    }

    printf("Chat server started on port %d\n", port);

    while (1) {
        client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &addr_len);
        if (client_sock < 0) {
            perror("Accept failed");
            continue;
        }

        client_t *client = (client_t *)malloc(sizeof(client_t));
        client->sock = client_sock;

        pthread_create(&tid, NULL, handle_client, (void *)client);
        pthread_detach(tid);
    }

    close(server_sock);
    return 0;
}

void *handle_client(void *arg) {
    client_t *client = (client_t *)arg;

    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (!clients[i]) {
            client->id = i + 1;
            sprintf(client->name, "Client %d", client->id);
            clients[i] = client;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);

    handle_chat(client);
    return NULL;
}

void handle_chat(client_t *client) {
    char buffer[BUFFER_SIZE];
    int bytes_read;

    sprintf(buffer, "You are connected as Client %d\nType /help for commands\n", client->id);
    send(client->sock, buffer, strlen(buffer), 0);

    while ((bytes_read = recv(client->sock, buffer, BUFFER_SIZE, 0)) > 0) {
        buffer[bytes_read] = '\0';

        if (buffer[0] == '/') {
            char *command = strtok(buffer, " \n");

            if (strcmp(command, "/list") == 0) {
                list_clients(client->id);
            } else if (strcmp(command, "/msg") == 0) {
                char *to_str = strtok(NULL, " ");
                char *message = strtok(NULL, "\n");
                if (to_str && message) {
                    int to_id = atoi(to_str);
                    send_message(client->id, to_id, message);
                } else {
                    sprintf(buffer, "Usage: /msg <client_id> <message>\n");
                    send(client->sock, buffer, strlen(buffer), 0);
                }
            } else if (strcmp(command, "/broadcast") == 0) {
                char *message = strtok(NULL, "\n");
                if (message) broadcast_message(client->id, message);
            } else if (strcmp(command, "/help") == 0) {
                sprintf(buffer, "Commands:\n/list\n/msg <id> <msg>\n/broadcast <msg>\n/help\n");
                send(client->sock, buffer, strlen(buffer), 0);
            } else {
                sprintf(buffer, "Unknown command\n");
                send(client->sock, buffer, strlen(buffer), 0);
            }
        } else {
            broadcast_message(client->id, buffer);
        }
    }

    printf("Client %d disconnected.\n", client->id);
    close(client->sock);

    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i] && clients[i]->id == client->id) {
            clients[i] = NULL;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);

    free(client);
}

void send_message(int sender_id, int receiver_id, char *message) {
    pthread_mutex_lock(&clients_mutex);
    char buffer[BUFFER_SIZE];
    client_t *sender = NULL, *receiver = NULL;

    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i]) {
            if (clients[i]->id == sender_id) sender = clients[i];
            if (clients[i]->id == receiver_id) receiver = clients[i];
        }
    }

    if (sender && receiver) {
        sprintf(buffer, "[Private from %d] %s\n", sender_id, message);
        send(receiver->sock, buffer, strlen(buffer), 0);
        sprintf(buffer, "[Private to %d] %s\n", receiver_id, message);
        send(sender->sock, buffer, strlen(buffer), 0);
    } else if (sender && !receiver) {
        sprintf(buffer, "Client %d not found\n", receiver_id);
        send(sender->sock, buffer, strlen(buffer), 0);
    }

    pthread_mutex_unlock(&clients_mutex);
}

void broadcast_message(int sender_id, char *message) {
    pthread_mutex_lock(&clients_mutex);
    char buffer[BUFFER_SIZE];
    client_t *sender = NULL;

    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i] && clients[i]->id == sender_id) {
            sender = clients[i];
            break;
        }
    }

    if (sender) {
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (clients[i] && clients[i]->id != sender_id) {
                sprintf(buffer, "[Broadcast from %d] %s\n", sender_id, message);
                send(clients[i]->sock, buffer, strlen(buffer), 0);
            }
        }
        sprintf(buffer, "[Broadcast sent]\n");
        send(sender->sock, buffer, strlen(buffer), 0);
    }

    pthread_mutex_unlock(&clients_mutex);
}

void list_clients(int client_id) {
    pthread_mutex_lock(&clients_mutex);
    char buffer[BUFFER_SIZE] = "Connected clients:\n", temp[64];

    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i]) {
            sprintf(temp, "%d. %s\n", clients[i]->id, clients[i]->name);
            strcat(buffer, temp);
        }
    }

    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i] && clients[i]->id == client_id) {
            send(clients[i]->sock, buffer, strlen(buffer), 0);
            break;
        }
    }

    pthread_mutex_unlock(&clients_mutex);
}
