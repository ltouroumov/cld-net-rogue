#!/bin/bash

pushd () {
    command pushd "$@" > /dev/null
}

popd () {
    command popd > /dev/null
}

mkcd () {
    [[ ! -d "$1" ]] && mkdir -p $1
    pushd $1
}

copy_libs () {
    typ=$1
    dir=$2
    dest=$3
    find $dir -name "*.$typ" -exec cp \{\} $dest \;
}

build_gamelift_server () {
    gamelift_release="3.1.5"
    mkcd aws-gamelift-server
    if [[ ! -f gamelift-server.zip ]]; then
        echo "[+] Downloading SDK"
        wget -qO gamelift-server.zip "https://s3-us-west-2.amazonaws.com/gamelift-release/GameLift_04_11_2017.zip"
        unzip -q gamelift-server.zip
    fi
    pushd GameLift-SDK-Release-$gamelift_release/GameLift-Cpp-ServerSDK-$gamelift_release
    mkcd build
    cmake \
        -DUSE_SYSTEM_BOOST=OFF \
        -DUSE_SYSTEM_SIOCLIENT=OFF \
        -DUSE_SYSTEM_PROTOBUF=OFF \
        -DCMAKE_INSTALL_PREFIX=./prefix \
        ..
    make -j 6
    copy_libs "a" prefix $lib_dir
    copy_libs "la" prefix $lib_dir
    popd
    popd
    popd
}

build_aws_sdk () {
    if [[ ! -d aws-sdk-cpp ]]; then
        git clone https://github.com/aws/aws-sdk-cpp.git
    fi
    pushd aws-sdk-cpp
    mkcd build
    cmake \
        -DBUILD_ONLY="core;gamelift" \
        -DCMAKE_INSTALL_PREFIX=./prefix \
        -DBUILD_SHARED_LIBS=ON \
        -DCUSTOM_MEMORY_MANAGEMENT=0 \
        ..
    make -j 6 install
    copy_libs "so" prefix $lib_dir
    popd
    popd
}

build_yaml_cpp () {
    if [[ ! -d yaml-cpp ]]; then
        git clone https://github.com/jbeder/yaml-cpp.git
    fi
    pushd yaml-cpp
    git checkout release-0.5.3
    mkcd build
    cmake \
        -DBUILD_SHARED_LIBS=OFF \
        -DYAML_CPP_BUILD_TOOLS=OFF \
        -DCMAKE_INSTALL_PREFIX=./prefix \
        ..
    make -j 6 install
    copy_libs "a" prefix $lib_dir
    popd
    popd
}

build_rltk () {
    if [[ ! -d rltk ]]; then
        git clone https://github.com/thebracket/rltk.git
    fi
    pushd rltk
    mkcd build
    cmake \
        -DCMAKE_INSTALL_PREFIX=./prefix \
        ..
    make -j 6 install
    copy_libs "a" prefix $lib_dir
    popd
    popd
}

echo "[+] Setting up vendors"
lib_dir=`realpath ext/lib`
mkcd ext/src

echo "[+] aws-cpp-sdk-gamelift-server"
build_gamelift_server

echo "[+] yaml-cpp"
build_yaml_cpp

if [[ -n "$BUILD_CLIENT_LIBS" ]]; then
    echo "[+] aws-cpp-sdk-(core,gamelift)"
    build_aws_sdk

    echo "[+] rltk"
    build_rltk
fi
