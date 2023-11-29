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
|`gbf [-p \| --path] <file>`|实现文件中注册的行为数函数|
|...||

### TODO

- 自动注册行为树函数
- 适配不同格式注册的行为树函数
- ...
