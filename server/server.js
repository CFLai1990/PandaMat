//服务器负责拆解信息，得到客户端的需求
var http = require("http");
var url = require("url");
var express = require("express");
var bodyParser = require("body-parser");
var app = express();

function initialize(router, v_port){
	app.use("/", router);
	// app.use(bodyParser.urlencoded({ extended: false }));
	// app.use(bodyParser.json());
	// app.post("/pandamat", router.compute);
	var server = http.createServer(app).listen(v_port, "0.0.0.0");
	return server;
}

exports.initialize = initialize;