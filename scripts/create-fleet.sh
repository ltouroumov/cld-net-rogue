#!/bin/bash

fleet_name="cld-netrl"
build_id=""
instance_type="t2.micro"
runtime_config='ServerProcesses=[{LaunchPath="/local/game/server",Parameters="9000",ConcurrentExecutions=1}]'
port_config='FromPort=9000,ToPort=9100,IpRange="0.0.0.0/0",Protocol="TCP"'

create_fleet () {
    aws --profile heig gamelift create-fleet \
        --name $fleet_name \
        --build-id $build_id \
        --ec2-instance-type $instance_type \
        --runtime-configuration $runtime_config \
        --ec2-inbound-permissions $port_config \
        | jq -r ".FleetAttributes.FleetId"
}

while getopts "i:b:n:" opt; do
    case "$opt" in
        n)
            fleet_name=$OPTARG
            ;;
        b)
            build_id=$OPTARG
            ;;
        i)
            instance_type=$OPTARG
            ;;
        \?)
            echo "Unkown argument -$OPTARG"
            exit 1
            ;;
    esac
done


if [[ -z "$build_id" ]]; then
    echo "[!] Requires Build ID (-b)"
    exit 1
fi

echo "[+] Creating Fleet $fleet_name"
echo "[+] Build ID: $build_id"
echo "[+] Instance type: $instance_type"

fleet_id=`create_fleet`
echo "[+] Fleet Created: $fleet_id"
