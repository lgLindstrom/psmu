#!/usr/bin/env bash
set -euo pipefail

# Launch ESP-IDF MCP server (Maty403/esp-idf-mcp).
# Override completely with ESP_IDF_MCP_CMD, or override paths via IDF_PATH/IDF_TOOLS_PATH/IDF_PYTHON_ENV_PATH.
if [ -n "${ESP_IDF_MCP_CMD:-}" ]; then
  exec bash -lc "${ESP_IDF_MCP_CMD}"
fi

# Auto-detect ESP-IDF (prefer newest ~/esp/esp-idf-v*)
if [ -z "${IDF_PATH:-}" ] && [ -d "$HOME/esp" ]; then
  latest_idf="$(ls -1d "$HOME"/esp/esp-idf-v* 2>/dev/null | sort -V | tail -n 1 || true)"
  if [ -n "$latest_idf" ]; then
    export IDF_PATH="$latest_idf"
  fi
fi

# Auto-detect Espressif tools dir
if [ -z "${IDF_TOOLS_PATH:-}" ] && [ -d "$HOME/.espressif" ]; then
  export IDF_TOOLS_PATH="$HOME/.espressif"
fi

# Auto-detect matching python env for selected IDF version
if [ -z "${IDF_PYTHON_ENV_PATH:-}" ] && [ -n "${IDF_PATH:-}" ] && [ -d "$HOME/.espressif/python_env" ]; then
  ver="$(basename "$IDF_PATH" | sed -n 's/^esp-idf-v\([0-9][0-9.]*\).*/\1/p')"
  if [ -n "$ver" ]; then
    pyenv_match="$(ls -1d "$HOME/.espressif/python_env"/idf${ver}_* 2>/dev/null | sort -V | tail -n 1 || true)"
    if [ -n "$pyenv_match" ]; then
      export IDF_PYTHON_ENV_PATH="$pyenv_match"
    fi
  fi
fi

if command -v uvx >/dev/null 2>&1; then
  # esp-idf-mcp 1.1.x requires mcp<2 (FastMCP v1 API) and Python>=3.10
  exec uvx --python 3.12 --with 'mcp<2' esp-idf-mcp "$@"
elif command -v esp-idf-mcp >/dev/null 2>&1; then
  exec esp-idf-mcp "$@"
else
  echo "esp-idf-mcp not found: install uv (for uvx) or 'pip install \"esp-idf-mcp\" \"mcp<2\"'" >&2
  exit 127
fi
