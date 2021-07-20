const colors = require('colors');
const moment = require('moment');
const fs = require('fs');
const discord = require('discord.js');
const command_handler = require('./discord_command_handler.js');
const event_handler = require('./discord_event_handler.js');
const bot = new discord.Client();

bot.cache = {
    configs: {
        cfg: null
    }
}

bot.functions = {
    cache_config: function cache_discord_config(bool) {
        fs.readFile(__dirname + "/data/cfg.json", 'utf8', function(err, data) {
            if(err) {
                console.log(err);
                process.exit(5);
            }
            var config = JSON.parse(data)
            bot.cache.configs.cfg = config
            if(bool) {
                bot.login(config.token);
            }   
        })
    },

    load_commands: function() {
        command_handler.handle_commands(bot);
    },

    load_events: function() {
        event_handler.handle_events(bot);
    },
    check_channel: function(command, id) {
        if(command.config.channel.includes(id))
            return true;
        if(command.config.channel.length == 0)
            return true;
        return false;
    }
}

bot.functions.load_commands()
bot.functions.load_events()
bot.functions.cache_config(true)

module.exports = { discord, bot }