# BakkesWebApi

The goal is to have a Rocket League stats/event web API to be used : 

- External statistics gathering
- Bots
- Desktop/Mobile applications
- HTML/CSS/JS widgets to be used in OBS (or any streaming solution supporting transparent HTML).

## Websocket events

Address: ws://localhost:8323

```json
{
  "event" : "goal_scored",
  "player" : {
    "Assists" : 0,
    "BallTouches" : 7,
    "CurrentBoostAmount" : 0.121569,
    "Goals" : 2,
    "Kills" : 0,
    "MMR" : 100.019989,
    "Name" : "Squall",
    "PlayerID" : 0,
    "PlayerUniqueID" : 0,
    "Saves" : 0
  }
}
```

## HTTP Endpoint

The base URL is http://localhost:8323

### `/player`

Gets the current player details

```json
{
    "Assists" : 2,
    "BallTouches" : 34,
    "CurrentBoostAmount" : 0.854902,
    "Goals" : 0,
    "Kills" : 0,
    "MMR" : 1391.078003,
    "Name" : "Noobimode",
    "PlayerID" : 27763934,
    "PlayerUniqueID" : 27763934,
    "Saves" : 0
  }
```

### `/players`

Get details from all players in the game

```json
[{
    "Assists" : 2,
    "BallTouches" : 34,
    "CurrentBoostAmount" : 0.854902,
    "Goals" : 0,
    "Kills" : 0,
    "MMR" : 1391.078003,
    "Name" : "Noobimode",
    "PlayerID" : 27763934,
    "PlayerUniqueID" : 27763934,
    "Saves" : 0
  }, {
    "Assists" : 1,
    "BallTouches" : 30,
    "CurrentBoostAmount" : 0.898039,
    "Goals" : 1,
    "Kills" : 0,
    "MMR" : 1334.280029,
    "Name" : "Hollow_Itchigo",
    "PlayerID" : 27763934,
    "PlayerUniqueID" : 285014072,
    "Saves" : 0
  }, {
    "Assists" : 0,
    "BallTouches" : 37,
    "CurrentBoostAmount" : 0.219608,
    "Goals" : 1,
    "Kills" : 0,
    "MMR" : 1016.713989,
    "Name" : "bola_89",
    "PlayerID" : 27763934,
    "PlayerUniqueID" : 1205544678,
    "Saves" : 1
  }, {
    "Assists" : 0,
    "BallTouches" : 32,
    "CurrentBoostAmount" : 0.686275,
    "Goals" : 3,
    "Kills" : 0,
    "MMR" : 1174.307983,
    "Name" : "SirMDRuffy",
    "PlayerID" : 27763934,
    "PlayerUniqueID" : 2066629203,
    "Saves" : 0
  }, {
    "Assists" : 0,
    "BallTouches" : 17,
    "CurrentBoostAmount" : 0.078431,
    "Goals" : 0,
    "Kills" : 0,
    "MMR" : 495.686005,
    "Name" : "BRSI-SB",
    "PlayerID" : 27763934,
    "PlayerUniqueID" : 1395579408,
    "Saves" : 0
  }, {
    "Assists" : 0,
    "BallTouches" : 17,
    "CurrentBoostAmount" : 0.113725,
    "Goals" : 0,
    "Kills" : 0,
    "MMR" : 495.686005,
    "Name" : "BRSI-SB",
    "PlayerID" : 27763934,
    "PlayerUniqueID" : 1395579408,
    "Saves" : 0
  }, {
    "Assists" : 2,
    "BallTouches" : 35,
    "CurrentBoostAmount" : 0.129412,
    "Goals" : 0,
    "Kills" : 0,
    "MMR" : 1073.645996,
    "Name" : "KingRotia",
    "PlayerID" : 27763934,
    "PlayerUniqueID" : 2072219851,
    "Saves" : 1
  }, {
    "Assists" : 2,
    "BallTouches" : 35,
    "CurrentBoostAmount" : 0.796079,
    "Goals" : 0,
    "Kills" : 0,
    "MMR" : 1073.645996,
    "Name" : "KingRotia",
    "PlayerID" : 27763934,
    "PlayerUniqueID" : 2072219851,
    "Saves" : 1
  }, {
    "Assists" : 0,
    "BallTouches" : 0,
    "CurrentBoostAmount" : 0.000000,
    "Goals" : 0,
    "Kills" : 0,
    "MMR" : 1086.732056,
    "Name" : "KingRotia",
    "PlayerID" : 27763934,
    "PlayerUniqueID" : 2072219851,
    "Saves" : 0
  }]
```

### `/teams`

Get teams informations

```json
[{
    "Goals" : 3,
    "Name" : "M&M'S"
  }, {
    "Goals" : 0,
    "Name" : "10100111"
}]
```

## OBS widgets

goals.html is provided as an example. Load it as transparent html in the browser view.

## To come

- More events
- Some more CSS animated widgets

## Libraries required :

- websocketpp 0.8.1 https://github.com/zaphoyd/websocketpp
- asio https://think-async.com/Asio/
- SimpleJson https://github.com/nbsdx/SimpleJSON
