# CLD - Networked Roguelike Demo

Networked Roguelike, using Amazon GameLift

## Requirements

  * CMake 3.8
  * C++14 compatible compiler
  * Docker (for gamelift compatible builds)
  * Patience

## How to build

  1. `git clone https://github.com/ltouroumov/cld-net-rogue.git`
  1. `cd cld-net-rogue`
  1. Copy `config.yaml.sample` to `config.yaml` and fill in blanks
  1. Local Build (for development)
    1. `yum install sfml-devel asio-devel spdlog-devel`
    1. `./scripts/setup.sh`
    1. `./scripts/package.sh`
  1. Compat Build (for gamelift)
    1. `./docker-build.sh`
    1. `./scripts/package.sh -B -u` (to upload to aws)
  
## How to run

  * `cd build/pack`
  * `mkdir logs`
  * `./server` or `./client`
  
## CMake options

  * `-DBUILD_SERVER:BOOL=<YES|NO>`
  * `-DBUILD_CLIENT:BOOL=<YES|NO>`
  * `-DUSE_REMOTE:BOOL=<YES|NO>` (`YES` for production builds, `NO` for development builds)
  * `-DLOG_STDOUT:BOOL=<YES|NO>` (Change logger to stdout or file)
