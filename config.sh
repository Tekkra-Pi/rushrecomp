# Project configuration — sourced by shell scripts.
#
# Override any variable by setting it before sourcing:
#   export SONIC_RUSH_ROM="/path/to/rom.nds"
#   source config.sh

# Project root (auto-detected from this file's location)
PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# ROM path — set SONIC_RUSH_ROM env var to override
: "${SONIC_RUSH_ROM:=${PROJECT_ROOT}/games/sonic-rush.nds}"

# Tools
: "${ARMIPS:=${PROJECT_ROOT}/tools/armips}"
: "${ARM_NONE_GCC:=${PROJECT_ROOT}/tools/arm-gcc/bin/arm-none-eabi-gcc}"
: "${R2:=r2}"

# Directories
DISASSEMBLY_DIR="${PROJECT_ROOT}/disassembly"
RUNTIME_DIR="${PROJECT_ROOT}/runtime"
SCRIPTS_DIR="${PROJECT_ROOT}/scripts"
