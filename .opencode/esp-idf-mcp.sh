#!/usr/bin/env bash
set -euo pipefail

# Launch ESP-IDF MCP server (Maty403/esp-idf-mcp).
# Override completely with ESP_IDF_MCP_CMD, or override paths via IDF_PATH/IDF_TOOLS_PATH/IDF_PYTHON_ENV_PATH.
if [ -n "${ESP_IDF_MCP_CMD:-}" ]; then
  exec bash -lc "${ESP_IDF_MCP_CMD}"
fi

# --- 1. EIM installations -----------------------------------------------------
# `eim` writes ~/.espressif/tools/eim_idf.json with every install and the id that
# `eim select` marked. Each install also has a generated activation script; its
# `-e` mode prints KEY=VALUE and nothing else, which is what we need here:
# `eim shell` is interactive and both it and `eim run` print a banner to stdout,
# and a banner on stdout would corrupt the MCP JSON-RPC stream.
eim_json="${EIM_IDF_JSON:-$HOME/.espressif/tools/eim_idf.json}"
eim_activation=""
if [ -f "$eim_json" ] && command -v python3 >/dev/null 2>&1; then
  read -r eim_activation _ignored < <(
    python3 - "$eim_json" "${EIM_IDF_VERSION:-}" 2>/dev/null <<'PY' || true
import json, sys

path, want = sys.argv[1], sys.argv[2]
with open(path, encoding="utf-8") as fh:
    data = json.load(fh)
items = [i for i in data.get("idfInstalled", []) if i.get("status") == "finished"]


def version(item):
    parts = item.get("name", "").lstrip("v").split("-")[0].split(".")
    return tuple(int(p) for p in parts if p.isdigit())


if want:
    items = [i for i in items if i.get("name") in (want, "v" + want)]
else:
    selected = [i for i in items if i.get("id") == data.get("idfSelectedId")]
    items = selected or sorted(items, key=version)

if items:
    print(items[-1].get("activationScript", "-"), items[-1].get("path", "-"))
PY
  ) || true
fi

if [ -n "$eim_activation" ] && [ "$eim_activation" != "-" ] && [ -f "$eim_activation" ]; then
  _tools_path=""
  while IFS='=' read -r key value; do
    case "$key" in
      '' | SYSTEM_PATH) continue ;;
      PATH) _tools_path="$value" ;;
      *[!A-Za-z0-9_]*) continue ;;
      *) export "$key=$value" ;;
    esac
  done < <(bash "$eim_activation" -e 2>/dev/null || true)
  # The activation script prepends to PATH instead of replacing it, so do the same.
  if [ -n "$_tools_path" ]; then
    export PATH="$_tools_path:$PATH"
  fi
fi

# --- 2. Git-clone installations ----------------------------------------------
# Used only when EIM did not provide IDF_PATH: ~/esp/esp-idf-v<ver>, highest wins.
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
    _v="$(basename "$_p" | sed -n 's/^esp-idf-v\{0,1\}\([0-9][0-9.]*\)$/\1/p')"
  fi
  printf '%s\n' "$_v"
}

if [ -z "${IDF_PATH:-}" ]; then
  latest_idf="$(
    for candidate in "$HOME"/esp/esp-idf-v*; do
      [ -d "$candidate" ] || continue
      version="$(idf_version_of "$candidate")"
      [ -n "$version" ] || continue
      printf '%s %s\n' "$version" "$candidate"
    done | sort -V -k1,1 | tail -n 1 | cut -d' ' -f2- || true
  )"
  if [ -n "$latest_idf" ]; then
    export IDF_PATH="$latest_idf"
  fi
fi

if [ -z "${IDF_TOOLS_PATH:-}" ] && [ -d "$HOME/.espressif" ]; then
  export IDF_TOOLS_PATH="$HOME/.espressif"
fi

# Python env for a git-clone install: classic installer layout.
if [ -z "${IDF_PYTHON_ENV_PATH:-}" ] && [ -n "${IDF_PATH:-}" ]; then
  idf_ver="$(idf_version_of "$IDF_PATH")"
  if [ -n "$idf_ver" ]; then
    for candidate in "$HOME/.espressif/python_env/idf${idf_ver}_"*_env; do
      if [ -d "$candidate" ]; then
        export IDF_PYTHON_ENV_PATH="$candidate"
        export ESP_IDF_VERSION="${ESP_IDF_VERSION:-$idf_ver}"
        break
      fi
    done
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
