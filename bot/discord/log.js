const moment = require('moment');

function log_date_time(message) {
    console.log(`[${moment().format('DD/MM/YY, h:mm a')}]`.italic.yellow + ' ' + message);
} 
module.exports.log_date_time = log_date_time;

function log_ruler() {
    console.log(`>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>`);
}
module.exports.log_ruler = log_ruler;