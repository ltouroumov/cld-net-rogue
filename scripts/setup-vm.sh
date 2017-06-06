#!/bin/bash

yum update -y
yum install -y epel-release

yum group install -y development
yum install -y asio-devel spdlog-devel curl-devel zlib-devel openssl-devel wget

pushd /tmp
if [[ ! -d "cmake-3.8.1" ]]; then
    wget "https://cmake.org/files/v3.8/cmake-3.8.1.tar.gz"
    tar xf cmake-3.8.1.tar.gz
fi
pushd cmake-3.8.1
./configure --system-curl
gmake install
popd
popd

