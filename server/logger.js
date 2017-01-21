function initialize(){
	var fs = require('fs');
	var colors = require('colors');
	var logger = require("tracer").colorConsole({
	    filters : [
	               {            
	               	trace : colors.magenta,
		            debug : colors.blue,
		            info : colors.green,
		            warn : colors.yellow,
		            error : [ colors.red, colors.bold ]
	               }
	    ]
	});
	return logger;
}

module.exports = {
	initialize: initialize,
};