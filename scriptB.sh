#!/bin/bash

# Variables
SERVER_URL="http://127.0.0.1/compute"   # Replace with the actual server URL or IP
REQUEST_INTERVAL_MIN=5          # Minimum time between requests (seconds)
REQUEST_INTERVAL_MAX=6         # Maximum time between requests (seconds)

# Function to send HTTP requests asynchronously
send_requests() {
    while true; do
        # Generate a random delay between REQUEST_INTERVAL_MIN and REQUEST_INTERVAL_MAX
        delay=$((RANDOM % (REQUEST_INTERVAL_MAX - REQUEST_INTERVAL_MIN + 1) + REQUEST_INTERVAL_MIN))

        # Log the action
        echo "$(date '+%Y-%m-%d %H:%M:%S'): Sending HTTP request to $SERVER_URL after $delay seconds..."

        # Perform an HTTP GET request
        curl -s -o /dev/null -w "Request status: %{http_code}\n" "$SERVER_URL" &

        # Wait for the random interval before sending the next request
        sleep "$delay"
    done
}

# Start sending requests
echo "Starting asynchronous HTTP requests to $SERVER_URL..."
send_requests

