const fs = require('fs');
const colors = require('colors');
const moment = require('moment');
const discord = require('discord.js');
const log = require('./log.js');

async function handle_commands(bot) {
    bot.commands = new discord.Collection();
    bot.aliases = new discord.Collection();
    fs.readdir(__dirname + '/commands/', (err, files) => {
        
        if (err) return console.log(err);
        let jsfile = files.filter(f => f.split(".").pop() === "js");
        if (jsfile.length <= 0) {
            log.log_date_time(` HANDLER`.magenta + ` No Commands found!`.red);
            return;
        }

        jsfile.forEach((f, i) => {
            delete require.cache[require.resolve(`./commands/${f}`)]
            let props = require(`./commands/${f}`);
            bot.commands.set(props.config.name, props);
            props.config.aliases.forEach(alias => {
                bot.aliases.set(alias, props.config.name);
            });
            log.log_date_time(` HANDLER`.magenta + ` ${f}`.grey + ` loaded!`.green);
        });

        function loaded2() {
            log.log_date_time(` HANDLER`.magenta + ` ${jsfile.length}`.grey + ` commands loaded!`.green)
        }
        setTimeout(loaded2, 50)
    });
}
module.exports.handle_commands = handle_commands;