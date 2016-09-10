define([
	"./jquery",
	], function($){
		"use strict";

		var PandaMat = (typeof exports === "undefined")?(function pandamat() {}):(exports);
		if(typeof global !== "undefined") { global.PandaMat = PandaMat; }

		PandaMat.version = "1.0.0";
		PandaMat.get = function(v_data, v_callback){
			$.ajax({
				url: "http://vis.pku.edu.cn/PandaMat/pandamat?"+JSON.stringify(v_data),
				type: "GET",
				success: function(v_result){
					if(v_result.state){
						v_callback(v_result.result);
					}else{
						v_callback(null);
						throw v_result.message;
					}
				},
				error: function(err){
					v_callback(null);
					throw "PandaMat: server error!";
				},
			});
		}

		return (window.PandaMat = PandaMat);
	});