#!/bin/bash

# This script ensures the VS Code configuration file (c_cpp_properties.json) exists.
# It copies a template file if one is not found.

echo "--- Checking for VS Code C/C++ Configuration ---"

# Get the directory where this script is located
SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)

# Define file paths
VSCODE_DIR="${SCRIPT_DIR}/../../../.vscode"
CONFIG_FILE="${VSCODE_DIR}/c_cpp_properties.json"
TEMPLATE_FILE="${SCRIPT_DIR}/c_cpp_properties.json.template"

# --- Check if the configuration file already exists ---
if [ -f "${CONFIG_FILE}" ]; then
    echo "VS Code C/C++ configuration file (${CONFIG_FILE}) already exists. Skipping creation."
    exit 0
fi

# --- Check if the template file exists ---
if [ ! -f "${TEMPLATE_FILE}" ]; then
    echo "❌ ERROR: Template file not found at ${TEMPLATE_FILE}."
    echo "Please create 'c_cpp_properties.json.template' in the scripts directory."
    exit 1
fi

# --- Create the .vscode directory if it doesn't exist ---
if [ ! -d "${VSCODE_DIR}" ]; then
    echo "Creating directory: ${VSCODE_DIR}"
    mkdir -p "${VSCODE_DIR}"
fi

# --- Copy the template to the destination ---
echo "Copying template file to destination: ${CONFIG_FILE}"
cp "${TEMPLATE_FILE}" "${CONFIG_FILE}"

echo "Configuration file created from template. You may need to verify its contents."