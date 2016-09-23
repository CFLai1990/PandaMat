var wsServer = require("ws").Server;
var t_send = function(v_ws, v_message){
	if(v_ws.OPEN){
		var t_message = v_message;
		if(typeof(v_message) != "string"){
			t_message = JSON.stringify(v_message);
		}
		var t_result = v_ws.send(t_message);
		if(t_result == "error"){
			console.log("Warning: Connection not open!");
			v_ws.close();
		}
	}else{
		console.log("Warning: Connection not open!");
		v_ws.close();
	}
}

function initialize (v_server, v_computation, logger) {
	var stream = new wsServer({server: v_server});
	stream.on('connection', function connection(v_ws) {
		var t_computation = v_computation.initialize(logger);
		v_ws.on('message', function incoming(message) {
			var self = this;
			var t_message = JSON.parse(message);
			switch(t_message.state){
				case "pandaCompute":
					t_computation.handle(t_message.data, function(v_result){
						t_send(self, {state: "result", data: v_result});
					});
				break;
				case "close":
					t_computation = null;
					self.close();
				break;
				default:
				break;
			}
		});
	});
	return stream;
}

exports.initialize = initialize;