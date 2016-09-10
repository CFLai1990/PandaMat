function initialize(){
	var fs = require('fs');
	var logger = require("tracer").console();
	/*{
		transport : function(data) {
			console.log(data.output);
			var stream = fs.createWriteStream("./stream.log", {
				flags: "a",
				encoding: "utf8",
				mode: 0666
			}).write(data.output+"\n");
		}
	}*/
	return logger;
}

module.exports = {
	initialize: initialize,
};