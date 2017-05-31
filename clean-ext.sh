#!/bin/bash

rm_s () {
    [[ -d "$1" ]] && rm -r "$1"
}

echo "[+] Cleaning Vendors Builds"

gamelift_version="3.1.5"

rm_s ext/src/aws-sdk-cpp/build
rm_s ext/src/aws-gamelift-server/GameLift-SDK-Release-$gamelift_version/GameLift-Cpp-ServerSDK-$gamelift_version/build
rm_s ext/src/rltk/build
rm_s ext/src/yaml-cpp/build
