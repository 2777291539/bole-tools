# bole-tools

便于日常开发的各个工具

## gbf

Generate Behaviortree Functions

### Quickstart

1. 克隆

```
git clone --depth 1 https://github.com/2777291539/bole-tools.git
```

2. 进入 gbf 目录

```
cd bole-tools/gbf
```

3. 构建

```
sudo ./build.sh
```

- 注意: 第一次执行可能需要 **超级用户权限**

4. 执行

```
gbf -v
```

- 出现版本号，即构建成功

### Update

1. 拉取

```
git pull --depth=1
```

2. 重新构建

```
./build.sh
```

### Usage

|Usage|Action|
|-|-|
|`gbf [-p] <file>`|实现文件中注册的行为数函数|
|`gbf -l`|输出 log 信息|
|`gbf -h`|输出 help 信息|
|...||

### Version

- 1.0.0: 生成行为树函数
- 1.1.0: 适配多行格式

### TODO

- 自动注册行为树函数
- 自动 Drop Idle
- ...
