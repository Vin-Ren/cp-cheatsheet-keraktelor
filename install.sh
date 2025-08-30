#!/usr/bin/env bash
set -euo pipefail
INIT_DIR="$(pwd)"
FONT_DIR="$HOME/.local/share/fonts/source-code-pro"

echo  "Installing dependencies..."

if fc-list | grep "Source Code Pro" > /dev/null 2>&1
then
    echo "✅ Source Code Pro already installed"
else
    echo "Font Source Code Pro not found, installing..."
    TMP_DIR="$(mktemp -d)"
    cd "$TMP_DIR"

    # Download release branch zip
    echo "⬇️ Downloading Source Code Pro..."
    curl -L -o scp.zip https://github.com/adobe-fonts/source-code-pro/archive/refs/heads/release.zip

    # Extract TTFs
    echo "📦 Extracting fonts..."
    unzip -q scp.zip "source-code-pro-release/TTF/*"

    # Install into user font directory
    mkdir -p "$FONT_DIR"
    mv source-code-pro-release/TTF/*.ttf "$FONT_DIR/"

    # Refresh font cache
    echo "🔄 Refreshing font cache..."
    fc-cache -f "$FONT_DIR"

    # Cleanup
    cd "$INIT_DIR"
    rm -rf "$TMP_DIR"
    echo "✅ Font Source Code Pro installed successfully"
fi

if xelatex --version > /dev/null 2>&1
then
    echo "✅ xelatex already installed"
else
    echo "xelatex not found, installing..."
    sudo apt-get install -y texlive-xetex
    if [ $? -ne 0 ]
        then 
        echo "Failed to install xelatex"
        exit 1
    fi
    echo "✅ xelatex installed successfully"
fi

echo "Extras: adding executable permission to scripts..."
chmod +x ./format.sh
chmod +x ./generate.sh
echo "✅ Added +x to scripts"

echo "Finished installing dependencies, you can now run ./generate.sh"
