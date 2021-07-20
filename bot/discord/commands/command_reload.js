const moment = require('moment');
const colors = require('colors');

async function run(discord_bot, message, args, discord, moment) {
    if (["203841691071807488", "203841691071807488"].includes(message.author.id)) {
        try {
            switch (args[0]) {
                case "commands":
                    discord_bot.commands.clear()
                    discord_bot.aliases.clear()
                    console.log(`[${moment().format('DD/MM/YY HH:mm')}]`.italic.yellow + ` HANDLER `.magenta + `${message.author.tag}`.grey + ` reloaded commands!`.red);
                    discord_bot.functions.load_commands()
                    message.channel.send("All commands successfully reloaded.")
                    break;
                case "events":
                    discord_bot.removeAllListeners();
                    console.log(`[${moment().format('DD/MM/YY HH:mm')}]`.italic.yellow + ` HANDLER `.magenta + `${message.author.tag}`.grey + ` reloaded events!`.red);
                    discord_bot.functions.load_events()
                    message.channel.send("All events successfully reloaded.")
                    break;
                case "all":
                    discord_bot.commands.clear()
                    discord_bot.aliases.clear()
                    console.log(`[${moment().format('DD/MM/YY HH:mm')}]`.italic.yellow + ` HANDLER `.magenta + `${message.author.tag}`.grey + ` reloaded commands and events!`.red);
                    discord_bot.functions.load_commands()
                    message.channel.send("All commands successfully reloaded.")
                    discord_bot.removeAllListeners();
                    discord_bot.functions.load_events()
                    message.channel.send("All events successfully reloaded.")
                    break;
                default:
                    message.channel.send("No category defined. Usage: " + discord_bot.cache.configs.discord.prefix + "settings [commands/events/all]")
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