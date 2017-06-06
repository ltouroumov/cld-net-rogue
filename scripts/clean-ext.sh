#!/bin/bash

rm_s () {
    [[ -d "$1" ]] && rm -rf "$1"
}

ext_path=`realpath ./ext`

while getopts "e:" opt; do
    case "$opt" in
        e)
            ext_path=`realpath $OPTARG`
            ;;
        \?)
            echo "Unkown Argument -$OPTARG"
            ;;
    esac
done


echo "[+] Cleaning Vendors Builds"

gamelift_version="3.1.5"

rm_s $ext_path/src/aws-sdk-cpp/build
rm_s $ext_path/src/aws-gamelift-server/GameLift-SDK-Release-$gamelift_version/GameLift-Cpp-ServerSDK-$gamelift_version/build
rm_s $ext_path/src/rltk/build
rm_s $ext_path/src/yaml-cpp/build
