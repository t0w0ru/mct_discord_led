# MCT-SS2021

## Requirements:


### Setting up a Discord Application
-------------------------------------------------------------------------------------------------
- Head to the [Discord Developer Portal](https://discord.com/developers/applications)
- Create a new Application
- Under "My Applications", click your newly created Application
- Select the "Bot" tab and click "Add Bot"
- Set the following permissions (these are crucial for our bot to work)
  * View Channels
  * Send Messages
  * Manage Messages
  * Read Message History
  * Add Reactions
- Copy the bots token and insert it [here](bot\discord\data\cfg.json "config.json")
    > _you should never share this token as it gives the holder full access over your (public) bot_ 
- Also add the ip the ESP module connected to (once you're done setting it up). This is important for our HTTP POST Requests to work.
  
### Node.js(https://nodejs.org/dist/v14.17.3/)
-------------------------------------------------------------------------------------------------
- install Node.js v14.17.3(https://nodejs.org/dist/v14.17.3/) (latest LTS)
- in PowerShell: 
  * run "npm i moment"
  * run "npm i discord.js"
  * run "npm i colors"
  * run "npm i node-fetch"
  * run "node index.js" in the "bot" folder of this repository
  
### ArduinoJSON(https://arduinojson.org/)
-------------------------------------------------------------------------------------------------
- download(https://github.com/bblanchon/ArduinoJson.git) and extract (copy /src filepath)
- in CCS, select the "WebServer" project and press ALT+ENTER (alternatively right-click and select properties)
- under "Paths and Symbols" click "Add..." for each Assembly, GNU C and GNU C++ and paste the directory-link to the /src folder
- next, in the ESPs [Makefile](ESP\WebServer\Makefile), add "INCLUDE_DIRS += <the /src folder path>" after the last INCLUDE_DIRS
- Compile/Upload the WebServer to the ESP8266
  
### LED-SETUP
-------------------------------------------------------------------------------------------------
- this project specifically requires "WS2812b" LEDs to work.
- in the [main.cpp file](MSP\MCT2021_FinnDriediger\src\main.cpp), you will have to edit the value of "total_leds", to be the amount of leds on your led strips.
