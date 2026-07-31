# ⚔️ SWORD BATTLE

> 세키로의 패링·체간(Posture) 시스템을 기반으로 구현한 실시간 1:1 PvP 검술 대전 게임

빠른 템포의 공방이 오가는 대전 게임입니다. 최대 2명이 대전에 참여하고, 나머지 플레이어는 **관전자 시스템**으로 실시간 관람합니다. 게임 시작 전 능력치를 자유롭게 분배해 다양한 전략과 플레이 스타일을 구성할 수 있습니다.

| | |
|---|---|
| **엔진** | Unreal Engine 5.7 |
| **언어** | C++ / Blueprint |
| **네트워크** | Listen Server 기반 멀티플레이 |
| **개발 인원** | 1인 (김우재) |
| **개발 기간** | 2026.06.01 ~ 2026.07.06 |

<br/>

## 주요 기능

- 🗡️ **세키로식 전투** — 패링·체간 시스템, 퍼펙트 가드, 콤보 공격
- 🌐 **실시간 멀티플레이** — Listen Server / RPC / Replication 기반 상태 동기화
- 👀 **관전자 시스템** — 대기 중인 유저가 실시간으로 대전 관람
- 📊 **스탯 분배** — 대전 전 능력치 포인트를 자유롭게 배분
- 🔒 **Server Authority** — 스탯·전투 판정을 서버에서 검증하여 조작 방지

<br/>

## 게임 흐름

```
Title ──▶ Lobby ──▶ Game ──▶ (종료) ──▶ Lobby
```

| Level | 역할 | 네트워크 |
|-------|------|----------|
| **Title** | 세션(방) 생성·검색·참가 | Standalone |
| **Test**  | 허수아비로 전투·조작 연습 | Standalone |
| **Lobby** | 참여자 2인/관전자 선정, 스탯 분배, Ready | Listen Server |
| **Game**  | 1:1 전투 진행 및 관전자 시점 제공 | Listen Server |

- Title → Lobby : `Online Session` 기반으로 방 참가
- Lobby → Game : 두 플레이어 Ready 후 게임 시작 시 `Seamless Travel`로 전환
- Game → Lobby : 대전 종료 후 대기방 복귀

<br/>

## 사용 기술

| 기술 | 사용 목적 |
|------|-----------|
| **Online Session** | 방 생성·검색·참가 기능 구현 |
| **Listen Server / RPC / Replication** | 실시간 멀티플레이 환경에서 플레이어 상태와 게임 데이터 동기화 |
| **Server Authority** | 스탯 배분·전투 판정을 서버에서 검증하여 클라이언트의 임의 조작 방지 |
| **Seamless Travel / CopyProperties** | 레벨 전환 시 플레이어 데이터를 유지하며 게임 상태를 자연스럽게 연속 |
| **Delegate** | Replicated 변수 변경 이벤트를 UI에 전달, 객체 간 결합도를 낮춘 이벤트 기반 UI 구조 |
| **AnimNotify / AnimMontage** | 공격 타이밍·퍼펙트 가드 판정을 애니메이션 프레임과 정확히 동기화 |
| **UI Pooling / UI Widget** | 방 목록 등 반복 위젯을 재사용(Collapsed 처리)하여 생성·제거 비용 최소화 |
| **Enhanced Input System** | 액션·콤보 입력을 Input Action / Mapping Context 기반으로 처리 |

<br/>

## 코드 구조

```
Source/ProjectBattle/
├── Title/          # 타이틀 — 세션 방 생성/검색/참가
│   ├── LobbyListUI, LobbyMainUI, TitleMainUI   # 방 목록 UI (재사용 Pooling)
│   └── TitleGameMode / PlayerController / PlayerState
│
├── Lobby/          # 대기방 — 스탯 분배, 관전자 선정, Ready
│   ├── LobbyGameState                   # 참여자 슬롯(PlayerSlot1/2) 관리
│   ├── LobbyPlayerState                 # 레벨 전환 시 CopyProperties로 데이터 전달
│   ├── LobbyRoomUI, GamePlayerListUI, ObserberListUI, JoinGameUI
│   └── LobbyGameMode / PlayerController
│
├── InGame/         # 인게임 — 전투 판정 및 동기화
│   ├── AttackFunction                   # 공격 트레이스 판정 라이브러리
│   └── Contents/
│       ├── Human                        # 체간/HP/가드 베이스 캐릭터
│       ├── InGamePlayer                 # 입력·콤보·회전·데미지 처리 (Enhanced Input)
│       ├── AnimNotify_*                 # 공격·콤보·퍼펙트가드 타이밍 판정
│       ├── AttackPracticeNPC 등         # 연습용 허수아비 NPC
│       └── InGameGameMode / PlayerState / HUD
│
└── MyGameInstance  # 세션 전역 데이터 보관
```

- **이벤트 기반 UI** — Replicated 변수 변경을 단일 Delegate로 통합해 UI 결합도 최소화
- **UI Pooling** — 방 목록 위젯을 재사용(Collapsed 처리)하여 생성/제거 관리
- **레벨 간 데이터 유지** — `GameInstance` + `PlayerState::CopyProperties`로 상태 연속

<br/>

## 트러블슈팅 — 체간(Posture) 수치 동기화 구조 개선

**문제 배경**

지속적으로 변하는 체간 값을 클라이언트가 매 Tick 계산해 서버로 전달하고, 이를 모든 클라이언트에 Replication하는 구조였습니다.
→ 클라이언트의 값 조작이 가능하고, 전투 내내 불필요한 네트워크 호출이 발생했습니다.

**해결 방법**

실제 체간 값은 서버(Server Authority)에서만 관리하고, 클라이언트는 UI 표시용 회복분만 독립적으로 예측 계산하도록 분리했습니다.
실제 값이 바뀌는 피격 시점에만 서버가 클라이언트로 값을 전달해 보정합니다.

**결과**
- 매 Tick 호출되던 RPC를 피격 시점으로 한정하고, 상시 발생하던 체간 Replication을 제거해 네트워크 호출을 줄이고 값 조작을 차단했습니다.
- 네트워크 동기화 주기와 무관하게 클라이언트가 매 프레임 UI를 갱신하므로, 더 부드러운 체간 게이지 변화를 표현할 수 있었습니다.


<br/>
