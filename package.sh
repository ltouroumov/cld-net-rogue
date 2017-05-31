#!/bin/bash

get_version() {
    if [[ ! -f .version ]]; then
        echo "0" > .version
    fi
    vers=`cat .version`
    if [[ "$1" == "YES" ]]; then
        next_vers=$(( $vers + 1 ))
        echo $next_vers | tee .version
    else
        cat .version
    fi
}

mkdir_s () {
    [[ ! -d "$1" ]] && mkdir -p "$1"
}

DO_UPLOAD="NO"
DO_BUILD="YES"

source_root=`realpath .`
build_root=`realpath ./build`


while getopts "b:Bu" opt; do
    case "$opt" in
        b)
            build_root=`realpath $OPTARG`
            ;;
        B)
            DO_BUILD="NO"
            ;;
        u)
            DO_UPLOAD="YES"
            ;;
        \?)
            echo "Invalid Option!"
            exit 1
            ;;
    esac
done

build_dest=$build_root/pack

mkdir_s $build_root
mkdir_s $build_dest

version=`get_version $DO_BUILD`

echo "[I] AWS Upload Script"
echo "[I] Version: $version"
echo "[I] Build path: $build_root"
echo "[I] Build dest: $build_dest"

if [[ "$DO_BUILD" == "YES" ]]; then
    echo "[+] Building Server"
    pushd $build_root
    cmake -DBUILD_SERVER=ON $source_root
    make prez_game_server -j 4
    popd

    echo "[+] Copying Files"
    pushd $build_dest
    cp -rv $build_root/server/prez_game_server $build_dest/server
    cp -rv $build_root/server/data $build_dest
    [[ -f $source_root/install.sh ]] && \
        cp -rv $source_root/install.sh $build_dest
    popd
fi

if [[ "$DO_UPLOAD" == "YES" ]]; then
    echo "[+] Uploading"
    aws --profile heig gamelift upload-build \
        --name cld-netrl \
        --build-version $version \
        --operating-system AMAZON_LINUX \
        --region us-east-1 \
        --build-root $build_dest
fi
echo "[+] Done!"
