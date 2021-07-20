const discord = require('discord.js');
const fs = require('fs');
const colors = require ('colors');
const log = require ('./log.js')

async function handle_events(bot) {
    bot.events = new discord.Collection();
    fs.readdir(__dirname + '/events', (err, files) => {

        if (err) return console.log(err);
        let jsfile = files.filter(f => f.split(".").pop() === "js");
        if (jsfile.length <= 0) {
            log.log_date_time(` HANDLER`.magenta + ` No events found!`.red);
            return;
        }

        jsfile.forEach((f, i) => {
            delete require.cache[require.resolve(`./events/${f}`)]
            let props = require(`./events/${f}`);
            if (props.config.enabled == false) return;
            bot.on(props.config.name, props.run.bind(null, bot));
            log.log_date_time(` HANDLER`.magenta + ` ${f}`.grey + ` loaded!`.green);
        });


        function loaded() {
            log.log_date_time(` HANDLER`.magenta + ` ${jsfile.length}`.grey + ` events loaded!`.green);
        }
        setTimeout(loaded, 50);
    });
}
module.exports.handle_events = handle_events;


