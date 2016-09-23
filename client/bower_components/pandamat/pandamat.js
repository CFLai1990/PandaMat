define([
	"./jquery",
	], function($){
		"use strict";

		var PandaMat = (typeof exports === "undefined")?(function pandamat() {}):(exports);
		if(typeof global !== "undefined") { global.PandaMat = PandaMat; }

		PandaMat.version = "1.0.0";
		var serverAddress = 'ws://192.168.10.9:2046', connectionWait = 10000, poolPrefix = "PPool_",
		connectCheckInterval = 5;

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
			var t_stream = new WebSocket(serverAddress);
            t_stream.onopen = function(e){
            	if(v_callback){
	            	v_callback(true);
            	}
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
			var t_message;
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
				var t_d = JSON.parse(e.data).data;
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
			strSend(v_stream, {state: "cloase"});
		};

		function open(v_callback){
			var t_stream = PandaMat.stream;
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
			var t_stream = PandaMat.stream;
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
			var t_panda = v_params.panda,
			t_success = replace(v_params.sucess, sucessGLB),
			t_error = replace(v_params.error, errorGLB);
			var t_stream;
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
			var t_stream = strOpen(function(vv_state){
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
        		if(vv_sucess){
        			if(vv_id && vv_variable){
        				PandaMat.variables = vv_variable;
        			}
        			if(v_success){
	        			v_success(vv_result);
        			}else{
        				if(vv_id){
        					callback(vv_id, vv_result, true);
        				}
        			}
        		}else{
        			if(v_error){
	                	v_error(vv_message);
        			}else{
        				if(vv_id){
        					callback(vv_id, vv_result, false);
        				}else{
        					errorGLB(vv_message);
        				}
        			}
                }
            });
		};

		function cache(v_panda, v_success, v_error){
			var t_id = poolPrefix + PandaMat.pandaCount;
			PandaMat.pandaPool[t_id] = {
				sucess: v_success,
				error: v_error,
			};
			v_panda.id = t_id;
			PandaMat.pandaCount ++;
			return v_panda;
		};

		function callback(v_id, v_result, v_state){
			var t_callback = PandaMat.pandaPool[v_id];
			if(v_state){
				t_callback.sucess(v_result);
			}else{
				t_callback.error(v_callback);
			}
			delete PandaMat.pandaPool[v_id];
		};

		function has(v_variable){
			if(!PandaMat.stream || !v_variable){
				return false;
			}else{
				return (PandaMat.variables.indexOf(v_variable)>=0);
			}
		};

		function clearAll(){
			PandaMat.stream = null;
			PandaMat.variables = [];
			PandaMat.pandaPool = {};
			PandaMat.pandaCount = 0;
		};

		PandaMat.stream = null;
		PandaMat.variables = [];
		PandaMat.pandaPool = {};
		PandaMat.pandaCount = 0;

		PandaMat.has = has;
		PandaMat.compute = compute;
		PandaMat.open = open;
		PandaMat.opened = opened;
		PandaMat.close = close;

		return (window.PandaMat = PandaMat);
	});