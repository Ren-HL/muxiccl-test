#!/bin/bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "${SCRIPT_DIR}"

LOG_FILE=""

if [[ "${1:-}" == "--all" ]]; then
  shift
  if [[ $# -gt 0 ]]; then
    run_args="$*"
  else
    read -r -p "请输入运行参数(可留空): " run_args
  fi
  log_file="allop.log"
  : >"${log_file}"
  for target in *_perf; do
    if [[ ! -f "${target}" ]]; then
      continue
    fi
    echo "运行: ${target}" | tee -a "${log_file}"
    if [[ -z "${run_args}" ]]; then
      "./${target}" >>"${log_file}" 2>&1
    else
      "./${target}" ${run_args} >>"${log_file}" 2>&1
    fi
  done
  exit 0
fi

if [[ "${1:-}" == "-l" || "${1:-}" == "--log" ]]; then
  if [[ -z "${2:-}" ]]; then
    echo "未提供日志文件名"
    exit 1
  fi
  LOG_FILE="$2"
  shift 2
fi

read -r -p "请输入要运行的程序文件名 (例如: allreduce_perf 或 allgather_perf): " target
if [[ -z "${target}" ]]; then
  echo "未提供文件名，已退出。"
  exit 1
fi

if [[ ! -f "${target}" ]]; then
  echo "文件不存在: ${target}"
  exit 1
fi

read -r -p "请输入运行参数(可留空): " run_args

if [[ -z "${LOG_FILE}" ]]; then
  LOG_FILE="${target%_perf}.log"
fi

echo "运行: ${target} (log: ${LOG_FILE})"
if [[ -z "${run_args}" ]]; then
  "./${target}" >"${LOG_FILE}" 2>&1
else
  "./${target}" ${run_args} >"${LOG_FILE}" 2>&1
fi
