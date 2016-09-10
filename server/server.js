//服务器负责拆解信息，得到客户端的需求
var http = require("http");
var url = require("url");
var express = require("express");
var app = express();

function initialize(router, v_port){
	app.use("/", router);
	var server = http.createServer(app).listen(v_port, "0.0.0.0");
	return server;
}

exports.initialize = initialize;