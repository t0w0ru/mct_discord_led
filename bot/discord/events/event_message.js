const moment = require('moment');
const discord = require('discord.js');
const colors = require('colors');
const log = require('../log.js');

async function run(bot, message) {
    if (message.author.bot) return;
    if (message.channel.type == "dm") return;
    let member = await message.guild.members.fetch(message.author.id);
    if (member == undefined) {
        let e2 = new discord.MessageEmbed()
            .setTitle("internal error occured!")
            .setDescription("couldn't fetch member")
        message.channel.send(e2)
        return
    }

    const args = message.content.split(" ").slice(1);
    const cmd = message.content.toLowerCase().split("").slice(bot.cache.configs.cfg.prefix.length).join("").split(" ")[0];
    let command;

    if (bot.commands.has(cmd)) {
        command = bot.commands.get(cmd);
    } else {
        command = bot.commands.get(bot.aliases.get(cmd));
    };

    if (command) {
        if (command.config.enabled) {
            log.log_date_time(` HANDLER `.blue + `${message.author.tag}`.grey + ` called command `.green + `${command.config.name}`.grey)
            command.run(bot, message, args, discord, moment);

        } else if (command.config.enabled == false) {
            let e2 = new discord.MessageEmbed()
                .setTitle("command disabled")
                .setDescription("\`" + bot.cache.configs.cfg.prefix + command.config.name + "\` disabled")
            message.channel.send(e2)

        } else {
            let e2 = new discord.MessageEmbed()
                .setTitle("command broken")
                .setDescription("\`" + bot.cache.configs.cfg.prefix + command.config.name + "\` seems to be broken, by either a corrupted config or a corrupted file")
            message.channel.send(e2)
        }
    }
};
module.exports = {
    config: {
        name: "message", 
        enabled: true
    },
    run: run
};