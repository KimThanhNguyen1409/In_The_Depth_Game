<h1 align="center">Game Object Sequences</h1>

This document describes the runtime sequence of each main object in "In the depth". Each object is handled by its own AK task and receives signals from the screen task (`scr_game_in_the_depth`), button callbacks, the periodic game tick timer, or other object tasks.

## I. Object Summary
|**Object**|**Task ID**|**Handler**|**Main responsibility**|
|----------|-----------|-----------|-----------------------|
|Mainsub | `ITD_GAME_MAINSUB_ID` |`itd_game_mainsub_handler()` |Controls the player's position and checks all collision statuses such as `check_hit_by_bomb` or `check_get_coin`. | 
|Bomb    | `ITD_GAME_BOMB_ID`| `itd_game_bomb_handler()` |Controls spawn rates, moves active bombs, and despawns them when they exit the screen. |
|Spike   | `ITD_GAME_SPIKE_ID`| `itd_game_spike_handler()`|Spawns spikes, assigns their types, moves them, and despawns them when they exit the screen. |
|Coin    | `ITD_GAME_COIN_ID`| `itd_game_coin_handler()`|Spawns coins randomly, moves them toward the player, and despawns them upon exit. |
|Gift    | `ITD_GAME_GIFT_ID`| `itd_game_gift_handler()`|Spawns gifts with random buffs and handles their movement. |
|Border  | `ITD_GAME_BORDER_ID`| `itd_game_border_handler()`|Tracks score, time, heart/shield stats, and checks game-over conditions. | 

The screen task posts `ITD_GAME_TIME_TICK` every `ITD_GAME_TIME_TICK_INTERVAL` (100ms). On each tick the screen task fans out signals to every object task in a fixed order.
## II. Mainsub Object Sequence  
Mainsub owns the player position (`Mainsub`). 
- **Setup:** `ITD_GAME_MAINSUB_SETUP` parks **Mainsub** at `(MAINSUB_AXIS_X, MAINSUB_AXIS_Y)` with `visible = WHITE` and initializes stats (`invincibility_time = 0`, `current_heart = HEART_MAX_NUMBER`, `shield_health = 0`).
- **Input:** Button callbacks update `mainsub_dir` within the Screen task without posting directly to Mainsub.
- **Per-tick:** On each `ITD_GAME_TIME_TICK`, the Screen task translates `mainsub_dir` into `ITD_GAME_MAINSUB_GO_DOWN` or `ITD_GAME_MAINSUB_GO_UP`, and posts `ITD_GAME_MAINSUB_UPDATE`.
    - `UP` / `DOWN` - Moves `mainsub.y` by `MAINSUB_STEP_AXIS_Y`, clamping it within screen bounds.
    - `UPDATE` - Checks all collisions (`bomb`, `spike`, `coin`, `gift`): 
        - **Bomb/Spike:** If not invincible, hitting an obstacle reduces `current_heart` or `shield_health`.
        - **Coin:** Collecting a coin increments the global coin count.
        - **Gift:** Collecting a gift immediately applies its buff effect to the player.
- **Reset:** `ITD_GAME_MAINSUB_RESET` resets the position and hides Mainsub (`visible = BLACK`).
```mermaid
%%{init: {
  "theme": "base",
  "sequence": {
    "actorMargin": 50,
    "noteMargin": 10
  }
}}%%
sequenceDiagram
        participant Button
        participant Screen
        participant Mainsub
        participant Bomb
        participant Spike
        participant Boom
        participant Coin
        participant Gift
        note over Screen: SCREEN_ENTRY                                                                                                     
        Screen->>Mainsub: ITD_GAME_MAINSUB_SETUP                                                                                           
        note over Screen: arm 100 ms periodic tick                                                                                         
        note right of Mainsub: mainsub.x = MAINSUB_AXIS_X<br/>mainsub.y = MAINSUB_AXIS_Y<br/>mainsub.visible = WHITE<br/>mainsub.shield_heart = 0<br/>mainsub.invincibility_time = 0                                                                                      
        note over Button: Button events fire asynchronously (between ticks)                                                                
        Button->>Screen: AC_DISPLAY_BUTTON_DOWN_PRESSED                                                                                    
        note right of Screen: mainsub_dir = MAINSUB_UP                                                                                     
        Button->>Screen: AC_DISPLAY_BUTTON_UP_PRESSED                                                                                      
        note right of Screen: mainsub_dir = MAINSUB_DOWN                                                                                   
        note right of Screen: if mainsub_dir matches release key<br/>mainsub_dir = NONE                                                    
        loop Each ITD_GAME_TIME_TICK
            alt mainsub_dir == MAINSUB_UP                                                                                                  
                Screen->>Mainsub: ITD_GAME_MAINSUB_GO_UP                                                                                   
                note right of Mainsub: mainsub.y -= MAINSUB_STEP_AXIS_Y<br/>clamp > MAINSUB_AXIS_Y_MIN                                     
            else mainsub_dir == MAINSUB_DOWN                                                                                               
                Screen->>Mainsub: ITD_GAME_MAINSUB_GO_DOWN                                                                                 
                note right of Mainsub: mainsub.y += MAINSUB_STEP_AXIS_Y<br/>clamp < MAINSUB_AXIS_Y_MAX                                     
            end
            Screen->>Mainsub: ITD_GAME_MAINSUB_UPDATE                                                                                      
            activate Mainsub
            opt invincibility_time > 0                                                                                                     
                Mainsub->>Mainsub: Toggle visible                                                                                          
                Mainsub->>Mainsub: invincibiBOMB_NUMBER_MAXlity_time--                                                                                    
            end
            opt invincibility_time == 0                                                                                                    
                alt get_hit_by_bomb                                                                                                        
                    Mainsub->>Bomb: bomb.visible = BLACK                                                                                   
                    Mainsub->>Boom: itd_game_boom_spawn()                                                                                  
                    note right of Mainsub: mainsub.shield_heart--<br/>current_heart--
                else get_hit_by_spike
                    Mainsub->>Spike: spike.visible = BLACK
                    Mainsub->>Boom: itd_game_boom_spawn()
                    note right of Mainsub: mainsub.shield_heart--<br/>current_heart--
                end
            end
            opt get_coin
                Mainsub->>Coin: coin.visible = BLACK
                note right of Coin: coin.x = 0
                note right of Mainsub: current_coin++
            end
            opt get_gift
                Mainsub->>Gift: gift.visible = BLACK
                note right of Gift: gift.x = 0
                note right of Mainsub: Apply buff
            end
            deactivate Mainsub
        end
```
<p align="center"><strong><em>Figure 1:</em></strong> Mainsub sequence logic</p>

## III. Bomb Object Sequence
The Bomb task manages the `bombs[BOMB_NUMBER_MAX]` array. Spawn rates are randomly generated during active gameplay.
- **Setup:** `ITD_GAME_BOMB_SETUP` parks all bombs at `(0, 0)` and sets `visible = BLACK`.
- **Per-tick:** 
    - `ITD_GAME_BOMB_SPAWN`: Decrements `bomb_cooldown`. When it reaches 0, spawns a new bomb at a random position with `visible = WHITE`. It then iterates through all active bombs, moving them left by `BOMB_STEP_AXIS_X + bonus_speed`. If a bomb crosses the left border, it is despawned (`visible = BLACK`).
    - `ITD_GAME_BOMB_UPDATE`: Cycles the `action_image` for active bombs to animate them.
- **Reset:** `ITD_GAME_BOMB_RESET` hides and parks all bombs at `(0, 0)`.

```mermaid
%%{init: {
  "theme": "base",
  "sequence": {
    "actorMargin": 50,
    "noteMargin": 10
  }
}}%%
sequenceDiagram
        participant Screen
        participant Bomb
        Screen->>Bomb: ITD_GAME_BOMB_SETUP
        note right of Bomb: bombs[i].x = 0<br>bombs[i].y = 0<br>bombs[i].visible = BLACK
        loop Each ITD_GAME_TIME_TICK
            Screen->>Bomb: ITD_GAME_BOMB_SPAWN
            alt bomb_cooldown > 0
                note right of Bomb: bomb_cooldown--
            else bomb_cooldown == 0
                note right of Bomb: bombs[i].x = random between(BOMB_AXIS_X_MAX, BOMB_AXIS_X_MIN)<br>bombs[i].y = random between(BOMB_AXIS_Y_MAX, BOMB_AXIS_Y_MIN)<br> bombs[i].visible = WHITE<br>Reset bomb_cooldown
            end
            loop For each active bomb (bombs[i].visible == WHITE)
                alt bombs[i].x <= BOMB_DESPAWN_AXIS_X + BOMB_STEP_AXIS_X
                    note right of Bomb: bombs[i].visible = BLACK<br>bombs[i].x = 0
                else 
                    note right of Bomb: bombs[i].x -= (BOMB_STEP_AXIS_X + bonus_speed)
                end
            end
            Screen->>Bomb: ITD_GAME_BOMB_UPDATE
            note right of Bomb: Update bomb frame
            alt bombs[i].action_image >= 4
                note right of Bomb: bombs[i].action_image = 1
            else 
                note right of Bomb: bombs[i].action_image++
            end
        end
        Screen->>Bomb: ITD_GAME_BOMB_RESET
        note right of Bomb: bombs[i].x = 0<br>bombs[i].y = 0<br>bombs[i].visible = BLACK
```
<p align="center"><strong><em>Figure 2:</em></strong> Bomb sequence logic</p>

## IV. Spike Object Sequence
The Spike task manages the `spikes[SPIKE_NUMBER]` array and their types.
- **Setup:** `ITD_GAME_SPIKE_SETUP` parks all spikes at `(0, 0)` with `type = 0` and `visible = BLACK`.
- **Per-tick:** `ITD_GAME_SPIKE_SPAWN` determines the spike type to spawn:
    - `type == 2`: Spawns a `SPIKE_SHORT_TYPE` at specific `(X, Y)` coordinates.
    - `type == 3`: Spawns a `SPIKE_TALL_TYPE` at specific `(X, Y)` coordinates.
    The new spike is set to `WHITE`. Active spikes are then moved left by `SPIKE_STEP_X`. If they cross the left border, they are despawned (`visible = BLACK`).
- **Reset:** `ITD_GAME_SPIKE_RESET` hides and parks all spikes at `(0, 0)`.
```mermaid
%%{init: {
  "theme": "base",
  "sequence": {
    "actorMargin": 50,
    "noteMargin": 10
  }
}}%%
sequenceDiagram
        participant Screen
        participant Spike
        Screen->>Spike: ITD_GAME_SPIKE_SETUP
        note right of Spike: spikes[i].x = 0<br>spikes[i].y = 0<br>spikes[i].type = 0<br>spikes[i].visible = BLACK
        Screen->>Spike: ITD_GAME_SPIKE_SPAWN
        loop Each ITD_GAME_TIME_TICK
            note over Spike: Define spike's type
            alt type == 2
                note right of Spike: spikes[i].type = SPIKE_SHORT_TYPE<br>spikes[i].x = SPIKE_SHORT_SPAWN_X<br>spikes[i].y = SPIKE_SHORT_SPAWN_Y<br>spikes[i].visible = WHITE
            else type == 3
                note right of Spike: spikes[i].type = SPIKE_TALL_TYPE<br>spikes[i].x = SPIKE_TALL_SPAWN_X<br>spikes[i].y = SPIKE_TALL_SPAWN_Y<br>spikes[i].visible = WHITE
            end
            loop Find active spike (spikes[i].visible = WHITE)
        
                alt spikes[i].x <= SPIKE_DESPAWN_AXIS_X + SPIKE_STEP_X
                    note right of Spike: spikes[i].x -= SPIKE_STEP_X
                else
                    note right of Spike: spikes[i].visible = BLACK<br>spikes[i].x = 0
                end         
            end
        end
        Screen->>Spike: ITD_GAME_SPIKE_RESET:
        note right of Spike: spikes[i].x = 0<br>spikes[i].y = 0<br>spikes[i].type = 0<br>spikes[i].visible = BLACK
```
<p align="center"><strong><em>Figure 3:</em></strong> Spike sequence logic</p>

## V. Coin Object Sequence
The Coin task manages the `coins[COIN_NUMBER_MAX]` array. 
- **Setup:** `ITD_GAME_COIN_SETUP` parks all coins at `(0, 0)` with `visible = BLACK`.
- **Per-tick:** `ITD_GAME_COIN_SPAWN` decrements `coin_cooldown`. If 0 and there are no overlapping objects, it spawns a new coin at a random location. It then moves all active coins left by `COIN_STEP_AXIS_X`, despawning them if they cross the left border.
- **Reset:** `ITD_GAME_COIN_RESET` hides and parks all coins at `(0, 0)`.
```mermaid
%%{init: {
  "theme": "base",
  "sequence": {
    "actorMargin": 50,
    "noteMargin": 10
  }
}}%%
sequenceDiagram
        participant Screen
        participant Coins
        Screen->>Coins: ITD_GAME_COIN_SETUP
        note right of Coins: coins[i].x = 0<br>coins[i].y = 0<br>coins[i].visible = BLACK
        loop Each ITD_GAME_TIME_TICK
            Screen->>Coins: ITD_GAME_COIN_SPAWN
            alt coin_cooldown > 0
            note right of Coins: coin_cooldown--
            else coin_cooldown == 0
            note right of Coins: Check overlapped
                alt is_overlapped == 0
                    note right of Coins: coins[i].x = random between(COIN_AXIS_X_MAX, COIN_AXIS_X_MIN)<br>coins[i].y = random between(COIN_AXIS_Y_MAX, COIN_AXIS_Y_MIN)<br>coins[i].visible = WHITE<br>Reset coin_cooldown
                else 
                    note right of Coins: coin_cooldown = 2
                end
            end
            loop For each active coin (coins[i].visible == WHITE)
                alt coins[i].x <= COIN_DESPAWN_AXIS_X + COIN_STEP_AXIS_X
                    note right of Coins: coins[i].visible = BLACK<br>coins[i].x = 0
                else 
                    note right of Coins: coins[i].x -= COIN_STEP_AXIS_X
                end
            end
        end
        Screen->>Coins: ITD_GAME_COIN_RESET:
        note right of Coins: coins[i].x = 0<br>coins[i].y = 0<br>coins[i].visible = BLACK
```
<p align="center"><strong><em>Figure 4:</em></strong> Coin sequence logic</p>

## VI. Gift Object Sequence
The Gift task manages the `gifts[GIFT_NUMBER_MAX]` array.
- **Setup:** `ITD_GAME_GIFT_SETUP` parks all gifts at `(0, 0)` with `visible = BLACK`.
- **Per-tick:** 
    - `ITD_GAME_GIFT_SPAWN`: Decrements `gift_cooldown`. If 0 and space allows, spawns a new gift with a random buff ID (1 to 4). Active gifts are moved left and despawned if they cross the border.
    - `ITD_GAME_GIFT_UPDATE`: Processes logic when a gift is collected by the player (e.g., healing, adding shields, rewarding bonus coins, or activating the NUKE).
- **Reset:** `ITD_GAME_GIFT_RESET` hides and parks all gifts at `(0, 0)`.

```mermaid
%%{init: {
  "theme": "base",
  "sequence": {
    "actorMargin": 50,
    "noteMargin": 10
  }
}}%%
sequenceDiagram                                                                                                                                                                                                                                       
    participant Screen                                                                                                                                                                                                                              
    participant Gift
    participant Bomb
    participant Spike
    Screen->>Gift: ITD_GAME_GIFT_SETUP
    note right of Gift:gifts[i].x = 0<br>gifts[i].y = 0<br>gifts[i].visible = BLACK
    loop Each ITD_GAME_TIME_TICK
        Screen->>Gift: ITD_GAME_GIFT_SPAWN
        alt gift_cooldown > 0
            note right of Gift: gift_cooldown--
        else 
            note right of Gift: Check overlapped
            alt is_overlapped == 0
                note right of Gift: gifts[i].x = random between(GIFT_AXIS_X_MAX, GIFT_AXIS_X_MIN)<br>gifts[i].y = random between(GIFT_AXIS_Y_MAX, GIFT_AXIS_Y_MIN)<br>gifts[i].visible = WHITE<br>gifts[i].buff = random between 1 and 4<br>Reset coin_cooldown
            else
                note right of Gift: gift_cooldown = 2
            end
            loop Find active gift(gifts[i].visible == WHITE)
                alt gifts[i].x <= GIFT_DESPAWN_AXIS_X + GIFT_STEP_AXIS_X
                    note right of Gift: gifts[i].visible = BLACK<br>gifts[i].x = 0
                else 
                    note right of Gift: gifts[i].x -= GIFT_STEP_AXIS_X
                end
            end
        end
        Screen->>Gift: ITD_GAME_GIFT_UPDATE
        loop Find active gift(gifts[i].visible == WHITE)
            note right of Gift: Check mainsub get gift
            alt get_gift
                note right of Gift: Store gift's buff and set timer for displaying
                alt gifts[i].buff == GET_HEART
                    alt mainsub.health < HEART_MAX
                        note right of Gift: mainsub.health++ 
                    end
                else gifts[i].buff == GET_SHIELD
                    note right of Gift: mainsub.shield_health++
                else gifts[i].buff == GET_BONUS_COIN
                    note right of Gift: current_coin += 10
                else gifts[i].buff == GET_NUKE
                    note right of Gift: Activate NUKE
                    Gift->>Bomb: ITD_GAME_BOMB_RESET
                    Gift->>Spike: ITD_GAME_SPIKE_RESET
                end
            end
        end
    end
    Screen->>Gift: ITD_GAME_GIFT_RESET
    note right of Gift:gifts[i].x = 0<br>gifts[i].y = 0<br>gifts[i].visible = BLACK
```
<p align="center"><strong><em>Figure 5:</em></strong> Gift sequence logic</p>

## VII. Boom Object Sequence

The Boom task handles the explosion animations when objects are destroyed.
- **Setup:** `ITD_GAME_BOOM_SETUP` parks all boom instances at `(0, 0)` with `visible = BLACK`.
- **Per-tick:** `ITD_GAME_BOOM_UPDATE` increments the `action_image` animation frame for all active explosions. Once the animation completes (`action_image >= 6`), the explosion is hidden.
- **Reset:** `ITD_GAME_BOOM_RESET` hides and parks all boom instances at `(0, 0)`.

```mermaid
%%{init: {
  "theme": "base",
  "sequence": {
    "actorMargin": 50,
    "noteMargin": 10
  }
}}%%
  sequenceDiagram                                                                                                                                                                                                                                       
        participant Screen                                                                                                                                                                                                                              
        participant Boom(n)
        Screen->>Boom: ITD_GAME_BOOM_SETUP
        Note right of Boom: boom[i].x = 0<br>boom[i].y = 0<br>boom[i].visible = BLACK                                                                                                                   
        Screen->>Boom: ITD_GAME_BOOM_UPDATE
        alt boom[i].action_image >= 6
            Note right of Boom: boom[i].action_image = 1<br>boom[i].visible = BLACK
        else 
            Note right of Boom: boom[i].action_image++
        end
        Screen->>Boom: ITD_GAME_BOOM_RESET
        Note right of Boom: boom[i].x = 0<br>boom[i].y = 0<br>boom[i].visible = BLACK;
       
```
<p align="center"><strong><em>Figure 6:</em></strong> Boom sequence logic</p>

## VIII. Border Object Sequence

The Border task monitors the global game state, including player health, score, and game duration.
- **Setup:** `ITD_GAME_BORDER_SETUP` resets `score`, `time`, `heart` (to 3), `shield` (to 0), and sets `is_game_over = false`.
- **Per-tick:** 
    - `ITD_GAME_BORDER_CHECK_GAMEOVER`: Checks if player hearts have reached 0. Uses `is_game_over` as a latch to send a single `ITD_GAME_RESET` message to the Screen task.
    - `ITD_GAME_BORDER_UPDATE`: Synchronizes the border stats with current game variables (calculates score from coins, mirrors player hearts/shields, increments time).
- **Reset:** `ITD_GAME_BORDER_RESET` resets all tracked variables back to their initial states.

```mermaid
%%{init: {
  "theme": "base",
  "sequence": {
    "actorMargin": 50,
    "noteMargin": 10
  }
}}%%
sequenceDiagram                                                                                                                                                                                                                                       
    participant Screen                                                                                                                                                                                                                              
    participant Border
    Screen->>Border: ITD_GAME_BORDER_SETUP
    note right of Border:itd_game_score = 0<br>itd_game_time = 0<br>itd_game_heart = 3<br>itd_game_shield = 0<br>is_game_over = false
    loop Each ITD_GAME_TIME_TICK
        Screen->>Border: ITD_GAME_BORDER_CHECK_GAMEOVER
        alt itd_game_heart == 0 && is_game_over == false
            note right of Border: is_game_over = true
            Border->>Screen: ITD_GAME_RESET
        end
        Screen->>Border: ITD_GAME_BORDER_UPDATE
        note right of Border:itd_game_score = 10 * current_coin<br>itd_game_heart = mainsub.current_heart<br>itd_game_shield = mainsub.shield_heart<br>itd_game_time++
    end
    Screen->>Border: ITD_GAME_BORDER_RESET
    note right of Border:itd_game_score = 0<br>itd_game_time = 0<br>itd_game_heart = 3<br>itd_game_shield = 0<br>is_game_over = false
```
<p align="center"><strong><em>Figure 7:</em></strong> Border sequence logic</p>

## IX. Per-Tick Signal Order
The screen task `scr_game_in_the_depth` posts the following sequences on every `ITD_GAME_TIME_TICK`:
1. `ITD_GAME_MAINSUB_GO_DOWN` or `ITD_GAME_MAINSUB_GO_UP` (depends on `mainsub_dir`)
2. `ITD_GAME_BOMB_SPAWN`
3. `ITD_GAME_BOMB_UPDATE`
4. `ITD_GAME_SPIKE_SPAWN`
5. `ITD_GAME_COIN_SPAWN`
6. `ITD_GAME_GIFT_SPAWN`
7. `ITD_GAME_GIFT_UPDATE`
8. `ITD_GAME_MAINSUB_UPDATE`
9. `ITD_GAME_BOOM_UPDATE`
10. `ITD_GAME_BORDER_CHECK_GAME_OVER`
11. `ITD_GAME_BORDER_UPDATE`

On `SCREEN_ENTRY` the screen task posts the matching `*_SETUP` signals to all object tasks and starts the periodic `ITD_GAME_TIME_TICK` timer. <br>
On `ITD_GAME_RESET` it removes the timer, posts the `*_RESET` signals, saves `scores.score_now` = `itd_game_score` and `time_last` = `itd_game_time`, transitions to `GAME_OVER`, plays `BUZZER_SOUND_GAME_OVER`, and arms a one-shot `ITD_GAME_EXIT_GAME` timer.
## X. Code References

|**Object**|**Source file**| **Header file**|
|---------|---------------|-----------------|
|Mainsub|`application/sources/app/game/in_the_depth_game/itd_game_mainsub.cpp`|`application/sources/app/game/in_the_depth_game/itd_game_mainsub.h`|
|Bomb|`application/sources/app/game/in_the_depth_game/itd_game_bomb.cpp`|`application/sources/app/game/in_the_depth_game/itd_game_bomb.h`|
|Spike|`application/sources/app/game/in_the_depth_game/itd_game_spike.cpp`|`application/sources/app/game/in_the_depth_game/itd_game_spike.h`|
|Coin|`application/sources/app/game/in_the_depth_game/itd_game_coin.cpp`|`application/sources/app/game/in_the_depth_game/itd_game_coin.h`|
|Gift|`application/sources/app/game/in_the_depth_game/itd_game_gift.cpp`|`application/sources/app/game/in_the_depth_game/itd_game_gift.h`|
|Boom|`application/sources/app/game/in_the_depth_game/itd_game_boom.cpp`|`application/sources/app/game/in_the_depth_game/itd_game_boom.h`|
|Border|`application/sources/app/game/in_the_depth_game/itd_game_border.cpp`|`application/sources/app/game/in_the_depth_game/itd_game_border.h`|
|Screen|`application/sources/app/screens/scr_game_in_the_depth.cpp`|`application/sources/app/screens/scr_game_in_the_depth.h`|