var Q = require("q");
var c = require("../c_modules/c_loader.js"), PandaMat = c.require("pandamat");

function initialize(v_logger){
	var t_compute = {
		panda: null,
	};

	function handle(v_par, v_callback){
		if(!v_par){
			v_callback({
				state: false,
				message: "Parameters missing!",
				result: null,
			});
			return;
		}
		var t_data = v_par.data, t_command = v_par.command, t_return = v_par.return, t_id = v_par.id, t_result;
		if(!t_data){
			v_callback({
				state: false,
				message: "Data missing!",
				result: null,
			});
			return;
		}
		if(!t_command){
			v_callback({
				state: false,
				message: "Command missing!",
				result: null,
			});
			return;
		}
		var panda;
		if(!t_compute.panda){
			panda = t_compute.panda = PandaMat.init();
		}else{
			panda = t_compute.panda;
		}
		console.time("PandaMat");
		if(t_data){
			t_result = panda.data(t_data);
		}else{
			t_result = panda.check(t_data);
		}
		if(!t_result.state){
			v_callback(t_result);
			console.timeEnd("PandaMat");
			return;
		}
		t_result = panda.get(t_command, t_return);
		t_result.id = t_id;
		console.timeEnd("PandaMat");
		v_callback(t_result);
	};

	t_compute = {
		handle: handle,
	};
	return t_compute;
}

exports.initialize = initialize;