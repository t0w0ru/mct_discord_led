const moment = require('moment');
const colors = require('colors');

async function run(bot, message, args, discord, moment) {
    //t0ru //near
    if (["203841691071807488", "611290610254610514"].includes(message.author.id)) {
        try {
            switch (args[0]) {
                case "commands":
                    bot.commands.clear()
                    bot.aliases.clear()
                    console.log(`[${moment().format('DD/MM/YY HH:mm')}]`.italic.yellow + ` HANDLER `.magenta + `${message.author.tag}`.grey + ` reloaded commands!`.red);
                    bot.functions.load_commands()
                    message.channel.send("All commands successfully reloaded.")
                    break;
                case "events":
                    bot.removeAllListeners();
                    console.log(`[${moment().format('DD/MM/YY HH:mm')}]`.italic.yellow + ` HANDLER `.magenta + `${message.author.tag}`.grey + ` reloaded events!`.red);
                    bot.functions.load_events()
                    message.channel.send("All events successfully reloaded.")
                    break;
                case "all":
                    bot.commands.clear()
                    bot.aliases.clear()
                    console.log(`[${moment().format('DD/MM/YY HH:mm')}]`.italic.yellow + ` HANDLER `.magenta + `${message.author.tag}`.grey + ` reloaded commands and events!`.red);
                    bot.functions.load_commands()
                    message.channel.send("All commands successfully reloaded.")
                    bot.removeAllListeners();
                    bot.functions.load_events()
                    message.channel.send("All events successfully reloaded.")
                    break;
                default:
                    message.channel.send("No category defined. Usage: " + bot.cache.configs.cfg.prefix + "settings [commands/events/all]")
                    break;
            }
        } catch (e) {
            let e1 = new discord.MessageEmbed()
                .setTitle("error!")
                .setDescription("Oops! An error occured!")
                .addField("error", `\`\`\`js\n${e}\n\`\`\``)
                .setColor("#34363C")
            message.channel.send(e1);
        }
    } else {
        let e2 = new discord.MessageEmbed()
            .setTitle("error!")
            .setDescription("Oops! An error occured!\nYou don't have enough permissions to perform this command.")
            .setColor("#34363C")
        message.channel.send(e2);
    }
};

module.exports = {
    config: {
        name: "reload",
        aliases: [],
        channel: [],
        enabled: true
    },
    run: run
};