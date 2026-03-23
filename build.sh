#!/bin/bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "${SCRIPT_DIR}"

SDK="${1:-}"
TARGET="${2:---all}"
if [[ -z "${SDK}" ]]; then
  echo "用法: bash build.sh <maca|hpcc> [--all|<operator>]"
  exit 1
fi

case "${SDK}" in
  maca)
    COMPILER="mxcc"
    SDK_DEFINE="-DSDK_MACA"
    LIB_DIR="/opt/maca-3.2.1/lib/"
    COLL_LIB="-lmccl"
    OUT_SUFFIX="maca"
    COMMON_SRC="common.maca"
    VERIFY_SRC="verifiable.maca"
    ;;
  hpcc)
    COMPILER="htcc"
    SDK_DEFINE="-DSDK_HPCC"
    LIB_DIR="/opt/hpcc/lib/"
    COLL_LIB="-lhccl"
    OUT_SUFFIX="hpcc"
    TMP_DIR="$(mktemp -d)"
    trap 'rm -rf "${TMP_DIR}"' EXIT
    cp common.maca "${TMP_DIR}/common.hpcc"
    cp verifiable.maca "${TMP_DIR}/verifiable.hpcc"
    COMMON_SRC="${TMP_DIR}/common.hpcc"
    VERIFY_SRC="${TMP_DIR}/verifiable.hpcc"
    ;;
  *)
    echo "不支持的SDK类型: ${SDK}"
    echo "用法: bash build.sh <maca|hpcc> [--all|<operator>]"
    exit 1
    ;;
esac

compile_one() {
  local op_name="$1"
  local op_src
  local output_file

  op_name="${op_name%.maca}"

  if [[ "${SDK}" == "maca" ]]; then
    op_src="${op_name}.maca"
  else
    cp "${op_name}.maca" "${TMP_DIR}/${op_name}.hpcc"
    op_src="${TMP_DIR}/${op_name}.hpcc"
  fi

  if [[ ! -f "${op_name}.maca" ]]; then
    echo "文件不存在: ${op_name}.maca"
    exit 1
  fi

  output_file="${op_name}_perf_${OUT_SUFFIX}"
  "${COMPILER}" ${SDK_DEFINE} "${COMMON_SRC}" "${VERIFY_SRC}" "${op_src}" \
    -o "${output_file}" \
    -L"${LIB_DIR}" ${COLL_LIB}
  echo "已生成: ${output_file}"
}

if [[ "${TARGET}" == "--all" ]]; then
  for source_path in *.maca; do
    source_name="$(basename "${source_path}" .maca)"
    if [[ "${source_name}" == "common" || "${source_name}" == "verifiable" ]]; then
      continue
    fi
    compile_one "${source_name}"
  done
else
  compile_one "${TARGET}"
fi
