#!/bin/bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "${SCRIPT_DIR}"

compile_one() {
  local source_file="$1"
  if [[ ! -f "${source_file}" ]]; then
    echo "文件不存在: ${source_file}"
    exit 1
  fi

  local binary_name
  binary_name="$(basename "${source_file}")"
  binary_name="${binary_name%.*}_perf"
  if [[ -z "${binary_name}" ]]; then
    echo "无法解析输出程序名。"
    exit 1
  fi

  mxcc common.maca verifiable.maca "${source_file}" \
    -o "${binary_name}" \
    -L/opt/maca-3.2.1/lib/ -lmccl

  echo "已生成: ${binary_name}"
}

if [[ "${1:-}" == "--all" ]]; then
  for source_file in *.maca; do
    if [[ "${source_file}" == "common.maca" || "${source_file}" == "verifiable.maca" ]]; then
      continue
    fi
    compile_one "${source_file}"
  done
  exit 0
fi

read -r -p "请输入要编译的.maca文件名 (例如: allreduce.maca/allgather.maca/alltoall.maca/alltoallv.maca/alltoalld.maca): " source_file
if [[ -z "${source_file}" ]]; then
  echo "未提供文件名，已退出。"
  exit 1
fi

compile_one "${source_file}"
