#!/bin/bash

# You must have yq installed

# Exit immediately if a command exits with a non-zero status.
set -e

# --- Configuration ---
# Get the absolute path of the directory containing this script.
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

# Define the path to the dependencies file.
DEPS_FILE="$SCRIPT_DIR/../src/teensy/library-dependencies.yml"

# --- Script Logic ---
arduino-cli config set library.enable_unsafe_install true

# 1. Check if the dependency file actually exists.
if [ ! -f "$DEPS_FILE" ]; then
    echo "❌ Error: Dependency file not found at '$DEPS_FILE'"
    exit 1
fi

echo "🚀 Starting library installation from '$DEPS_FILE'..."
echo "--------------------------------------------------"

# 2. Use yq to parse the file and a 'while read' loop to process each library.
yq -r '.dependencies[] | .source // .name' "$DEPS_FILE" | while IFS= read -r library; do
    # Skip any blank lines.
    if [ -z "$library" ]; then
        continue
    fi

    # Check if the library is a Git repository URL.
    if [[ "$library" == *.git ]]; then
        echo "Attempting to install from Git URL: '$library'"
        # Use the --git-url flag for git repositories.
        arduino-cli lib install --git-url "$library"
    else
        echo "Attempting to install from Library Manager: '$library'"
        # Use a normal install for library names.
        arduino-cli lib install "$library"
    fi

    # Check the exit code to confirm success.
    if [ $? -eq 0 ]; then
        echo "✅ Successfully installed '$library'"
    else
        # Because of 'set -e', the script will exit here on failure.
        echo "❌ Failed to install '$library'"
    fi
    echo "--------------------------------------------------"
done

echo "✨ Dependency installation complete!"