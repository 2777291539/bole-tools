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

### Config

- 配置文件路径: gbf/Default/Config
- View 文件路径: gbf/Default/Path
    - Path 文件中可添加多个路径，但只会使用第一个路径

### 注释

- Config: 注释需要在行为树函数上面任意位置单独起一行
- View: 注释需要加在注册的行为树函数后面

### Usage

|Usage|Action|
|-|-|
|`gbf`|通过配置文件实现行为树函数|
|`gbf [-p] <file>`|实现文件中注册的行为树函数|
|`gbf -l`|输出 log 信息(配合 `-p` 选项)|
|`gbf -h`|输出 help 信息|
|`gbf -i`|交互式|

### Version

- 1.0.0: 生成行为树函数
- 1.1.0: 适配多行格式
- 2.0.0: 配置文件注册+生成行为树函数

### TODO

- 自动注册行为树函数 ✓
- 自动 Drop Idle
- ...
