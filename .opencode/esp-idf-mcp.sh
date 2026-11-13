#!/usr/bin/env bash
set -euo pipefail

# Launch ESP-IDF MCP server (Maty403/esp-idf-mcp).
# Override completely with ESP_IDF_MCP_CMD, or override paths via IDF_PATH/IDF_TOOLS_PATH/IDF_PYTHON_ENV_PATH.
if [ -n "${ESP_IDF_MCP_CMD:-}" ]; then
  exec bash -lc "${ESP_IDF_MCP_CMD}"
fi

# Print the version of one ESP-IDF installation: version.cmake first, then
# version.txt, then the directory name.
idf_version_of() {
  _p="$1"
  _v=""
  if [ -f "$_p/tools/cmake/version.cmake" ]; then
    _major="$(sed -n 's/^set(IDF_VERSION_MAJOR[[:space:]]\{1,\}\([0-9]\{1,\}\)).*/\1/p' "$_p/tools/cmake/version.cmake" | sed -n 1p)"
    _minor="$(sed -n 's/^set(IDF_VERSION_MINOR[[:space:]]\{1,\}\([0-9]\{1,\}\)).*/\1/p' "$_p/tools/cmake/version.cmake" | sed -n 1p)"
    if [ -n "$_major" ] && [ -n "$_minor" ]; then
      _v="$_major.$_minor"
    fi
  fi
  if [ -z "$_v" ] && [ -f "$_p/version.txt" ]; then
    _v="$(sed -n '1s/^v\{0,1\}\([0-9][0-9.]*\).*/\1/p' "$_p/version.txt")"
  fi
  if [ -z "$_v" ]; then
    _v="$(basename "$(dirname "$_p")" | sed -n 's/^v\{0,1\}\([0-9][0-9.]*\)$/\1/p')"
  fi
  if [ -z "$_v" ]; then
    _v="$(basename "$_p" | sed -n 's/^esp-idf-v\{0,1\}\([0-9][0-9.]*\)$/\1/p')"
  fi
  printf '%s\n' "$_v"
}

# Print "version path" for every installation found.
idf_candidates() {
  { ls -1d "$HOME"/esp/esp-idf-v* 2>/dev/null || true
    ls -1d "$HOME"/.espressif/v*/esp-idf 2>/dev/null || true
  } | while read -r p; do
    v="$(idf_version_of "$p")"
    [ -n "$v" ] || continue
    printf '%s %s\n' "$v" "$p"
  done
}

idf_ver=""
if [ -z "${IDF_PATH:-}" ]; then
  latest_idf="$(idf_candidates | sort -V -k1,1 | tail -n 1)"
  if [ -n "$latest_idf" ]; then
    export IDF_PATH="${latest_idf#* }"
  fi
fi

# Auto-detect Espressif tools dir
if [ -z "${IDF_TOOLS_PATH:-}" ] && [ -d "$HOME/.espressif" ]; then
  export IDF_TOOLS_PATH="$HOME/.espressif"
fi

# Version of the selected installation, used for the venv, the shims and the
# ESP_IDF_VERSION variable that idf_component_manager expects.
if [ -n "${IDF_PATH:-}" ]; then
  idf_ver="$(idf_version_of "$IDF_PATH")"
fi

# Auto-detect matching python env for the selected IDF version. EIM keeps its
# venv under $IDF_TOOLS_PATH/python/v<ver>/venv, the classic installer under
# $HOME/.espressif/python_env/idf<ver>_*_env.
if [ -z "${IDF_PYTHON_ENV_PATH:-}" ] && [ -n "$idf_ver" ]; then
  for cand in \
    "$IDF_TOOLS_PATH/python/v$idf_ver/venv" \
    "$HOME/.espressif/tools/python/v$idf_ver/venv" \
    "$HOME/.espressif/python_env/idf${idf_ver}_"*_env; do
    if [ -d "$cand" ]; then
      export IDF_PYTHON_ENV_PATH="$cand"
      break
    fi
  done
fi

if [ -n "$idf_ver" ]; then
  export ESP_IDF_VERSION="${ESP_IDF_VERSION:-$idf_ver}"
  export IDF_VERSION="${IDF_VERSION:-$idf_ver}"
fi

# idf.py needs the venv and the version's shims on PATH. export.sh normally does
# this, but it also prints to stdout, which would corrupt the MCP stdio protocol.
if [ -n "${IDF_PYTHON_ENV_PATH:-}" ]; then
  export PATH="$IDF_PYTHON_ENV_PATH/bin:$PATH"
fi
if [ -n "$idf_ver" ] && [ -d "$IDF_TOOLS_PATH/shims/v$idf_ver" ]; then
  export PATH="$IDF_TOOLS_PATH/shims/v$idf_ver:$PATH"
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
