const discord = require ('discord.js');
const colors = require ('colors');
const log = require ('../log.js');
const moment = require ('moment');

async function run(bot, message) {
    if((message.author.bot) || (message.channel.type == "dm"))
        return;
    let member = await message.guild.members.fetch(message.author.id);
    if(member == undefined) {
        let err = new discord.MessageEmbed()
            .setTitle("internal error occured!")
            .setDescription("couldn't fetch member")
        message.channel.send(err)
        return;
    }
        
    const args = message.content.split(" ").slice(1);
    const cmd = message.content.toLowerCase().split("").slice(bot.cache.configs.cfg.prefix.length).join("").split(" ")[0];
    let command;

    if(bot.commands.has(cmd)) {
        command = bot.commands.get(cmd);
    } else { 
        command = bot.commands.get(bot.aliases.get(cmd));
    }
    if(command) {
        if(command.config.enabled) {
            if(bot.cache.configs.cfg.blocks.includes(message.author.id))
                return;
            if(bot.functions.check_channel(command, message.channel.id)) {
                log.log_date_time(` HANDLER `.magenta + `${message.author.tag}`.grey + ` called command `.green + `${command.config.name}`.grey);
                command.run(bot, message, args, discord, moment);
            } else {
                if(message.deletable)
                    message.delete();
            }
            
        } else if (!command.config.enabled) {
            let e2 = new discord.MessageEmbed()
                .setTitle("command disabled")
                .setDescription("\`" + bot.cache.configs.cfg.prefix + command.config.name + "\` disabled")
            message.channel.send(e2)
        }
    } else {
        let embed = new discord.MessageEmbed()
            .setTitle("command not found")
            .setDescription("please check your spelling.")
        message.channel.send(embed)
    }
}    

module.exports = {
    config: {
        name: "message",
        enabled: true
    },
    run: run
};