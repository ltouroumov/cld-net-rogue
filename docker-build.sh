#!/bin/bash

BUILD_IMAGE="NO"
CLEAN_BEFORE="NO"

while getopts "ic" opt; do
    case "$opt" in
        i)
            BUILD_IMAGE="YES"
            ;;
        c)
            CLEAN_BEFORE="YES"
            ;;
        \?)
            echo "Unkown option -$OPTARG"
            ;;
    esac
done


[[ "$BUILD_IMAGE" == "YES" ]] && \
    docker build -t cld-rl-build .

docker run -i -v `realpath .`:/opt/game:Z cld-rl-build /bin/bash <<SCRIPT
    pushd /opt/game
    [[ "$CLEAN_BEFORE" == "YES" ]] && \
        ./scripts/clean-ext.sh -e ext-compat && \
        ./scripts/clean.sh
    ./scripts/setup.sh -e ext-compat
    ./scripts/package.sh -e ext-compat -C
SCRIPT
