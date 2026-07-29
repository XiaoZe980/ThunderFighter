# ThunderFighter 资产路径规划

## 目录结构

```
Content/
├── Blueprints/                    # 蓝图资产
│   ├── Core/                      # 框架类 BP
│   │   ├── BP_ThunderFighterGameMode
│   │   ├── BP_ThunderFighterPlayerController
│   │   ├── BP_ThunderFighterGameInstance
│   │   └── BP_ThunderFighterHUD
│   ├── Player/                    # 玩家相关
│   │   └── BP_ThunderFighterPlayerPawn
│   ├── Enemies/                   # 敌机 BP
│   │   ├── BP_Enemy_Grunt         (杂兵 · 直线移动)
│   │   ├── BP_Enemy_Shooter       (射击型 · 定点射击)
│   │   ├── BP_Enemy_Rammer        (撞击型 · 冲向玩家)
│   │   └── BP_Enemy_Boss          (Boss · 多阶段)
│   ├── Projectiles/               # 弹幕 BP
│   │   ├── BP_Projectile_Player   (玩家子弹)
│   │   └── BP_Projectile_Enemy    (敌方子弹)
│   ├── Pickups/                   # 道具 BP
│   │   ├── BP_Pickup_WeaponUpgrade
│   │   ├── BP_Pickup_HealthRestore
│   │   ├── BP_Pickup_Bomb
│   │   └── BP_Pickup_ScoreBonus
│   ├── Background/                # 背景
│   │   └── BP_ScrollingBackground
│   └── Components/                # 蓝图组件（可选）
│
├── Input/                         # 输入配置
│   ├── Actions/                   # Input Action 资源
│   │   ├── IA_MoveHorizontal      (float)
│   │   ├── IA_MoveVertical        (float)
│   │   ├── IA_Fire                (bool)
│   │   ├── IA_Bomb                (bool)
│   │   └── IA_Pause               (bool)
│   └── Mappings/                  # Input Mapping Context
│       └── IMC_Gameplay
│
├── Maps/                          # 关卡
│   ├── MainMenu/
│   │   └── L_MainMenu             (主菜单关卡)
│   └── Game/
│       ├── L_Stage_01             (第1关)
│       ├── L_Stage_02             (第2关)
│       └── ...
│
├── UI/                            # 用户界面
│   └── Widgets/                   # UMG Widget 蓝图
│       ├── WBP_GameplayHUD        (游戏内 HUD)
│       ├── WBP_GameOver           (结算界面)
│       ├── WBP_PauseMenu          (暂停菜单)
│       └── WBP_MainMenu           (主菜单)
│
├── Art/                           # 美术资源
│   ├── Meshes/                    # 静态网格体
│   ├── Materials/                 # 材质
│   ├── Textures/                  # 纹理
│   └── VFX/                       # 特效
│
├── Audio/                         # 音频
│   ├── BGM/                       # 背景音乐
│   └── SFX/                       # 音效
│
└── Data/                          # 数据配置
    ├── Curves/                    # 曲线（敌机移动路径等）
    │   ├── Curve_SineWave         (正弦波移动)
    │   ├── Curve_SpiralIn         (螺旋入场)
    │   └── Curve_LerpToPosition   (移动到目标位置)
    └── Tables/                    # 数据表
        ├── DT_EnemyConfig         (敌机属性配置)
        └── DT_WaveConfig          (波次配置)
```

## 命名规范

| 前缀 | 类型 | 示例 |
|------|------|------|
| `BP_` | Blueprint Class | `BP_Enemy_Grunt` |
| `IA_` | Input Action | `IA_Fire` |
| `IMC_` | Input Mapping Context | `IMC_Gameplay` |
| `L_` | Level / Map | `L_Stage_01` |
| `WBP_` | Widget Blueprint | `WBP_GameplayHUD` |
| `DT_` | Data Table | `DT_EnemyConfig` |
| `Curve_` | Curve | `Curve_SineWave` |
| `M_` | Material | `M_Ship` |
| `MI_` | Material Instance | `MI_Ship_Red` |
| `SM_` | Static Mesh | `SM_Ship_Player` |
| `T_` | Texture | `T_Ship_Diffuse` |

## C++ ↔ Blueprint 映射关系

| C++ 类 (Source/) | Blueprint (Content/) |
|------------------|----------------------|
| `AThunderFighterGameMode` | `Blueprints/Core/BP_ThunderFighterGameMode` |
| `AThunderFighterPlayerController` | `Blueprints/Core/BP_ThunderFighterPlayerController` |
| `UThunderFighterGameInstance` | `Blueprints/Core/BP_ThunderFighterGameInstance` |
| `AThunderFighterHUD` | `Blueprints/Core/BP_ThunderFighterHUD` |
| `AThunderFighterPlayerPawn` | `Blueprints/Player/BP_ThunderFighterPlayerPawn` |
| `AEnemyBase` | `Blueprints/Enemies/BP_Enemy_*` |
| `AProjectileBase` | `Blueprints/Projectiles/BP_Projectile_*` |
| `APickupBase` | `Blueprints/Pickups/BP_Pickup_*` |
| `AScrollingBackground` | `Blueprints/Background/BP_ScrollingBackground` |
