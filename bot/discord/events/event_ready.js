const colors = require('colors');
const moment = require('moment');
const log = require('../log.js');

async function run(bot_bot) {
    bot_bot.user.setActivity('Waiting for user input', { type: 'STREAMING' })
    bot_bot.user.setStatus('dnd')
    log.log_date_time(` BOT `.red + ` Instance ready!`.green);
    log.log_date_time(` BOT `.red + ` EARLY RELEASE!`.bold);
    log.log_ruler();
};
module.exports = {
config: {
    name: "ready",
    enabled: true
},
run: run
};