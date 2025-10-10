#!/bin/bash

# --- Post-Container Initialization Script ---
# This script executes all other .sh files in this directory that start with
# a two-digit number (e.g., 01-*, 02-*) in numerical and then alphabetical order.

echo "--- Running Post-Startup Initialization Scripts ---"

# Get the directory where this script is located
SCRIPT_DIR=$(cd "$(dirname "$0")" && pwd)

# Find all files ending in .sh that begin with two digits (0-9)
# They are executed in the order returned by 'find', which is typically alphabetical,
# ensuring 01 runs before 02, etc.
find "${SCRIPT_DIR}" -maxdepth 1 -type f -name "[0-9][0-9]-*.sh" | sort | while read -r script; do
    
    # Check if the found item is the wrapper script itself (unlikely due to the name pattern, but safe)
    if [ "$(basename "${script}")" == "post-startup.sh" ]; then
        continue
    fi

    echo "▶️ Executing script: $(basename "${script}")"
    # Execute the script. If the script fails (non-zero exit code), the whole process should stop.
    if ! bash "${script}"; then
        echo "❌ ERROR: Script $(basename "${script}") failed. Exiting startup sequence."
        exit 1
    fi
    echo "✅ Finished script: $(basename "${script}")"
done

echo "--- Post-Startup Scripts Complete ---"

# Execute the main command passed to the script (CMD or arguments to ENTRYPOINT)
echo "🚀 Starting main container process: $@"
exec "$@"