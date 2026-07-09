# bks_server

C++/Qt TCP proxy server for hardware device control.

## Overview

`bks_server` is a console TCP server for receiving external client commands, processing them through a request/response layer and communicating with a hardware device through transport abstractions.

The project is designed for hardware-oriented software where a Qt-based service works as an intermediate layer between an external control system and a device connected through TCP or serial communication.

This repository demonstrates C++/Qt development experience in:

* TCP server applications;
* device command processing;
* request/response architecture;
* INI-based configuration;
* reusable transport abstractions;
* integration with a shared C++/Qt library.

## Features

* TCP server based on Qt Network
* Client connection handling
* Request/response command processing
* Hardware device abstraction
* TCP/Serial transport integration through `common_lib`
* INI-based configuration
* Console application architecture
* Reusable parser/requester/device components

## Tech Stack

* C++14
* Qt Core
* Qt Network
* qmake
* TCP/IP
* Serial/TCP transport
* Linux-oriented service development

## Project Structure

```text
bks_server/
├── etc/                       # Example configuration files
├── third_party/
│   └── common_lib/             # Shared C++/Qt library as a git submodule
├── bks_server.pro              # qmake project file
├── main.cpp                    # Application entry point
├── bks_proxy_server.*          # TCP proxy/server layer
├── bks_device.*                # Device-level logic
└── README.md
```

## Architecture

```text
External client
        ↓
QTcpServer / QTcpSocket
        ↓
BKS proxy server
        ↓
Requester / command layer
        ↓
Transport abstraction
        ↓
Hardware device
```

The server receives commands from external clients over TCP, converts them into internal request objects and forwards them to the device communication layer. Transport, requester and parser components are provided by `common_lib`.

## Dependency: common_lib

This repository uses `common_lib` as a git submodule.

`common_lib` provides reusable C++/Qt components for:

* network transport;
* serial transport;
* request/response processing;
* command parsing;
* device abstractions;
* protocol-related utilities.

Expected repository structure after cloning with submodules:

```text
bks_server/
├── third_party/
│   └── common_lib/
└── bks_server.pro
```

## Clone

Clone the repository with submodules:

```bash
git clone --recurse-submodules https://github.com/Unsainted38/bks_server.git
cd bks_server
```

If the repository was already cloned without submodules, initialize them manually:

```bash
git submodule update --init --recursive
```

## Build

### 1. Build common_lib

```bash
cd third_party/common_lib
qmake common_lib.pro
make
```

### 2. Build bks_server

```bash
cd ../..
qmake bks_server.pro
make
```

## Configuration

The server uses INI-based configuration.

Production configuration files are not included in the repository. Use `etc/config.example.ini` as a template for local setup.

The exact configuration depends on the target hardware device and deployment environment.

## Example Usage Scenario

A typical use case:

1. An external client connects to `bks_server` over TCP.
2. The client sends a command according to the device control protocol.
3. The server parses the command and creates an internal request.
4. The request is sent to the hardware device through a transport abstraction.
5. The response is received, parsed and returned to the client.

## What I Implemented

* Designed the Qt-based TCP proxy server structure.
* Implemented client connection handling through Qt Network.
* Integrated the server with reusable request/response components from `common_lib`.
* Implemented device-level communication logic.
* Added INI-based configuration support.
* Prepared the project for reuse with `common_lib` as an external submodule dependency.

## Known Limitations

* The project currently uses qmake.
* Real hardware communication requires a compatible device or test environment.
* Some protocol details may depend on the target hardware configuration.
* The repository is intended as a portfolio/code review version of a hardware-oriented service.
* CMake support and automated CI build may be added later.

## Repository Purpose

This repository demonstrates experience with:

* C++/Qt console service development;
* TCP server implementation;
* hardware control software;
* network protocol processing;
* request/response architecture;
* reusable C++/Qt library integration;
* Linux-oriented deployment scenarios.
