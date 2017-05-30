#!/bin/bash

get_version() {
    if [[ ! -f .version ]]; then
        echo "1" > .version
    fi
    vers=`cat .version`
    next_vers=$(( $vers + 1 ))
    echo $next_vers | tee .version
}


version=`get_version`
dest="pak"

echo "[I] AWS Upload Script"
echo "[I] Version: $version"

echo "[+] Building Server"
mkdir build
pushd build
cmake ..
make prez_game_server -j 4
popd

echo "[+] Copying Files"
mkdir arch
cp -rv build/server/prez_game_server $dest/server
cp -rv build/server/data $dest/
# cp -rv install.sh $dest/

# echo "[+] Uploading"
# aws gamelift upload-build \
#     --name cld-netrl \
#     --build-version $version \
#     --operating-system AMAZON_LINUX \
#     --region us-east-1 \
#     --build-root arch

echo "[+] Done!"
