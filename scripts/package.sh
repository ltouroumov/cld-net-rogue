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
DO_SERVER="YES"
DO_CLIENT="YES"

source_root=`realpath .`
build_root=`realpath ./build`
ext_path=`realpath ./ext`

while getopts "b:e:BSCu" opt; do
    case "$opt" in
        b)
            build_root=`realpath $OPTARG`
            ;;
        e)
            ext_path=`realpath $OPTARG`
            ;;
        B)
            DO_SERVER="NO"
            DO_CLIENT="NO"
            ;;
        u)
            DO_UPLOAD="YES"
            ;;
        S)
            DO_SERVER="NO"
            ;;
        C)
            DO_CLIENT="NO"
            ;;
        \?)
            echo "usage: package.sh -b <build_path> -e <ext_path> -BSCu"
            echo "  -B : Do not build anything"
            echo "  -S : Do not build server"
            echo "  -C : Do not build client"
            echo "  -u : Upload to cloud"
            echo
            exit 1
            ;;
    esac
done

build_dest=$build_root/pack

mkdir_s $build_root
mkdir_s $build_dest

version=`get_version $DO_SERVER`

echo "[I] AWS Upload Script"
echo "[I] Version: $version"
echo "[I] Build path: $build_root"
echo "[I] Build dest: $build_dest"

if [[ "$DO_SERVER" == "YES" || "$DO_CLIENT" == "YES" ]]; then
    pushd $build_root
    echo "[+] Configuring Build"
    cmake -DBUILD_SERVER:BOOL=$DO_SERVER -DBUILD_CLIENT:BOOL=$DO_CLIENT -DUSE_REMOTE:BOOL=YES -DEXT_PATH=$ext_path $source_root
    if [[ "$DO_SERVER" == "YES" ]]; then
        echo "[+] Building Server"
        make prez_game_server -j 4
    fi
    if [[ "$DO_CLIENT" == "YES" ]]; then
        echo "[+] Building Client"
        make prez_game_client
    fi
    popd

    echo "[+] Copying Files"
    pushd $build_dest
    [[ -f $build_root/server/prez_game_server ]] && cp -rv $build_root/server/prez_game_server $build_dest/server
    [[ -f $build_root/client/prez_game_client ]] && cp -rv $build_root/client/prez_game_client $build_dest/client
    [[ -d $source_root/shared/data ]] && cp -rv $source_root/shared/data $build_dest
    if [[ "$DO_CLIENT" == "YES" ]]; then
        [[ -d $build_root/client/assets ]] && cp -rv $build_root/client/assets $build_dest
        [[ -f $source_root/client/config.yaml ]] && cp -rv $source_root/client/config.yaml $build_dest
    fi
    [[ -f $source_root/install.sh ]] && cp -rv $source_root/install.sh $build_dest
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
