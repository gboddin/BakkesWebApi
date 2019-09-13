# BakkesWebApi

The goal is to have a Rocket League stats/event web API to be used : 

- External statistics gathering
- Bots
- Desktop/Mobile applications
- HTML/CSS/JS widgets to be used in OBS (or any streaming solution supporting transparent HTML).

## Websocket events

Address: ws://localhost:8323

There are 2 events atm : `goal_scored` and `game_changed`, feel free to open issues with new requests.

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
{
  "event" : "game_changed",
  "game" : {
    "PlaylistId" : 2,
    "PlaylistName" : "Casual Doubles",
    "SecondsElapsed" : 188.442749,
    "SecondsRemaining" : 198,
    "TeamSize" : 2
  }
}
```

## HTTP Endpoint

The base URL is http://localhost:8323

### `/player`

Gets the current player details

```json
{
    "Assists" : 0,
    "BallTouches" : 22,
    "CurrentBoostAmount" : 0.714390,
    "Goals" : 4,
    "IsBot" : 0,
    "Kills" : 0,
    "MMR" : 1074.588013,
    "PlayerID" : 540,
    "PlayerName" : "L E X",
    "PlayerUniqueID" : 17419247,
    "Saves" : 1,
    "TeamNum" : 0
}
```

### `/players`

Get details from all players in the game

```json
[{
    "Assists" : 0,
    "BallTouches" : 0,
    "CurrentBoostAmount" : 0.333000,
    "Goals" : 0,
    "IsBot" : 0,
    "Kills" : 0,
    "MMR" : 1332.083984,
    "PlayerID" : 349,
    "PlayerName" : "Noobimode",
    "PlayerUniqueID" : 27763934,
    "Saves" : 0,
    "TeamNum" : 0
  }, {
    "Assists" : 0,
    "BallTouches" : 0,
    "CurrentBoostAmount" : 0.333333,
    "Goals" : 0,
    "IsBot" : 0,
    "Kills" : 0,
    "MMR" : 864.298035,
    "PlayerID" : 350,
    "PlayerName" : "TIGERNAYLOR",
    "PlayerUniqueID" : 97585630,
    "Saves" : 0,
    "TeamNum" : 1
  }, {
    "Assists" : 0,
    "BallTouches" : 0,
    "CurrentBoostAmount" : 0.333333,
    "Goals" : 0,
    "IsBot" : 0,
    "Kills" : 0,
    "MMR" : 1155.297974,
    "PlayerID" : 351,
    "PlayerName" : "Hawt Rybz",
    "PlayerUniqueID" : -1532239573,
    "Saves" : 0,
    "TeamNum" : 1
  }, {
    "Assists" : 0,
    "BallTouches" : 0,
    "CurrentBoostAmount" : 0.333333,
    "Goals" : 0,
    "IsBot" : 0,
    "Kills" : 0,
    "MMR" : 1470.308105,
    "PlayerID" : 352,
    "PlayerName" : "cal c00l",
    "PlayerUniqueID" : 94007169,
    "Saves" : 0,
    "TeamNum" : 1
  }, {
    "Assists" : 0,
    "BallTouches" : 0,
    "CurrentBoostAmount" : 0.333333,
    "Goals" : 0,
    "IsBot" : 0,
    "Kills" : 0,
    "MMR" : 699.049988,
    "PlayerID" : 353,
    "PlayerName" : "MyFing3rSlipp3d",
    "PlayerUniqueID" : 1151246163,
    "Saves" : 0,
    "TeamNum" : 1
  }, {
    "Assists" : 0,
    "BallTouches" : 0,
    "CurrentBoostAmount" : 0.333333,
    "Goals" : 0,
    "IsBot" : 0,
    "Kills" : 0,
    "MMR" : 1234.656006,
    "PlayerID" : 354,
    "PlayerName" : "Dave-J07",
    "PlayerUniqueID" : -1027782561,
    "Saves" : 0,
    "TeamNum" : 0
  }, {
    "Assists" : 0,
    "BallTouches" : 0,
    "CurrentBoostAmount" : 0.000000,
    "Goals" : 0,
    "IsBot" : 1,
    "Kills" : 0,
    "MMR" : 100.019989,
    "PlayerID" : 0,
    "PlayerName" : "Heater",
    "PlayerUniqueID" : 0,
    "Saves" : 0,
    "TeamNum" : 0
  }, {
    "Assists" : 0,
    "BallTouches" : 0,
    "CurrentBoostAmount" : 0.000000,
    "Goals" : 0,
    "IsBot" : 1,
    "Kills" : 0,
    "MMR" : 100.019989,
    "PlayerID" : 0,
    "PlayerName" : "Scout",
    "PlayerUniqueID" : 0,
    "Saves" : 0,
    "TeamNum" : 0
  }]
```

### `/teams`

Get teams informations

```json
[{
    "Goals" : 2,
    "Name" : "Blue"
  }, {
    "Goals" : 2,
    "Name" : "1up2down"
  }]
```


### `/game`

Get current game info

```json
{
  "PlaylistId" : 6,
  "PlaylistName" : "Private 1v1",
  "SecondsElapsed" : 0.000000,
  "SecondsRemaining" : 0,
  "TeamSize" : 1
}
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