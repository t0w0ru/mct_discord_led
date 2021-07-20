const colors = require('colors');
const log = require('../log');

async function run(bot, message, args, discord, moment) {
    //t0ru
    try {
        switch (args[0]) {
            case "static":
                let emb = new discord.MessageEmbed()
                    .setTitle("Connecting...")
                    .setDescription("Awaiting response from " + bot.cache.configs.cfg.ip)
                message.channel.send(emb);
                let embed = new discord.MessageEmbed()
                    .setTitle("Success!")
                    .setDescription("LED-MODE: " + `${args[0]}` + "\nLED-COLOR: " + `${args[1]}`);
                break;
            case "colorcycle":
            case "rainbow":
                
                break;
            case "off":

                break;
            default:
                message.channel.send("No led mode defined.\n`Usage: " + bot.cache.configs.cfg.prefix + "toggle [static/rainbow/off/all] [color]`")
                break;
        }
    } catch (e) {
        let e1 = new discord.MessageEmbed()
            .setTitle("error!")
            .setDescription("Oops! An error occured!")
            .addField("error", `\`\`\`js\n${e}\n\`\`\``)
            .setColor("#FF0000")
        message.channel.send(e1);
    }
};
module.exports = {
    config: {
        name: "toggle",
        aliases: ["set"],
        channel: [],
        enabled: true
    },
    run: run
};