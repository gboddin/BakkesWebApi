# BakkesWebApi

The goal is to have a Rocket League stats/event web API to be used : 

- External statistics gathering
- Bots
- Desktop/Mobile applications
- HTML/CSS/JS widgets to be used in OBS (or any streaming solution supporting transparent HTML).

## Websocket

Address: ws://localhost:8323
Events:
 - goal_scored

## HTTP Endpoint

 - http://localhost:8323/players
 - http://localhost:8323/teams


## OBS widgets

goals.html is provided as an example. Load it as transparent html in the browser view.

## To come
 - More events
 - Some more CSS animated widgets

 Libraries required :

  - websocketpp 0.8.1 https://github.com/zaphoyd/websocketpp
  - asio https://think-async.com/Asio/
  - SimpleJson https://github.com/nbsdx/SimpleJSON

  ## Current bugs

  - Cannot unload/reload
  - MMR is always 0