# Webserv Project - 1337

## Introduction

This project involves building a HTTP server in C++ 98 that adheres to the HTTP 1.1 protocol. The server should handle various HTTP methods, serve static websites, allow file uploads, and support CGI for dynamic content generation.

## General Rules

- **Stability:** The program must not crash under any circumstances, including out-of-memory conditions. Unexpected termination will result in a non-functional status.
- **Compilation:** Use C++ 98 standard with `-std=c++98` flag. Code should compile without errors or warnings with `-Wall -Wextra -Werror`.
- **Features:** Utilize modern C++ features where applicable, preferring `<cstring>` over `<string.h>` and using object-oriented programming principles.
- **External Libraries:** No external libraries or Boost libraries are allowed.
- **Error Handling:** Properly manage errors using system calls like `strerror` and `errno`.
- **Documentation:** Include comprehensive documentation in the code and README for clarity and maintainability.

## Requirements

- **Server Features:** Implement a fully functional HTTP server supporting:
  - Multiple ports and hosts configurations.
  - Handling of GET, POST, and DELETE HTTP methods.
  - Static file serving with directory listings.
  - CGI execution for dynamic content generation (e.g., PHP).
  - File uploads and configurable storage paths.
  - Default error pages for common HTTP status codes.

- **HTTP Compliance:** Ensure compliance with HTTP 1.1 standards, including accurate handling of status codes, headers, and response formats.
  
- **Non-Blocking I/O:** Use non-blocking I/O operations throughout the server implementation, utilizing a single `poll()` (or equivalent) instance for efficient event handling.

- **Configuration:** Read server configurations from a specified configuration file allowing:
  - Definition of server ports, hosts, and server names.
  - Setup of default error pages.
  - Limitation of client request body size.
  - Configuration of routes with rules for HTTP methods, redirects, directories, default files, CGI execution rules, and file uploads.

## Configuration File

- **Format:** The configuration file follows a format similar to NGINX's server configuration.
- **Options:** Configuration options include:
  - Server ports and hosts settings.
  - Definition of server names and default server setups.
  - Configuration of error pages and client request body size limits.
  - Route definitions specifying HTTP method handling, redirects, directory settings, default files, CGI execution rules, and file upload configurations.

## Implementation Details

- **Architecture:** Implement a modular architecture with clear separation of concerns (e.g., networking, HTTP protocol handling, file operations).
- **Socket Programming:** Utilize low-level socket programming techniques for handling network connections and data exchange.
- **Event Handling:** Use `poll()` (or equivalent) for managing I/O events and multiplexing connections.
- **CGI Execution:** Implement CGI support based on file extensions and handle input/output redirection as per CGI specifications.
- **Security Considerations:** Ensure server robustness against malicious input, buffer overflows, and denial-of-service attacks.
- **Performance Optimization:** Optimize server performance through efficient resource management, asynchronous I/O, and minimal overhead.

## Usage

```bash
./webserv [configuration_file]
```

## Testing

- Manual Testing: Perform manual testing using different web browsers and HTTP clients to verify functionality and compatibility.
- Automated Testing: Develop automated tests using scripting languages (e.g., Python, Bash) or dedicated testing frameworks to validate server behavior under stress and edge cases.
- Comparison: Compare server behavior and responses with established HTTP servers like NGINX for conformity and performance benchmarks.

## Screenshots

**Server:**
<img width="1883" alt="Screen Shot 2024-07-11 at 10 41 07 AM" src="https://github.com/belkdioui/Webserv-1337/assets/76043881/7fa1cb7f-933b-4e2c-9515-c8fa8cb6c9f9">

**Upload:**
<img width="1885" alt="Screen Shot 2024-07-11 at 10 41 30 AM" src="https://github.com/belkdioui/Webserv-1337/assets/76043881/2275e90e-378a-40db-9400-5b38844a5452">

**Cgi:**
<img width="1882" alt="Screen Shot 2024-07-11 at 10 44 18 AM" src="https://github.com/belkdioui/Webserv-1337/assets/76043881/8d4eea3b-1546-4aa4-a797-d02fe247e557">

**Wrong Url:**
<img width="1883" alt="Screen Shot 2024-07-11 at 10 45 00 AM" src="https://github.com/belkdioui/Webserv-1337/assets/76043881/4dad7234-0a61-4e4e-a430-8c69349f33d4">


## Contributing

Contributions are welcome! Fork the repository, make improvements, and submit a pull request. Ensure code quality, maintain documentation, and adhere to project guidelines.
