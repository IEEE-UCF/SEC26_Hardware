#!/usr/bin/env bash

# Minimal dependency installer for Arduino libraries used by the workspace.
# Make sure this file uses Unix line endings (LF). CRLF will break the shell on Linux.

# Fail fast, treat unset variables as errors, and make pipelines fail on any command failure.
set -euo pipefail

# --- Configuration ---
# Get the absolute path of the directory containing this script.
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

# Define the path to the dependencies file.
DEPS_FILE="$SCRIPT_DIR/../src/teensy/library-dependencies.yml"

# --- Pre-flight checks ---
command -v yq >/dev/null 2>&1 || { echo "❌ 'yq' is required but not installed. Install it and retry." >&2; exit 1; }
command -v arduino-cli >/dev/null 2>&1 || { echo "❌ 'arduino-cli' is required but not installed. Install it and retry." >&2; exit 1; }

arduino-cli config set library.enable_unsafe_install true

# 1. Check if the dependency file actually exists.
if [ ! -f "$DEPS_FILE" ]; then
    echo "❌ Error: Dependency file not found at '$DEPS_FILE'"
    exit 1
fi

echo "🚀 Starting library installation from '$DEPS_FILE'..."
echo "--------------------------------------------------"

# 2. Use yq to parse the file and a 'while read' loop to process each library.
# Use a subshell pipeline so 'set -e' doesn't kill the whole script if one install fails; handle failures explicitly.
yq -r '.dependencies[] | .source // .name' "$DEPS_FILE" | while IFS= read -r library || [ -n "$library" ]; do
    # Skip any blank lines.
    if [ -z "$library" ]; then
        continue
    fi

    # Check if the library is a Git repository URL.
    if [[ "$library" == *.git ]]; then
        echo "Attempting to install from Git URL: '$library'"
        # Use the --git-url flag for git repositories.
        if arduino-cli lib install --git-url "$library"; then
            echo "✅ Successfully installed '$library'"
        else
            echo "❌ Failed to install '$library' from git" >&2
        fi
    else
        echo "Attempting to install from Library Manager: '$library'"
        # Use a normal install for library names.
        if arduino-cli lib install "$library"; then
            echo "✅ Successfully installed '$library'"
        else
            echo "❌ Failed to install '$library' from Library Manager" >&2
        fi
    fi

    echo "--------------------------------------------------"
done

echo "✨ Dependency installation complete!"