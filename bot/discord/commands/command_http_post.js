const colors = require('colors');
const log = require('../log');
const fetch = require('node-fetch');

async function run(bot, message, args, discord, moment) {

    let mode = args[0].toUpperCase()
    let col = args[1].toUpperCase()
    const _body = '{ MODE: ' + `${mode}`+ ', COLOR: ' + `${col}` + ' }';

    fetch('https://ptsv2.com/t/u0yw8-1626867561/post', {
        method: 'POST',
        body:  _body
    })
    .then(checkStatus)
    .then(res => res.text())
    .then(body => {
        let e = new discord.MessageEmbed()
            .setTitle("Successfully posted")
            .setDescription(`Response:  ${body}`)
            .addField("MODE: ", `\`\`\`prolog\n${mode}\n\`\`\``)
            .addField("COLOR: ", `\`\`\`prolog\n${col}\n\`\`\``)
            .setColor("#00FF00")
        message.channel.send(e);
    })
    
    function checkStatus(res) {
        if (res.ok) { // res.status >= 200 && res.status < 300
            return res;
        } else {
            let e1 = new discord.MessageEmbed()
                .setTitle("error!")
                .setDescription("Couldn't connect!")
                .addField("Server responded with:",  `\`\`\`prolog\n${res.status} ${res.statusText}\n\`\`\``)
                .setColor("#34363C")
            message.channel.send(e1);
        }
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
//'http://' + bot.cache.configs.cfg.ip + '/POST'