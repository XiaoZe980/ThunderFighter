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
│   ├── Blueprints/
│   │   ├── Core/                 # BP_GameMode / BP_PlayerController / BP_HUD
│   │   ├── Player/               # BP_PlayerPawn
│   │   ├── Enemies/              # BP_Enemy_Grunt / BP_Enemy_Boss ...
│   │   ├── Projectiles/          # BP_Projectile_Player / BP_Projectile_Enemy
│   │   ├── Pickups/              # BP_Pickup_*
│   │   └── Background/           # BP_ScrollingBackground
│   ├── Input/
│   │   ├── Actions/              # IA_MoveHorizontal / IA_Fire ...
│   │   └── Mappings/             # IMC_Gameplay
│   ├── UI/
│   │   └── Widgets/              # WBP_GameplayHUD / WBP_GameOver ...
│   ├── Maps/
│   │   ├── MainMenu/             # 主菜单关卡
│   │   └── Game/                 # 游戏关卡 (L_Stage_01 ...)
│   ├── Art/                      # 美术资源 (Meshes/Materials/Textures/VFX)
│   ├── Audio/                    # 音频 (BGM/SFX)
│   └── Data/                     # 数据配置 (Curves/Tables)
├── Config/                       # 项目配置
├── Docs/                         # 文档
└── ThunderFighter.uproject       # 项目入口
```

## 🚀 快速开始

### 环境要求
- **Unreal Engine 5.8**（Epic Games Launcher 安装）
- **Visual Studio 2022**（编译器工具链，勾选"使用 C++ 的游戏开发"）
- **推荐 IDE：JetBrains Rider**（也可用 VS2022 自带的 IDE）
- **Windows 10/11**

> 💡 **VS2022 ≠ IDE：** 即使使用 Rider 编写代码，仍然需要安装 VS2022 作为底层构建工具链。Rider 会自动检测并使用 VS2022 的 MSVC 编译器和 Windows SDK。

### 打开项目
1. 克隆仓库
   ```bash
   git clone https://github.com/XiaoZe980/ThunderFighter.git
   ```
2. 右键 `ThunderFighter.uproject` → **Generate Visual Studio project files**
3. 双击 `ThunderFighter.uproject` 打开 UE 编辑器

### 编译
- **在 UE 编辑器中**：工具栏点击 `Compile` 或按 `Ctrl + Alt + F11`
- **在 Rider 中**：`Build → Build Solution`
- **在 VS 中**：打开 `.sln` 文件，按 `F5`

## 📊 开发进度

### C++ 核心系统

| 系统 | 状态 |
|------|------|
| 游戏框架 (GameMode/PlayerController/GameInstance) | ✅ |
| 玩家战机 (移动/射击/生命值) | ✅ |
| 弹幕系统 (阵营/碰撞/弹幕模式) | ✅ |
| 敌机系统 (基类/波次生成/曲线移动) | ✅ |
| UI 系统 (HUD/暂停/结算) | ✅ |
| 道具系统 (武器升级/回血/炸弹/分数) | ✅ |
| 滚动背景 | ✅ |

### 蓝图资产

| 资产 | 状态 |
|------|------|
| Input Actions (IA_*) + IMC_Gameplay | ✅ |
| BP_ThunderFighterGameMode | ✅ |
| BP_ThunderFighterPlayerController | ✅ |
| BP_ThunderFighterPlayerPawn | ✅ |
| BP_Projectile_Player | ✅ |
| BP_Enemy_Grunt | ✅ |
| BP_ThunderFighterHUD | ✅ |
| WBP_GameplayHUD | ✅ |
| 测试关卡 L_Stage_01 | ✅ |

### 已验证功能
- ✅ 玩家 WASD 移动（屏幕边界限制）
- ✅ 俯视相机自动跟随
- ✅ 自动射击（无需按住按键）
- ✅ 子弹命中敌人 → 造成伤害 → 敌人死亡 → 得分
- ✅ 碰撞检测（OnHit + OnOverlap 双事件）

### 待完成
- ⬜ 敌机射击（BP_Projectile_Enemy）
- ⬜ 更多敌机类型（BP_Enemy_Shooter / BP_Enemy_Rammer / BP_Enemy_Boss）
- ⬜ 道具蓝图（BP_Pickup_*）
- ⬜ WBP_GameOver 结算界面
- ⬜ 滚动背景配置
- ⬜ 美术资源替换
- ⬜ 音效与 BGM

## 🎯 游戏特性

- **自动射击** — STG 标准玩法，无需按住按键
- **5 级武器系统** — 单发 → 双发 → 三发 → 四发 → 扇形弹幕
- **波次生成** — 可配置的敌机波次（FSpawnEntry / FEnemyWave）
- **曲线移动** — 使用 UCurveFloat 定义敌机飞行路径
- **多种弹幕模式** — 扇形、圆形、追踪玩家
- **4 种道具** — 武器升级(W)、生命恢复(H)、炸弹(B)、分数加成(S)
- **炸弹清屏** — 一键清除所有敌方弹幕
- **无敌帧** — 受伤后短暂无敌，防止秒杀

## 📝 命名规范

详见 [Docs/AssetPaths.md](Docs/AssetPaths.md)

## 📄 License

MIT
