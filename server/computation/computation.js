var Q = require("q");
var c = require("../c_modules/c_loader.js"), PandaMat = c.require("pandamat");

function initialize(v_logger){
	var t_compute = {};

	function handle(v_par, v_callback){
		var t_data = v_par.data, t_command = v_par.command, t_result;
		if(!t_data || !t_command){
			v_callback({
				state: false,
				message: "Data / Command missing!",
				result: null,
			})
		}
		var panda = PandaMat.init();
		console.time("PandaMat: ");
		t_result = panda.data(t_data);
		console.timeEnd("PandaMat: ");
		if(!t_result.state){
			v_callback(t_result);
			return;
		}
		v_callback(panda.get(t_command));
	};

	function parse(){

	};

	t_compute = {
		handle: handle,
	};
	return t_compute;
}

exports.initialize = initialize;