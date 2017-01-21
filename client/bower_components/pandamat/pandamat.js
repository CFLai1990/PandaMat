define([
	"jquery",
	], function($){
		"use strict";

		let PandaMat = (typeof exports === "undefined")?(function pandamat() {}):(exports);
		if(typeof global !== "undefined") { global.PandaMat = PandaMat; }

		window.onbeforeunload = function(){
			if(PandaMat.stream){
				close();
			}
		};

		PandaMat.version = "1.0.0";
		let serverAddress = 'ws://vis.pku.edu.cn/PandaMat/Server/', connectionWait = 10000, poolPrefix = "PPool_",
		connectCheckInterval = 5, ipAddress;
		//let ipdf = $.Deferred();
		// $.get("http://ipinfo.io", function(response) {
		//     ipAddress = response.ip;
		//     ipdf.resolve();
		// }, "jsonp");

		function replace(v_origin, v_replace){
			if(v_origin){
				return v_origin;
			}else{
				return v_replace;
			}
		};

		function sucessGLB(v_result){
			console.info("PandaMat: Computation finished!");
		};

		function errorGLB(v_message){
			if(v_message){
				throw "PandaMat: " + v_message;
			}else{
				throw "PandaMat: Unknown error!";
			}
		};

		function strOpen(v_callback, v_glb){
			let t_stream = new WebSocket(serverAddress);
            t_stream.onopen = function(e){
            	if(v_callback){
	            	v_callback(true);
            	}
            	// $.when(ipdf)
            	// .done(function(){
	            // 	strSend(t_stream, {state: "address", data: ipAddress});
            	// });
            };
            t_stream.onclose = function(e){
            	if(!this.used){
	            	errorGLB("Connection is broken!");
            	}
            	if(v_glb){
            		clearAll();
            	}
            }
			return t_stream;
		};

		function wait(v_stream, v_callback, v_interval){
		    if(v_stream.readyState === 1){
		        v_callback();
		    }else{
		        setTimeout(function(){
		            wait(v_stream, v_callback, v_interval);
		        }, v_interval);
		    }
		};

		function strSend(v_stream, v_message){
			let t_message;
			if(typeof(v_message) == "string"){
				t_message = v_message;
			}else{
				t_message = JSON.stringify(v_message);
			}
			wait(v_stream, function(){
				v_stream.send(t_message);
			}, connectCheckInterval);
		};

		function strMessage(v_stream, v_callback){
			v_stream.onmessage = function(e){
				if(!e.data){
					errorGLB();
				}
				let t_d = JSON.parse(e.data).data;
				if(v_callback){
					v_callback(t_d.result, t_d.state, t_d.message, t_d.variables, t_d.id);
				}else{
					if(t_d.state){
						sucessGLB(t_d.result);
					}else{
						errorGLB(t_d.message);
					}
				}
			}
		};

		function strClose(v_stream, v_callback){
			v_stream.onclose = function(e){
            	if(v_callback){
	            	v_callback(true);
            	}
			};
			strSend(v_stream, {state: "close"});
		};

		function open(v_callback){
			let t_stream = PandaMat.stream;
			if(!t_stream){
				PandaMat.stream = t_stream = strOpen(v_callback, true);
				t_stream.used = true;
			}else{
            	if(v_callback){
	            	v_callback(true);
            	}
			}
		};

		function opened(){
			return PandaMat.stream?true:false;
		};

		function close(v_callback){
			let t_stream = PandaMat.stream;
			if(t_stream){
				clearAll();
				strClose(t_stream, v_callback);
			}else{
            	if(v_callback){
	            	v_callback(true);
            	}
			}
		};

		function compute(v_params){
			let t_panda = v_params.panda,
			t_success = replace(v_params.sucess, sucessGLB),
			t_error = replace(v_params.error, errorGLB);
			if(!('global' in t_panda)){
				t_panda['global'] = true;
			}
			if(!('return' in t_panda)){
				t_panda['return'] = false;
			}
			let t_stream;
			if(t_panda.global){
				if(PandaMat.stream){
					t_stream = PandaMat.stream;
				}else{
					open(function(vv_state){
						if(vv_state){
							compute(v_params);
						}
					});
					return;
				}
				t_panda = cache(t_panda, t_success, t_error);
				get(t_stream, t_panda);
			}else{
				computeOneTime(t_panda, t_success, t_error);
			}
		};

		function computeOneTime(v_panda, v_success, v_error){
			let t_stream = strOpen(function(vv_state){
				if(vv_state){
					get(t_stream, v_panda, function(v_result){
						v_success(v_result);
						strClose(t_stream);
					}, function(v_message){
						v_error(v_message);
						strClose(t_stream);
					});
				}
			});
		};

		function get(v_stream, v_panda, v_success, v_error){
            strSend(v_stream, {state: "pandaCompute", data: v_panda});
            strMessage(v_stream, function(vv_result, vv_sucess, 
            	vv_message, vv_variable, vv_id){
        		v_stream.used = true;
        		// console.log(vv_sucess, vv_message, vv_id);
        		if(vv_sucess){
        			if(vv_id && vv_variable){
        				PandaMat.variables = new Set(vv_variable);
        			}
    				if(vv_id){
    					callback(vv_id, vv_result, true);
    				}else{
    					if(v_success){
	    					v_success(vv_result);
    					}else{
    						sucessGLB(vv_result);
    					}
    				}
        		}else{
    				if(vv_id){
    					callback(vv_id, vv_message, false);
    				}else{
    					if(v_error){
	    					v_error(vv_message);
    					}else{
    						errorGLB(vv_message);
    					}
    				}
                }
            });
		};

		function cache(v_panda, v_success, v_error){
			let t_id = poolPrefix + PandaMat.pandaCount;
			PandaMat.pandaPool.set(t_id, {
				sucess: v_success,
				error: v_error,
			});
			v_panda.id = t_id;
			PandaMat.pandaCount ++;
			return v_panda;
		};

		function callback(v_id, v_result, v_state){
			let t_callback = PandaMat.pandaPool.get(v_id);
			if(v_state){
				t_callback.sucess(v_result);
			}else{
				t_callback.error(v_result);
			}
			PandaMat.pandaPool.delete(v_id);
		};

		function has(v_variable){
			if(!PandaMat.stream || !v_variable){
				return false;
			}else{
				return PandaMat.variables.has(v_variable);
			}
		};

		function clearAll(){
			PandaMat.stream = null;
			PandaMat.variables.clear();
			PandaMat.pandaPool.clear();
			PandaMat.pandaCount = 0;
		};

		PandaMat.stream = null;
		PandaMat.variables = new Set();
		PandaMat.pandaPool = new Map();
		PandaMat.pandaCount = 0;

		PandaMat.has = has;
		PandaMat.compute = compute;
		PandaMat.open = open;
		PandaMat.opened = opened;
		PandaMat.close = close;

		return (window.PandaMat = PandaMat);
	});