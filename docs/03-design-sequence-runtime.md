<h1 align="center">Runtime Signal Processing</h1>

This document explains how the In The Depth Game processes button input, task messages, game-loop ticks, and object updates. The game uses the AK event-driven task architecture: each major game object owns a task, receives signals through AK messages, and updates its own state.

## I. Overview

The In The Depth Game is implemented using event-driven tasks.

Each game object owns:

- A dedicated task
- Its own signal handler
- Its own state data
- Its own update logic

The display task (`AC_TASK_DISPLAY_ID`) owns the screen manager and handles screen-level events. During gameplay the active screen `scr_in_the_depth_game` receives the periodic game tick and posts update messages for each game-object task.

Input events from hardware buttons are converted into software signals. Button callbacks always post `AC_DISPLAY_BUTTON_*` signals to `AC_TASK_DISPLAY_ID`; the active screen handler decides what to do with them. During gameplay the screen translates UP/DOWN into a latched `mainsub_dir`.

The main game loop is driven by a periodic timer signal:

```c
ITD_GAME_TIME_TICK
```

The current game tick interval is:

```c
ITD_GAME_TIME_TICK_INTERVAL = 100 ms
```

Main runtime flow:

1. Button callbacks or timers create software signals.
2. Signals are posted into the AK message pool.
3. The AK scheduler dispatches messages to destination task handlers.
4. Each task updates only the state it owns.
5. The screen render reads the latest object state and refreshes the display buffer.

### High Level Architecture

#### 1. Game Start

```mermaid
---
config:
  theme: dark
---
sequenceDiagram
    participant Screen
    participant EEPROM    
    participant AKOS 
    participant Mainsub
    participant Bomb
    participant Spike
    participant Coin
    participant Gift
    participant Boom
    participant Border
    participant Timer
    note over Screen: SCREEN_ENTRY dispatched to scr_game_in_the_depth
    Screen->>EEPROM: itd_game_setting_read(&settings)
    EEPROM->>Screen: settings loaded
    note over EEPROM: Post 7 SETUP signals 
    Screen->>AKOS: ITD_GAME_MAINSUB_SETUP to ITD_GAME_MAINSUB_ID
    Screen->>AKOS: ITD_GAME_BOMB_SETUP to ITD_GAME_BOMB_ID
    Screen->>AKOS: ITD_GAME_SPIKE_SETUP to ITD_GAME_SPIKE_ID
    Screen->>AKOS: ITD_GAME_COIN_SETUP to ITD_GAME_COIN_ID
    Screen->>AKOS: ITD_GAME_GIFT_SETUP to ITD_GAME_GIFT_ID
    Screen->>AKOS: ITD_GAME_BOOM_SETUP to ITD_GAME_BOOM_ID
    Screen->>AKOS: ITD_GAME_BORDER_SETUP to ITD_GAME_BORDER_ID
    note right of Screen:itd_game_state = GAME_PLAY<br>mainsub_dir = MAINSUB_NONE;
    Screen->>Timer: timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_IDLE);
    Screen->>Timer: timer_set(AC_TASK_DISPLAY_ID, ITD_GAME_TIME_TICK, ITD_GAME_TIME_TICK_INTERVAL, TIMER_PERIODIC);
    note over AKOS: AK scheduler dispatches each queued signal to its task (RTC)
    AKOS->>Mainsub: ITD_GAME_MAINSUB_SETUP
    note right of Mainsub: init pose (X, Y),inviciblity_time = 0<br>heart = 3, shield = 0, visible = WHITE
    AKOS->>Bomb: ITD_GAME_BOMB_SETUP
    note right of Bomb: init pose (0, 0), visible = BLACK
    AKOS->>Spike: ITD_GAME_SPIKE_SETUP
    note right of Spike: inti pose (0, 0), type = 0, visible = BLACK
    AKOS->>Coin: ITD_GAME_COIN_SETUP
    note right of Coin: init pose (0, 0), visible = BLACK
    AKOS->>Gift: ITD_GAME_GIFT_SETUP
    note right of Gift: init pose (0, 0), visible = BLACK
    AKOS->>Boom: ITD_GAME_BOOM_SETUP
    note right of Boom: inti pose (0, 0), visible = BLACK
    AKOS->>Border: ITD_GAME_BORDER_SETUP
    note right of Border: score, time, heart, shield = 0, game_over = false
    Screen->>Timer: ITD_GAME_TIME_TICK(periodic, until ITD_GAME_RESET)
    note over Timer: 100ms 

```

<p align="center"><strong><em>Figure 1:</em></strong> Game start sequence logic</p>

#### 2. Game Playing

```mermaid
---
config:
  theme: dark
---
sequenceDiagram
    participant Button
    participant Timer
    participant Timer
    participant AKOS 
    participant Screen
    participant Mainsub
    participant Bomb
    participant Spike
    participant Coin
    participant Gift
    participant Boom
    participant Border
    note over Button, Screen: Button events fire asynchronously between ticks
    Button->>AKOS: AC_DISPLAY_BUTTON_UP_PRESSED to AC_TASK_DISPLAY_ID
    AKOS->>Screen: dispatch 
    note right of Screen: 
    participant Timermainsub_dir = MAINSUB_UP
    Button->>Screen: AC_DISPLAY_BUTTON_UP_RELEASE
    AKOS->>Screen: dispatch
    note right of Screen: if mainsub_dir == MAINSUB_UP<br>mainsub_dir = MAINSUB_NONE
    note over Timer, Screen: Periodic tick (100ms)
    Timer->>AKOS: ITD_GAME_TIME_TICK to AC_TASK_DISPLAY_ID
    AKOS->>Screen: dispatch
    opt [itd_game_state != GAME_PLAY]:
      note right of Screen: break (ignore tick)
    end
    Screen->>AKOS: grid_offset_x++
    alt grid_offset_x >= 8
      note over AKOS, Screen: grid_offset_x = 0  
    end
    participant Timer
    alt buff_icon_display > 0
      note over AKOS, Screen: buff_icon_display--
    end
    alt [mainsub_dir == MAINSUB_UP]
      Screen->>AKOS: ITD_GAME_MAINSUB_GO_UP
    else [mainsub_dir == MAINSUB_DOWN]
      Screen->>AKOS: ITD_GAME_MAINSUB_GO_DOWN
    else [mainsub_dor == MAINSUB_NONE]
      note right of Screen: skip movement
    end
    Screen->>AKOS: ITD_GAME_MAINSUB_UPDATE
    Screen->>AKOS: ITD_GAME_BOMB_SPAWN
    Screen->>AKOS: ITD_GAME_BOMB_UPDATE
    Screen->>AKOS: ITD_GAME_SPIKE_SPAWN
    Screen->>AKOS: ITD_GAME_COIN_SPAWN
    Screen->>AKOS: ITD_GAME_GIFT_SPAWN
    Screen->>AKOS: ITD_GAME_GIFT_UPDATE
    Screen->>AKOS: ITD_GAME_BOOM_UPDATE
    Screen->>AKOS: ITD_GAME_BORDER_CHECK_GAME_OVER
    Screen->>AKOS: ITD_GAME_BORDER_UPDATE
    note over AKOS: AK scheduler dispatches each queued signal(RTC, FIFO)
    Screen->>Mainsub: ITD_GAME_MAINSUB_GO_UP or ITD_GAME_MAINSUB_GO_DOWN
    note right of Mainsub: mainsub.y += MAINSUB_STEP_AXIS_Y<br>mainsub.y < clamp
    Screen->>Mainsub: ITD_GAME_MAINSUB_UPDATE
    note right of Mainsub: check_hit_by_bomb or check_hit_by_spike<br>check_get_coin or check_get_gift
    Screen->>Bomb: ITD_GAME_BOMB_SPAWN
    note right of Bomb: spawn, move active bombs
    Screen->>Bomb: ITD_GAME_BOMB_UPDATE
    note right of Bomb: update bomb's action image
    Screen->>Spike: ITD_GAME_SPIKE_SPAWN
    note right of Spike: spawn, choose spike's type and move active spike
    Screen->>Coin: ITD_GAME_COIN_SPAWN
    note right of Coin: spawn, check overlapped and move active coins 
    Screen->>Gift: ITD_GAME_GIFT_SPAWN
    note right of Gift: spawn, check overlapped and move active gifts
    Screen->>Gift: ITD_GAME_GIFT_UPDATE
    note right of Gift: generate gift's buff
    Screen->>Boom: ITD_GAME_BOOM_UPDATE
    note right of Boom: Update boom's frame
    Screen->>Border: ITD_GAME_BORDER_CHECK_GAME_OVER
    note right of Border: check game status and mainsub's hearts
    Screen->>Border: ITD_GAME_BORDER_UPDATE
    note right of Border: update score, time, mainsub's hearts and shield
```

<p align="center"><strong><em>Figure 2:</em></strong> Gameplay sequence logic</p>

#### 3. Game Reset

```mermaid
---
config:
  theme: dark
---
sequenceDiagram
    participant Border
    participant AKOS 
    participant Timer
    participant Screen
    participant Mainsub
    participant Bomb
    participant Spike
    participant Coin
    participant Gift
    participant Boom
    participant EEPROM
    note over Border: trigger from ITD_GAME_BORDER_CHECK_GAME_OVER<br>mainsub's heart drop to zero
    Border->>AKOS: ITD_GAME_RESET to AC_TASK_DISPLAY_ID
    AKOS->>Screen: dispatch
    opt [itd_game_state != GAME_PLAY]
      note right of Screen: break (already over)
    end
    Note right of Screen: scores.score_now = itd_game_score<br>time_last = itd_game_time(latch for game over screen)
    EEPROM->>Screen: BUZZER_PlaySound(BUZZER_SOUND_GAME_OVER)
    Screen->>Timer: timer_remove(ITD_GAME_TIME_TICK)
    note over AKOS, Screen: Lanch 7 RESET signals (async)
    Screen->>AKOS: ITD_GAME_MAINSUB_RESET to ITD_GAME_MAINSUB_ID
    Screen->>AKOS: ITD_GAME_BOMB_RESET to ITD_GAME_BOMB_ID
    Screen->>AKOS: ITD_GAME_SPIKE_RESET to ITD_GAME_SPIKE_ID
    Screen->>AKOS: ITD_GAME_COIN_RESET to ITD_GAME_COIN_ID
    Screen->>AKOS: ITD_GAME_GIFT_RESET to ITD_GAME_GIFT_ID
    Screen->>AKOS: ITD_GAME_BOOM_RESET to ITD_GAME_BOOM_ID
    Screen->>AKOS: ITD_GAME_BORDER_RESET to ITD_GAME_BORDER_ID
    Screen->>Timer: timer_set(ITD_GAME_EXIT_GAME, ITD_GAME_TIME_EXIT_INTERVAL TIMER_ONE_SHOT)
    note over AKOS: AK dispatches each RESET in FIFO order (RTC)
    AKOS->>Mainsub: ITD_GAME_MAINSUB_RESET
    note right of Mainsub: re-park and reset all mainsub's status
    AKOS->>Bomb: ITD_GAME_BOMB_RESET
    note right of Bomb: init pose at (0, 0) and reset action images
    AKOS->>Spike: ITD_GAME_SPIKE_RESET
    note right of Spike: init pose at (0, 0), reset type
    AKOS->>Coin: ITD_GAME_COIN_RESET
    note right of Coin: init pose at (0, 0)
    AKOS->>Gift: ITD_GAME_GIFT_RESET
    note right of Gift: reset gift's buffs and re-park (0, 0)
    AKOS->>Boom: ITD_GAME_BOOM_RESET
    note right of Boom: clear and reset Boom's action images
    AKOS->>Border: ITD_GAME_BORDER_RESET
    note right of Border: reset all border's values
    note over of Timer: ITD_GAME_TIME_EXIT_INTERVAL / 1000 s
    Timer->>AKOS: ITD_GAME_EXIT_GAME to AC_TASK_DISPLAY_ID
    AKOS->>Screen: dispatch
    note right of Screen: itd_game_state = GAME_OVER<br>SCREEN_TRAN to scr_game_over_handle

```
<p align="center"><strong><em>Figure 3:</em></strong> Game reset sequence logic</p>

## II. Code References
|**Area**|**File**|
|-------|-------|
|Task IDs and task handlers |`application/sources/app/task_list.h`|
|Task table registration|`application/sources/app/task_list.cpp`|
|Signal definitions|`application/sources/app/app.h`|
|Button callback logic|`application/sources/app/app_bsp.cpp`|
|Main game screen logic|`application/sources/app/screens/scr_game_in_the_depth.cpp`|
|Game-over screen logic|`application/sources/app/screens/scr_game_over.cpp`|
|Screen manager|`application/sources/common/screen_manager.cpp`|

## III. Task Ownership
|**Task**|**Responsibility**|**Own Datas**|**Receives Data**|
|--------|-----------------|--------------|-----------------|
|`AC_TASK_DISPLAY_ID`|Screen manager, render scheduling, button routing, central game-tick dispatch|-|All `AC_DISPLAY_*` button signal, `ITD_GAME_SETUP`,`ITD_GAME_TICK`,`ITD_GAME_RESET`|
|`ITD_GAME_MAINSUB_ID`|Player control, check impact and collision|-|`ITD_GAME_MAINSUB_SETUP`,`ITD_GAME_MAINSUB_GO_UP`,`ITD_GAME_MAINSUB_GO_DOWN`,`ITD_GAME_MAINSUB_UPDATE`, `ITD_GAME_MAINSUB_RESET`|
|`ITD_GAME_BOMB_ID`|Bomb's movement and animation|-|`ITD_GAME_BOMB_SETUP`,`ITD_GAME_BOMB_SPAWN`,`ITD_GAME_BOMB_UPDATE`,`ITD_GAME_BOMB_RESET`|
|`ITD_GAME_SPIKE_ID`|Spike's type and movement|-|`ITD_GAME_SPIKE_SETUP`, `ITD_GAME_SPIKE_SPAWN`, `ITD_GAME_SPIKE_RESET`|
|`ITD_GAME_COIN_ID`|Coin's spawn rate and movement|-|`ITD_GAME_COIN_SETUP`,`ITD_GAME_COIN_SPAWN`,`ITD_GAME_COIN_RESET`|
|`ITD_GAME_GIFT_ID`|Gift's movement and generate buff|-|`ITD_GAME_GIFT_SETUP`, `ITD_GAME_GIFT_SPAWN`, `ITD_GAME_GIFT_UPDATE`,`ITD_GAME_GIFT_RESET`|
|`ITD_GAME_BOOM_ID`|Update Boom animation|-|`ITD_GAME_BOOM_SETUP`,`ITD_GAME_BOOM_UPDATE`,`ITD_GAME_BOOM_RESET`|
|`ITD_GAME_BORDER_ID`|Update score, time, heart, shield and check game over|-|`ITD_GAME_BORDER_SETUP`,`ITD_GAME_BORDER_UPDATE`,`ITD_GAME_BORDER_CHECK_GAME_OVER`,`ITD_GAME_BORDER_RESET`|
## IV. Button Event Processing
In "In The Depth", button callbacks always post `AC_DISPLAY_BUTTON_*` signals to `AC_TASK_DISPLAY_ID`. The currently active screen handler then decides what to do with them. The "In The Depth" gameplay screen (`scr_game_in_the_depth`) handles those signals locally; it does not require the BSP to know which screen is active. 
### Button Processing Rules

|**Button**|**BSP posts to AK**|**Active screen**|**Result inside the screen handler**|
|----------|-------------------|-----------------|------------------------------------|
|UP Pressed|`AC_DISPLAY_BUTTON_UP_PRESSED`->`AC_TASK_DISPLAY_ID`|`scr_game_in_the_depth`|`mainsub_dir = MAINSUB_UP`(no message posted)|
|UP Released|`AC_DISPLAY_BUTTON_UP_RELEASED`->`AC_TASK_DISPLAY_ID`|`scr_game_in_the_depth`|If`(mainsub_dir == MAINSUB_UP)`:<br>`mainsub_dir = MAINSUB_NONE`|
|DOWN Pressed|`AC_DISPLAY_BUTTON_DOWN_PRESSED`->`AC_TASK_DISPLAY_ID`|`scr_game_in_the_depth`|`mainsub_dir = MAINSUB_DOWN`(no message posted)|
|DOWN Released|`AC_DISPLAY_BUTTON_DOWN_RELEASED`->`AC_TASK_DISPLAY_ID`|`scr_game_in_the_depth`|If `(mainsub_dir == MAINSUB_DOWN)`:<br>`mainsub_dir = MAINSUB_NONE`|



