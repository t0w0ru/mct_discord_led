# MCT-SS2021

## Requirements


## Setting up a Discord Application
-------------------------------------------------------------------------------------------------------
- Head to [this](https://discord.com/developers/applications "Discord Developer Portal") link
- Create a new Application
- Under "My Applications", click your newly created Application
- Select the "Bot" tab and click "Add Bot"
- Set the following permissions (these are crucial for our bot to work)
  * View Channels
  * Send Messages
  * Manage Messages
  * Read Message History
  * Add Reactions
- Copy the bots token and insert it [here](bot\discord\data\config.json "config.json")
    > _you should never share this token as it gives the holder full access over your (public) bot_
- Also add the same ip as the ESP module will connect to. This is important for our HTML POST Requests to work.

## Node.js(https://nodejs.org/dist/v14.17.3/)
-----------------------------------------------
- install Node.js v14.17.3(https://nodejs.org/dist/v14.17.3/) (latest LTS)
- in PowerShell: 
  * run "npm i moment"
  * run "npm i discord.js"
  * run "npm i colors"
  * run "npm i node-fetch"
  * run "node index.js" in the "bot" folder of this repository
 
