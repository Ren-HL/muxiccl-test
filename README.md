# muxiccl-test

沐曦集合通信测试程序，支持两套 SDK：

- `maca + mccl`
- `hpcc + hccl`

项目通过统一适配层 [`sdk_adapter.h`](/Users/hl/Documents/qiyuan/code/muxi/muxiccl-test/sdk_adapter.h) 屏蔽两套 SDK 的头文件、类型和集合通信 API 差异。业务侧统一使用：

- runtime 抽象：`rt*`
- collective 抽象：`ccl*`

## 目录结构

- [`common.maca`](/Users/hl/Documents/qiyuan/code/muxi/muxiccl-test/common.maca)：测试主框架、参数解析、线程与性能统计
- [`verifiable.maca`](/Users/hl/Documents/qiyuan/code/muxi/muxiccl-test/verifiable.maca)：设备端数据初始化与结果校验
- [`sdk_adapter.h`](/Users/hl/Documents/qiyuan/code/muxi/muxiccl-test/sdk_adapter.h)：`maca/mccl` 与 `hpcc/hccl` 统一适配层
- 各算子文件：
  - [`allreduce.maca`](/Users/hl/Documents/qiyuan/code/muxi/muxiccl-test/allreduce.maca)
  - [`allgather.maca`](/Users/hl/Documents/qiyuan/code/muxi/muxiccl-test/allgather.maca)
  - [`broadcast.maca`](/Users/hl/Documents/qiyuan/code/muxi/muxiccl-test/broadcast.maca)
  - [`reduce.maca`](/Users/hl/Documents/qiyuan/code/muxi/muxiccl-test/reduce.maca)
  - [`reducescatter.maca`](/Users/hl/Documents/qiyuan/code/muxi/muxiccl-test/reducescatter.maca)
  - [`sendrecv.maca`](/Users/hl/Documents/qiyuan/code/muxi/muxiccl-test/sendrecv.maca)
  - [`alltoall.maca`](/Users/hl/Documents/qiyuan/code/muxi/muxiccl-test/alltoall.maca)
  - [`alltoallv.maca`](/Users/hl/Documents/qiyuan/code/muxi/muxiccl-test/alltoallv.maca)
  - [`alltoalld.maca`](/Users/hl/Documents/qiyuan/code/muxi/muxiccl-test/alltoalld.maca)
- [`build.sh`](/Users/hl/Documents/qiyuan/code/muxi/muxiccl-test/build.sh)：双 SDK 编译脚本
- [`run.sh`](/Users/hl/Documents/qiyuan/code/muxi/muxiccl-test/run.sh)：运行脚本

## 支持的算子

- `allreduce`
- `allgather`
- `broadcast`
- `reduce`
- `reducescatter`
- `sendrecv`
- `alltoall`
- `alltoallv`
- `alltoalld`

## 编译

进入项目目录：

```bash
cd /Users/hl/Documents/qiyuan/code/muxi/muxiccl-test
```

### 编译全部算子

MACA:

```bash
bash build.sh maca --all
```

HPCC:

```bash
bash build.sh hpcc --all
```

### 编译单个算子

MACA:

```bash
bash build.sh maca allreduce
```

HPCC:

```bash
bash build.sh hpcc allreduce
```

也支持传完整文件名：

```bash
bash build.sh maca allreduce.maca
bash build.sh hpcc allreduce.maca
```

### 编译输出

编译产物命名规则：

- MACA: `<op>_perf_maca`
- HPCC: `<op>_perf_hpcc`

例如：

- `allreduce_perf_maca`
- `allreduce_perf_hpcc`

## 运行

### 直接运行二进制

示例：

```bash
./allreduce_perf_maca
./allreduce_perf_hpcc
```

带参数运行：

```bash
./allreduce_perf_maca -b 10485760 -e 1073741824 -f 2 -n 20 -w 5 -c 1 --check-samples 32 --all-dtypes
```

### 使用 run.sh

运行：

```bash
bash run.sh
```

然后输入目标程序名，例如：

```text
allreduce_perf_maca
```

再输入运行参数，例如：

```text
-b 10485760 -e 1073741824 -f 2 -n 20 -w 5 -c 1 --check-samples 32 --all-dtypes
```

说明：

- `run.sh` 的单程序模式可以运行带 `_maca` / `_hpcc` 后缀的二进制
- `run.sh --all` 当前会扫描 `*_perf`，不适用于带 `_maca` / `_hpcc` 后缀的输出；批量跑双 SDK 二进制时建议自行写循环或逐个执行

## 常用参数

- `-b, --minbytes`：最小消息大小，单位字节
- `-e, --maxbytes`：最大消息大小，单位字节
- `-f, --stepfactor`：步进倍数
- `-i, --stepbytes`：按固定字节步进
- `-n, --iters`：正式迭代次数
- `-w, --warmup_iters`：预热次数
- `-m, --agg_iters`：单次计时聚合次数
- `-c, --check`：是否校验结果，`0/1`
- `--check-samples`：抽样校验点数
- `--inplace`：仅测 in-place
- `--noinplace`：仅测 out-of-place
- `-r, --root`：root 类算子的根 rank
- `-d, --datatype`：指定数据类型
- `--all-dtypes`：测试所有数据类型

## 示例

### 从 10 MiB 到 1 GiB 做完整测试

```bash
./allreduce_perf_maca -b 10485760 -e 1073741824 -f 2 -n 20 -w 5 -m 1 -c 1 --check-samples 32 --all-dtypes
```

### 只使用 3 张卡

先限制可见设备，再设置线程数：

```bash
export MACA_VISIBLE_DEVICES=0,1,2
export NTHREADS=3
./allreduce_perf_maca
```

如果是在 HPCC 环境下，按实际环境变量名调整可见设备设置。

## 适配说明

- 集合通信接口统一使用 `ccl*` 命名，例如 `cclAllReduce`
- runtime 接口统一使用 `rt*` 命名，例如 `rtStreamSynchronize`
- `build.sh hpcc` 会将 `.maca` 源文件临时复制成 `.hpcc` 后缀后再调用 `htcc` 编译，这是为了兼容 `htcc` 不识别 `.maca` 后缀的问题

## 注意事项

- 当前 `hpcc` 分支是按 `maca` 风格平移得到，若某些 API、类型名、头文件名在实际 HPCC 环境中不一致，需要继续按报错微调
- `alltoallv` 和 `alltoalld` 当前测试数据布局使用的是等量、连续分段模式，目的是先保证框架接入和正确性验证通路稳定
