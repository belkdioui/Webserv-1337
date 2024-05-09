# Error Pages

First, we need to explain what is status code becuse Error Pages just **HTTP status code**.  Status code refer to the HTTP status code thet the server return to the client in response to a request. These status codes provide informations about the success, failure, or other conditions related to the request and response cycle.
There are many status codes like **1xx Informational**, **2xx Success**, **3xx Redirection**, **4xx Client Error** and **5xx Server Error**. Now we are focusing to **4xx client Error** and **5xx Server Error**.

## 4xx client Errors
These status code are returned when the client's request contains incorrect syntax or cannot be fulfilled.

### 400 Bad Request 

>The **400 Bad Request** status code is an HTTP response status code that indicates the server cannot process the client's request due to malformed syntax or invalid parameters in the request. When a server returns a **400 Bad Request** response, it means that the client's request couldn't be understood by the server because it violates the rules of the HTTP protocol. ***Server cannot process the request due to a client error.***

### 401 Unauthorized

>The **401 Unauthorized** status code is an HTTP response status code that indicates the client's request lacks proper authentication credentials or authorization to access the requested resource. When a server returns a **401 Unauthorized** response, it means that the client needs to provide valid credentials (such as a username and password) or authentication tokens to access the resource. ***Authentication is required to access the resource.***

### 403 Forbidden 

>  The **403 Forbidden** status code is an HTTP response status code that indicates the server understood the client's request, but the server is refusing to fulfill the request due to permissions or access restrictions. When a server returns a **403 Forbidden** response, it means that the client's credentials, permissions, or the nature of the request itself are not sufficient to access the requested resource. ***Client does not have permission to access the resource.***

### 404 Not Found

> The **404 Not Found** status code is an HTTP response status code that indicates the server could not find the requested resource. When a server returns a **404 Not Found** response, it means that the client's request specified a URL or resource that does not exist on the server. ***Resource could not be found on the server.***

### 405 Method Not Allowed

> The **405 Method Not Allowed** status code is an HTTP response status code that indicates that the server received a request with an HTTP method that is not allowed or supported for the specified resource. When a server returns a **405 Method Not Allowed** response, it means that the client used an HTTP method that the server does not permit for the requested resource. ***Request method (GET, POST, etc.) is not allowed for the resource.***


## 5xx Server Errors
These status codes indicate that the server encountered an error while processing the request and was unable to fulfill it

### 500 internal Server Error
>   The **500 Internal Server Error** is an HTTP status code that indicates an unexpected condition occurred on the server while processing the client's request . ***Generic server error response.***

### 501 Not Implemented 
> The **501 Not Implemented** status code is an HTTP response status code that indicates that the server does not support the functionality required to fulfill the client's request. When a server returns a **501 Not Implemented** response, it means that the server does not recognize or cannot handle the specific method used in the request. ***Server doesn't support the functionality required to fulfill the request.***
### 502 Bad Getway
> The **502 Bad Gateway** status code is an HTTP response status code that indicates that a server acting as a gateway or proxy received an invalid response from the upstream server it accessed while attempting to fulfill the client's request. This error typically occurs in the context of a server acting as a gateway or proxy, passing requests between the client and another server (such as an application server, API server, or caching server). ***Server received an invalid response from an upstream server.***

### 503 Service Unavailable

> The **503 Service Unavailable** status code is an HTTP response status code that indicates the server is temporarily unable to handle the request due to overload or maintenance. When a server returns a **503 Service Unavailable** response, it means that the server cannot process the request at the moment but expects to be able to do so in the future. ***Server is temporarily unavailable (often due to maintenance or overload).***


### 504 Getway Timeout

> The **504 Gateway Timeout** status code is an HTTP response status code that indicates that a server acting as a gateway or proxy did not receive a timely response from an upstream server while attempting to fulfill the client's request. This error typically occurs when the server acting as a gateway or proxy times out while waiting for a response from the upstream server. ***Server didn't receive a timely response from an upstream server.***