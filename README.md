# bks_server

C++/Qt TCP proxy server for hardware device control.

## Overview

`bks_server` is a console TCP server that receives client commands, processes them through a device/request layer and communicates with hardware using transport abstractions.

## Features

- TCP server based on Qt Network
- Client connection handling
- Request/response command processing
- INI-based configuration
- Hardware transport abstraction
- Reusable parser and requester modules

## Tech Stack

- C++
- Qt Core
- Qt Network
- QMake
- TCP/IP
- Serial/TCP transport

## Architecture

Client
   ↓
TCP server
   ↓
Proxy server
   ↓
Requester
   ↓
Transport layer
   ↓
Device
