#!/bin/bash

echo "[+] Cleaning"

rm -rv build
rm -rv pak

if [[ "$1" == "-a" ]]; then
    rm -rv ext/src
fi
