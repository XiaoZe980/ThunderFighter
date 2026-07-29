# ⚡ ThunderFighter 雷霆战机

基于 **Unreal Engine 5.8** 开发的竖版飞行射击游戏（STG），致敬经典雷霆战机。

---

## 🎮 游戏简介

玩家操控战机在屏幕底部移动，自动射击上方敌机。击败敌人获取道具升级武器，挑战关卡 Boss，冲击最高分数。

## 🛠 技术栈

| 模块 | 技术 |
|------|------|
| 引擎 | Unreal Engine 5.8 |
| 语言 | C++20 |
| IDE | JetBrains Rider |
| 构建工具链 | Visual Studio 2022 (MSVC) |
| 输入 | Enhanced Input System |
| UI | UMG (Unreal Motion Graphics) |

## 📁 项目结构

```
ThunderFighter/
├── Source/ThunderFighter/        # C++ 源码
│   ├── Core/                     # 游戏框架
│   │   ├── ThunderFighterGameMode       计分/波次/GameOver
│   │   ├── ThunderFighterPlayerController  EnhancedInput 输入
│   │   └── ThunderFighterGameInstance    全局状态管理
│   ├── Actors/                   # 游戏实体
│   │   ├── ThunderFighterPlayerPawn      玩家战机
│   │   ├── EnemyBase                     敌机基类
│   │   ├── EnemySpawner                  波次生成器
│   │   ├── ProjectileBase                弹幕基类
│   │   ├── PickupBase                    道具基类
│   │   └── ScrollingBackground           循环滚动背景
│   ├── Components/               # 可复用组件
│   │   ├── ThunderFighterHealthComponent 生命值管理
│   │   ├── ThunderFighterWeaponComponent 武器系统 (5级弹幕)
│   │   ├── ProjectilePatternComponent    弹幕发射器
│   │   └── PickupSpawnComponent          道具掉落
│   └── UI/
│       └── ThunderFighterHUD             HUD 管理
├── Content/                      # 蓝图资产 & 美术资源
├── Config/                       # 项目配置
└── Docs/                         # 文档
```

## 🚀 快速开始

### 环境要求
- **Unreal Engine 5.8**（Epic Games Launcher 安装）
- **Visual Studio 2022**（编译器工具链，勾选"使用 C++ 的游戏开发"） — UE 在 Windows 上需要 MSVC 编译器才能构建 C++ 项目
- **推荐 IDE：JetBrains Rider**（也可用 VS2022 自带的 IDE）
- **Windows 10/11**

> 💡 **VS2022 ≠ IDE：** 即使使用 Rider 编写代码，仍然需要安装 VS2022 作为底层构建工具链。Rider 会自动检测并使用 VS2022 的 MSVC 编译器和 Windows SDK。如果还没有安装 Rider，需要额外安装 Rider 对 UE 的支持插件。

### 打开项目
1. 克隆仓库
   ```bash
   git clone https://github.com/XiaoZe980/ThunderFighter.git
   ```
2. 右键 `ThunderFighter.uproject` → **Generate Visual Studio project files**（生成 Rider 也会用到的项目文件）
3. 双击 `ThunderFighter.uproject` 打开 UE 编辑器

### 编译
- **在 UE 编辑器中**：工具栏点击 `Compile` 或按 `Ctrl + Alt + F11`（Rider 中修改代码后，切换回编辑器点 Compile）
- **在 Rider 中**：Rider 自带 UE 编译支持，可直接 `Build → Build Solution`
- 或在 VS 中：打开 `.sln` 文件，按 `F5`

## 📊 开发进度

| 系统 | 状态 |
|------|------|
| 游戏框架 (GameMode/PlayerController/GameInstance) | ✅ C++ 完成 |
| 玩家战机 (移动/射击/生命值) | ✅ C++ 完成 |
| 弹幕系统 (阵营/碰撞/弹幕模式) | ✅ C++ 完成 |
| 敌机系统 (基类/波次生成/曲线移动) | ✅ C++ 完成 |
| UI 系统 (HUD/暂停/结算) | ✅ C++ 完成 |
| 道具系统 (武器升级/回血/炸弹/分数) | ✅ C++ 完成 |
| 滚动背景 | ✅ C++ 完成 |
| 输入配置 (Input Actions/IMC) | ✅ 蓝图完成 |
| 玩家蓝图 | ⬜ 待创建 |
| 敌机蓝图 | ⬜ 待创建 |
| 弹幕蓝图 | ⬜ 待创建 |
| UMG Widget 蓝图 | ⬜ 待创建 |
| 测试关卡 | ⬜ 待搭建 |
| 美术资源 | ⬜ 待制作 |

## 🎯 游戏特性

- **5 级武器系统** — 单发 → 双发 → 三发 → 四发 → 扇形弹幕
- **波次生成** — 可配置的敌机波次（FSpawnEntry / FEnemyWave）
- **曲线移动** — 使用 UCurveFloat 定义敌机飞行路径
- **多种弹幕模式** — 扇形、圆形、追踪玩家
- **4 种道具** — 武器升级(W)、生命恢复(H)、炸弹(B)、分数加成(S)
- **炸弹清屏** — 一键清除所有敌方弹幕

## 📝 命名规范

详见 [Docs/AssetPaths.md](Docs/AssetPaths.md)

## 📄 License

MIT
