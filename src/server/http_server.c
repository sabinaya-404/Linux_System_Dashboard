#include "http_server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

static void serve_file(int client_fd, const char *path, const char *content_type) {
    FILE *fp = fopen(path, "rb");
    if (!fp) {
        const char *not_found = "HTTP/1.1 404 Not Found\r\nContent-Length: 9\r\n\r\nNot Found";
        send(client_fd, not_found, strlen(not_found), 0);
        return;
    }

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char header[256];
    snprintf(header, sizeof(header),
             "HTTP/1.1 200 OK\r\nContent-Type: %s\r\nContent-Length: %ld\r\nConnection: close\r\n\r\n",
             content_type, size);
    send(client_fd, header, strlen(header), 0);

    char buf[1024];
    size_t n;
    while ((n = fread(buf, 1, sizeof(buf), fp)) > 0) {
        send(client_fd, buf, n, 0);
    }
    fclose(fp);
}

int http_server_run(int port) {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket creation failed");
        return -1;
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Bind failed");
        close(server_fd);
        return -1;
    }

    if (listen(server_fd, 10) < 0) {
        perror("Listen failed");
        close(server_fd);
        return -1;
    }

    printf("\033[1;32m[+] Web Dashboard running at: http://localhost:%d\033[0m\n", port);

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
        if (client_fd < 0) continue;

        char req[1024];
        int bytes = recv(client_fd, req, sizeof(req) - 1, 0);
        if (bytes > 0) {
            req[bytes] = '\0';
            char method[16], uri[256];
            sscanf(req, "%15s %255s", method, uri);

            if (strcmp(uri, "/api/stats") == 0) {
                SystemSnapshot snap;
                collector_get_snapshot(&snap);

                char json_buf[4096];
                json_build_snapshot(&snap, json_buf, sizeof(json_buf));

                char resp[4500];
                snprintf(resp, sizeof(resp),
                         "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nAccess-Control-Allow-Origin: *\r\nContent-Length: %zu\r\nConnection: close\r\n\r\n%s",
                         strlen(json_buf), json_buf);
                send(client_fd, resp, strlen(resp), 0);
            } else if (strcmp(uri, "/") == 0 || strcmp(uri, "/index.html") == 0) {
                serve_file(client_fd, "web/index.html", "text/html");
            } else if (strcmp(uri, "/style.css") == 0) {
                serve_file(client_fd, "web/style.css", "text/css");
            } else if (strcmp(uri, "/app.js") == 0) {
                serve_file(client_fd, "web/app.js", "application/javascript");
            } else {
                const char *not_found = "HTTP/1.1 404 Not Found\r\nContent-Length: 9\r\n\r\nNot Found";
                send(client_fd, not_found, strlen(not_found), 0);
            }
        }
        close(client_fd);
    }

    close(server_fd);
    return 0;
}