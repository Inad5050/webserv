<h1 align="center">Webserv - Proyecto 42</h1>

Colaborador: ![David Felipe Munilla](https://github.com/PIPEFD)

Este proyecto consiste en implementar un servidor HTTP desde cero en C++, sin el uso de bibliotecas externas como Boost. El objetivo es comprender y construir la arquitectura fundamental de un servidor web como Nginx o Apache, respetando los estándares HTTP/1.1 y los requerimientos del subject.

## ✅ Requisitos

    Configuración: Soporte para archivos de configuración tipo Nginx (.conf).

    Múltiples Puertos: Capacidad para escuchar en varios puertos e IPs simultáneamente.

    Concurrencia: Manejo de múltiples clientes a la vez mediante epoll (o kqueue).

    Métodos HTTP: Soporte para GET, POST y DELETE.

    Autoindex: Habilitado si no se encuentra un archivo index en un directorio.

    Redirecciones: Manejo de redirecciones 301 y 302.

    Códigos de Estado: Gestión correcta de códigos de estado HTTP (200, 404, 500, etc.).

    Páginas de Error: Soporte para páginas de error personalizadas.

    CGI: Ejecución de scripts CGI (PHP, Python, etc.) para generar contenido dinámico.

    Uploads: Gestión de subida de archivos a través de POST.

    Gestión de Memoria: Liberación completa de recursos y sin fugas de memoria.

## 🧱 Arquitectura

La arquitectura elegida es un modelo Event-Driven (Dirigido por Eventos), utilizando epoll para el manejo eficiente de I/O asíncrono. Este enfoque es altamente escalable y es el núcleo de servidores de alto rendimiento como Nginx.

flowchart TD

    A[Web Server] --> B[Parser .conf]
    
    B --> C{Server Sockets}
    
    C --> D[Event Loop (epoll)]
    
    D --> E{Request Handler}
    
    E --> F[Request Parsing]
    
    F --> G{Router}
    
    G --> H[Static File Handler]
    
    G --> I[CGI Handler]
    
    G --> J[Upload Handler]
    
    subgraph Response Generation
    
        H --> K[Response Builder]
        
        I --> K
        
        J --> K
        
    end
    
    K --> D

Flujo de una Petición:

    Inicio del Servidor: El programa primero parsea el archivo de configuración (.conf) para establecer los puertos, rutas y otras directivas.

    Creación de Sockets: Se crean los sockets de escucha para cada server block definido y se añaden al monitor de epoll.

    Loop de Eventos: El servidor entra en un bucle infinito esperando eventos de I/O con epoll_wait().

    Gestión de Conexión:

        Si un socket de escucha recibe una conexión, se acepta y el nuevo socket del cliente se añade a epoll.

        Si un socket de cliente tiene datos para leer, el Request Handler lee la petición HTTP.

    Procesamiento:

        La petición es parseada para extraer el método, la URI, las cabeceras y el cuerpo.

        El Router determina qué handler debe gestionar la petición basándose en la ruta y la configuración.

        El handler correspondiente (estático, CGI, upload) procesa la petición.

    Generación de Respuesta: El handler construye una respuesta HTTP, que se escribe de vuelta en el socket del cliente.

    Cierre: La conexión se cierra o se mantiene viva (keep-alive) según lo especificado en las cabeceras.

## 🌲 Estructura del Proyecto

webserv/

│

├── Makefile

├── README.md

│

├── config/

│   └── default.conf

│

├── data/

│   ├── uploads/

│   └── cgi-bin/

│       └── script.php

│

├── www/

│   ├── index.html

│   └── error_pages/

│       └── 404.html

│

├── include/

│   ├── Server.hpp

│   ├── Request.hpp

│   ├── Response.hpp

│   ├── ConfigParser.hpp

│   └── CgiHandler.hpp

│

└── src/

    ├── main.cpp
    
    ├── Server.cpp
    
    ├── Request.cpp
    
    ├── Response.cpp
    
    ├── ConfigParser.cpp
    
    └── CgiHandler.cpp

## 🚀 Uso

Para compilar y ejecutar el servidor, usa make. Por defecto, se configurará según config/default.conf.

$ make
$ ./webserv

## Navegador

Para conectarte al servidor desde un navegador, usa la dirección y el puerto especificados en tu archivo de configuración. Por ejemplo:
`http://localhost:8080`

## 📚 Documentación Oficial Relevante

* HTTP/1.1: RFC 7230-7235

* CGI: RFC 3875

* Sockets API: man socket, man bind, man listen, man accept

* I/O Multiplexing: man epoll
