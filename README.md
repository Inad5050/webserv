# Webserv: Your Own HTTP Server with C++98

Colaborador: ![David Felipe Munilla](https://github.com/PIPEFD)

## About The Project

[cite_start]This project involves creating an HTTP web server from scratch, compatible with C++98[cite: 37, 95]. [cite_start]The goal is to implement the essential components of the HTTP/1.1 protocol to handle requests from clients, such as a web browser, concurrently and in a non-blocking manner.

The server is capable of:
* [cite_start]Handling multiple client connections simultaneously using I/O multiplexing (`poll`, `select`, `kqueue`, or `epoll`)[cite: 99, 115].
* [cite_start]Processing and responding to **GET**, **POST**, and **DELETE** HTTP methods[cite: 136].
* [cite_start]Serving static content (HTML, CSS, images, etc.) from a specified root directory[cite: 131].
* [cite_start]Managing file uploads to the server[cite: 132, 166].
* [cite_start]Executing CGI scripts (e.g., in Python or PHP) to generate dynamic content.
* [cite_start]Being highly configurable through a text file inspired by NGINX's syntax[cite: 155].

---

## Usage

1.  **Compile the project:**
    ```bash
    make
    ```

2.  **Run the server:**
    ```bash
    ./webserv [path/to/your_config_file.conf]
    ```
    If no configuration file is provided, the server will use a default one.

---

## Introduction to the HTTP Protocol

[cite_start]**HTTP (Hypertext Transfer Protocol)** is the foundation for data communication on the World Wide Web[cite: 16]. [cite_start]It functions as a request-response protocol in the client-server model[cite: 20]. [cite_start]A client (like a web browser) sends an HTTP request to a server, and the server returns an HTTP response[cite: 23].

### HTTP Message Format

Both HTTP requests and responses share a similar structure:
1.  **Start-line**: Contains the method, URI, and HTTP version for requests, or the HTTP version, status code, and status phrase for responses.
2.  **Headers**: `key: value` pairs that convey additional information about the message.
3.  **Blank Line (CRLF)**: Separates the headers from the message body.
4.  **Message Body**: Optional content of the request or response (e.g., form data, HTML file content).

### HTTP Request

An example of an HTTP request:

```http
GET /index.html HTTP/1.1
Host: localhost:8080
User-Agent: Mozilla/5.0 (X11; Linux x86_64)
