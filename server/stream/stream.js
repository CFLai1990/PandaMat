"use strict";
let wsServer = require("ws").Server, url = require('url'), wsPool = new Set(), 
wsPrefix = 'PConnect_', wsCount = 0, logger;
let t_send = function(v_ws, v_message){
	if(v_ws.OPEN){
		let t_message = v_message;
		if(typeof(v_message) != "string"){
			t_message = JSON.stringify(v_message);
		}
		let t_result = v_ws.send(t_message);
		if(t_result == "error"){
			logger.warn("Warning: Connection not open!");
			v_ws.close();
		}
	}else{
		logger.warn("Warning: Connection not open!");
		v_ws.close();
	}
}

let pandaState = function(v_ws, v_open, v_ip){
	if(v_ip){		
		logger.info(v_ws.pID + " IP: " + v_ip);
	}else{
		if(v_open){
			v_ws.pID = wsPrefix + wsCount; wsCount++;
			wsPool.add(v_ws.pID);
			logger.info(v_ws.pID + ' opened');
		}else{
			wsPool.delete(v_ws.pID);
			logger.info(v_ws.pID + ' closed');
		}
	}
}

function initialize (v_server, v_computation, v_logger) {
	let stream = new wsServer({server: v_server});
	logger = v_logger;
	stream.on('connection', function connection(v_ws) {
		    // console.log(v_ws.upgradeReq.headers);
		pandaState(v_ws, true);
		let t_headers = v_ws.upgradeReq.headers,
			t_proxyList = t_headers['x-forwarded-for'].split(","),
			t_address = t_proxyList[0];
		let t_computation = v_computation.initialize(logger, v_ws.pID, t_address);
		v_ws.on('message', function incoming(message) {
			let t_message = JSON.parse(message);
			switch(t_message.state){
				case "address":
					t_address = t_message.data;
					pandaState(v_ws, null, t_address);
					t_computation.getIP(t_address);
				break;
				case "pandaCompute":
					t_computation.handle(t_message.data, v_result => {
						t_send(this, {state: "result", data: v_result});
					});
				break;
				case "close":
					t_computation.exit();
					t_computation = null;
					pandaState(v_ws, false);
					this.close();
				break;
				default:
				break;
			}
		});
		v_ws.on("close", function(){
			// pandaState(v_ws, false);
		});
		v_ws.on('error', (err) => {
		  if (err.message === 'no handler for opcode 7') {
		  	console.log(v_ws.upgradeReq.headers);
		    // console.log(v_ws.upgradeReq.connection.remoteAddress);
		    // or check the headers if you are behind a reverse proxy
		  }
		});
	});
	return stream;
}

exports.initialize = initialize;