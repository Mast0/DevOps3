#!/bin/bash

# Variables
IMAGE_NAME="mast00/http_server_image:latest"
CPU_BUSY_THRESHOLD_SRV1=45.0
CPU_BUSY_THRESHOLD_SRV2=27.0
CPU_IDLE_THRESHOLD=1.0
CHECK_INTERVAL=10  # Seconds between checks

trap "exit" INT TERM
trap "kill 0" EXIT

# Functions
get_cpu_load() {
    docker stats --no-stream --format "{{.Name}} {{.CPUPerc}}" | grep "$1" | awk '{print $2}' | sed 's/%//' || echo "0"
}

get_cpu_index() {
    case $1 in
        srv1) echo "1" ;;
        srv2) echo "2" ;;
        srv3) echo "3" ;;
        *) echo "1" ;;
    esac
}

start_container() {
    local container_name=$1
    local cpu_core=$2

    if docker ps --format "{{.Names}}" | grep -q "^$container_name$"; then
        echo "Container $container_name already exists. Removing it..."
        docker rm -f "$container_name"
    fi

    echo "Starting container $container_name on CPU core #$cpu_core..."
    docker run --name "$container_name" --cpuset-cpus="$cpu_core" --network bridge -d "$IMAGE_NAME";
}

update_containers() {
    pull_result=$(docker pull "$IMAGE_NAME" | grep "Downloaded newer image")
    if [ -n "$pull_result" ]; then
        echo "New image detected ------> updating containers..."
        containers=("srv1" "srv2" "srv3")
        for container in "${containers[@]}"; do
            if docker ps --format "{{.Names}}" | grep -q "^$container$"; then
                new_container="${container}_new"
                start_container "$new_container" "$(get_cpu_index "$container")"
                docker kill "$container"
                docker rm "$container"
                docker rename "$new_container" "$container"
                echo "$container has been updated."
            fi
        done
    else
        echo "No new image available."
    fi
}

manage_containers() {
    while true; do
        # Check idle containers and terminate
        for container in srv3 srv2; do
            if docker ps --format "{{.Names}}" | grep -q "$container"; then
                cpu=$(get_cpu_load "$container")
                if (( $(echo "$cpu < $CPU_IDLE_THRESHOLD" | bc -l) )); then
                    echo "$container is idle -----> terminating..."
                    docker kill "$container"
                    docker rm "$container"
                fi
            fi
        done

        # Check and manage srv1
        if docker ps --format "{{.Names}}" | grep -q "srv1"; then
            cpu_srv1=$(get_cpu_load "srv1")
            if (( $(echo "$cpu_srv1 > $CPU_BUSY_THRESHOLD_SRV1" | bc -l) )); then
                echo "srv1 is busy. Starting srv2..."
                if ! docker ps --format "{{.Names}}" | grep -q "srv2"; then
                    start_container "srv2" 2
                fi
            fi
        else
            start_container "srv1" 1
        fi

        # Check and manage srv2
        if docker ps --format "{{.Names}}" | grep -q "srv2"; then
            cpu_srv2=$(get_cpu_load "srv2")
            if (( $(echo "$cpu_srv2 > $CPU_BUSY_THRESHOLD_SRV2" | bc -l) )); then
                echo "srv2 is busy. Starting srv3..."
                if ! docker ps --format "{{.Names}}" | grep -q "srv3"; then
                    start_container "srv3" 3
                fi
            fi
        fi

        # Check for image updates
        update_containers

        sleep $CHECK_INTERVAL
    done
}

# Start container management
manage_containers

