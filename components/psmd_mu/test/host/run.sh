#!/bin/sh
set -eu

cd "$(dirname "$0")/../.."

out="${TMPDIR:-/tmp}/psmd_mu_sim_model_test"
cc -std=c11 -Wall -Wextra -Werror -Iinclude -I. \
    mu_sim_model.c test/host/mu_sim_model_test.c -o "$out"
"$out"
