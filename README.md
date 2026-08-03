# In the depth game - Game built on AK Embedded Base Kit

## Gameplay Demo:

## Documentation:
| File | Description |
|---|---|
| [README.md](README.md) | Main project overview, hardware information, gameplay rules, and object descriptions. |
| [docs/01-guide-getting-started.md](docs/01-guide-getting-started.md) | Game programming getting started guide. |
| [docs/02-guide-coding-rules.md](docs/02-guide-coding-rules.md) | Some rules for coding game. |
| [docs/03-design-sequence-object.md](docs/03-design-sequence-object.md) | Runtime sequence diagrams for gameplay objects: Gunner, Bullet, Zombie, Car, Bang, Tombstone, and Border. |
| [docs/04-design-sequence-runtime.md](docs/04-design-sequence-runtime.md) | Runtime signal-processing flow for button input, AK task messages, timers, game-loop ticks, object updates, and Mermaid sequence diagrams. |
## Introduction:
In the depth is an runner game built on top of the AK Embedded Base Kit — a hands-on platform for embedded programming enthusiasts to explore event-driven design in depth. While building and playing In the depth,  you put the following core concepts of modern embedded engineering into practice:
  - System design: Modelling complex logic flows with UML.
  - Process management: Coordinating cooperative Tasks and scheduling them efficiently.
  - Communication: Using Signals, Timers, and Messages to react in real time.
  - Control logic: Building robust state machines for the player, bombs and the overall match progression
### I. Hardware:

<table align="center">
  <tr>
    <td align="center"><img src="resources/images/boards/ak-embedded-base-kit-version-3.jpg" alt="AK Embedded Base Kit - STM32L151 - v3.0" width="480"/></td>
  </tr>
</table>
<p align="center"><strong><em>Figure 1:</em></strong> AK Embedded Base Kit - STM32L151</p>

[AK Embedded Base Kit](https://epcb.vn/products/ak-embedded-base-kit-lap-trinh-nhung-vi-dieu-khien-mcu) is an evaluation kit aimed at intermediate and advanced embedded software learners.

The kit integrates a **1.54" OLED LCD**, **3 push buttons**, and **a buzzer** capable of playing short melodies, giving you everything you need to study **event-driven systems** through hands-on game-machine design.
It also exposes **RS485**, the **Qwiic Connect System**, and **Grove** connectors, so it doubles as a convenient prototyping board for real-world embedded projects.

**MCU Overview:**

```text
SoC Name : STM32L151CBT6
RAM      : 16 KB

Flash Partitions Layout
----------------------
[ 0x08000000 - 0x08001FFF ] : Bootloader Partition (8 KB)
=> AK Bootloader

[ 0x08002000 - 0x08002FFF ] : BSF Shared Partition (4 KB)
=> Used for data sharing between Bootloader and Application

[ 0x08003000 - 0x0801FFFF ] : Application Partition (116 KB)
=> Zomwar firmware
```

**MCU Naming Convention:**

| Part | Meaning |
|---|---|
| `STM32` | STMicroelectronics 32-bit MCU family. |
| `L` | Low-power series. |
| `151` | STM32L151 product line. |
| `C` | 48-pin package. |
| `B` | 128 KB Flash memory. |
| `T` | LQFP package. |
| `6` | Industrial temperature grade. |


<table align="center">
  <tr>
    <td align="center"><img src="resources/images/boards/board-view-top-bottom.png" width="900"/></td>
  </tr>
</table>
<p align="center"><strong><em>Figure 2:</em></strong> Board view Top + Bottom </p>

### II. Game Description and Objects:
The following  section will describes the gameplay and core mechanics of **"In the depth**. It serves as a references for ongoing game design and firmware development. 

The game opens on **Wellcome screen**, which has a title for a the game and many others objects to make the environment more lively. 

<table align="center">
  <tr>
    <td align="center"><img src="resources/images/screens/gamewelcome.png" width="900"/></td>
  </tr>
</table>
<p align="center"><strong><em>Figure 3:</em></strong> Wellcome Screen </p>

After players press any button, they will transfer to **Main menu**, which offers the following options: 
- **Dive**: start a new match
- **Setting**: Configure gameplay parameters such as sound, speed
- **Rank**: Show the highest that players can achive
- **Exit**: Leave the menu and return to **Wellcome Screen**

<table align="center">
  <tr>
    <td align="center"><img src="resources/images/screens/gamemenu.png" width="900"/></td>
  </tr>
</table>
<p align="center"><strong><em>Figure 4:</em></strong> Game main menu </p>

<table align="center">
  <tr>
    <td align="center"><img src="resources/images/screens/gameplay.png" width="900"/></td>
  </tr>
</table>
<p align="center"><strong><em>Figure 5:</em></strong> Game play </p>


**Objects in the Game:**

|**Bit map**| **Object Name** | **Description** |
|---|---|---|
|<img src="resources/images/bitmap/mainsub.png" width="100"/> | **Mainsub** | The main object of the game, positioned on the left side of the screen. Moves vertically from the screen border to map's ground.|
|<img src="resources/images/bitmap/bomb.gif" width="100"/> | **Bomb**    | The most dangerous object in the game. Spawn randomly from the right edge of the screen. Move extremely fast and can damage mainsub. Their damge and speed can be configure in the game setting.|
|<img src="resources/images/bitmap/spike_tall.png" width="100"/> | **Spike tall** | Other object which can damge mainsub, spawn from the right edge of the screen but below. Can be extremely dangerous due to it's height|
|<img src="resources/images/bitmap/spike_short.png" width="100"/> | **Spike short** | Like **Spike tall** but shorter. Must be caution when low diving|
|<img src="resources/images/bitmap/coin.png" width="100"/> | **Coin**    | The coin's mechanic like bomb, these objects spawn randomly from the right edge of the screen and move kinda slow. Each coin contain 10 points |
|<img src="resources/images/bitmap/gift.png" width="100"/> | **Gift**    | The most mysterious objects in the game. Spawn random for the right edge of screen with a small amount. These objects contain 4 random rewards will can make the game more easy|
|<img src="resources/images/bitmap/boom.gif" width="100"/> | **Boom**    | The small animation, happen when these is a collosion between the mainsub and bombs or spikes. Have no game effect itself.|


### III. How to Play:
- You control the **Mainsub**. Use the **[Up]** and **[Down]** buttons to move between the 5 firing rows. Holding either button moves the Mainsub faster.
- Bombs, spikes and other objects will appear from the right egde of the map.
- Your mission is to dogde bombs, spikes and collect many coin as you can. The more coins you have, the higher score you get.  
#### Game Mechanics:
- **Scoring:**Each coin you collects will count as 10 point. The total score will be show on the border.
- **Bombs and Spikes**These can damage your mainsub so make sure to dogde it. Each bombs or spikes you hit will make your heart go down by 1. These objects'damage can be configure in the game setting. 
- **Special object:**These is a object called **"Gift"**, which give you a random reward, from bonus heart to a nuke that destroy all surronding objects. 
- **Animation:**To keep the game more lively, there are some extra animation for bomb, sea grass and boom animation when the mainsub get hit. 
- **Game over:**When mainsub's heart drop to zero, the match end, all the objects reset and the score are saved. A short mainsub sinking screen will appear and then the **Game over** appear, which offers 3 options:
  - **Retry:**play again
  - **Rank**: view leaderboard
  - **Home**: Return to main menu

<table align="center">
  <tr>
    <td align="center"><img src="resources/images/screens/gamending.png" width="900"/></td>
  </tr>
</table>
<p align="center"><strong><em>Figure 6:</em></strong> Mainsub sinking </p>

<table align="center">
  <tr>
    <td align="center"><img src="resources/images/screens/gameover.png" width="900"/></td>
  </tr>
</table>
<p align="center"><strong><em>Figure 5:</em></strong> Game over </p>


### IV. Basic Game Sequence Logic

<table align="center">
  <tr>
    <td align="center"><img src="resources/images/design_sequence/basic_game_sequences.png" width="900"/></td>
  </tr>
</table>
<p align="center"><strong><em>Figure 6:</em></strong> Basic game sequences </p>

<h3>Contact Me</h3>
<p>
  <a href="[https://github.com/NguyenKimThanh](https://github.com/KimThanhNguyen1409)">
    <img src="https://img.shields.io/badge/GitHub-KimThanhNguyen-181717?style=for-the-badge&logo=github&logoColor=white"/>
  </a>
  
  <a href="https://www.linkedin.com/in/nguyenkimthanh1409/">
    <img src="https://img.shields.io/badge/LinkedIn-Nguyễn%20Kim%20Thành-0A66C2?style=for-the-badge&logo=linkedin&logoColor=white"/>
  </a>
  
  <a href="mailto:nkimthanh47@gmail.com">
    <img src="https://img.shields.io/badge/Gmail-nkimthanh47%40gmail.com-EA4335?style=for-the-badge&logo=gmail&logoColor=white"/>
  </a>
</p>