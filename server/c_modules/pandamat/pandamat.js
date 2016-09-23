var PandaMat = require("./build/Release/pandamat");
var operators = {
	'+': {name: 'add', priority: 1, parameter: [-1,1]},
	// '-': {name: 'minus', priority: 1, parameter: [-1,1]},
	'\'': {name: 'trans', priority: 100, parameter: [-1]},
	'*': {name: 'dot', priority: 10, parameter: [-1,1]},
	'=': {name: 'equal', priority: 0, parameter: [-1,1]},
}, methods = {
	//注意函数名不能从后面起成子序列，譬如"substring"和"string"
	//一个有效的方法是将所有函数名首字母大写
	// 'Max': 'max',
	// 'Two': 'two',
	'Add': 'add',
	'Normalize': 'normalize',
	'Dist': 'sqrtDist',
}, failResult = {}, tempPrefix = "$PTemp_", tempCount = 0,
Perror = {
	grammar: "Grammar error!",
	method: "Operation not found!",
	bracket: "Bracket missing!",
	variable: "Variable not found!",
	data: "Data missing!",
};

var test_data = {}, test = false;

function isOpr(v_command){
	return operators[v_command];
}

Array.prototype.last = function(v_last){
	if(this.length >= 1){
		if(v_last){
			this[this.length - 1] = v_last;
		}else{
			return this[this.length - 1];
		}
	}else{
		return null;
	}
}

Array.prototype.has = function(v_value){
	return this.indexOf(v_value)>=0;
}

String.prototype.has = function(v_value){
	return this.indexOf(v_value)>=0;
}

Array.prototype.empty = function(){
	return !(this.length > 0);
}

function init(){
	var panda_js = {};

	function record(v_variable){
		if(!this.variables.has(v_variable) && v_variable != "null"){
			this.variables.push(v_variable);
		}
	};

	function remove(v_variable){
		if(this.variables.has(v_variable)){
			this.variables.splice(this.variables.indexOf(v_variable), 1);
		}
	}

	function data(v_data){
		for(var i in v_data){
			this.record(i);
		}
		if(test){
			test_data = v_data;
			return {
				state: true,
				message: "Success!",
				result: [],
			};
		}else{
			return this.panda.save(v_data);
		}
	};

	function getResult(v_command, v_return){
		var t_result, t_commands;
		try{
			t_commands = this.parse(v_command.replace(/ /g, ""), true);
			this.commands = t_commands.result;
			t_result = this.operate(this.commands);
			if(test){
				return {
					state: true,
					message: "Success!",
					result: test_data[t_result],
				};
			}else{
				var t_pResult;
				if(v_return){
					t_pResult = this.panda.get(t_result);
				}else{
					t_pResult = {
						state: true,
						message: "Success!",
						result: [],
					};
				}
				this.clear();
				if(t_pResult.state){
					t_pResult.variables = this.variables;
				}else{
					if(t_result){
						this.remove(t_result);
					}
				}
				return t_pResult;
			}
		}catch(err){
			var t_pResult = {
				state: false,
				message: err,
				result: [],
			};
			if(t_result){
				this.remove(t_result);
			}
			return t_pResult;
		}
	};

	function parse(v_command, v_sign){
		var t_command = [], t_commands = "", t_next = 1, t_sign = false, i = 0, t_brackets = 1;
		while(!t_sign && i < v_command.length){
			var tt_command = v_command[i];
			if(tt_command != "(" && tt_command != ")"){
				t_commands += tt_command;
				if(i == v_command.length - 1){
					if(t_commands != ""){
						t_command.push(t_commands);
						t_commands = "";
					}
				}
			}else{
				if(t_commands != ""){
					t_command.push(t_commands);
					t_commands = "";
				}
				if(tt_command == "("){
					var next_command = v_command.slice(i + 1, v_command.length);
					var t_result = this.parse(next_command, false);
					i += t_result.next;
					t_command.push(t_result.result);
				}else{
					if(tt_command == ")"){
						t_next = i + 1;
						t_sign = true;
						t_brackets --;
					}
				}
			}
			i++;
		}
		if(v_sign){
			t_brackets --;
		}
		if(t_brackets != 0){
			throw Perror.bracket;
		}
		return {
			result: t_command,
			next: t_next,
		}
	};

	function operate(v_commands){
		var t_comma_sign = false, t_commands = [];
		for(var i = 0; i < v_commands.length; i++){
			var t_command = v_commands[i];
			if(typeof(t_command) == "string"){
				if(t_command.has(",")){
					t_comma_sign = true;
					var tt_commands = t_command.split(",");
					for(var t_i = 0; t_i < tt_commands.length; t_i++){
						if(i < (v_commands.length-1) && typeof(v_commands[i+1]) != "string"){
							continue;
						}else{
							var tt_variable = this.operate([tt_commands[t_i]]);
							this.record(tt_variable);
							tt_commands[t_i] = tt_variable;
						}
					}
					t_commands.push(tt_commands.join(","));
				}else{
					t_commands.push(t_command);
				}
			}else{
				var tt_variable = this.operate(t_command);
				if(tt_variable.has(",")){
					var ttt_com = t_commands.last();
					if(!ttt_com){
						throw Perror.grammar;
					}
					var ttt_variable, t_found = false;
					for(var t_i = ttt_com.length - 1; t_i >= 0; t_i--){
						var tt_name = ttt_com.substr(t_i);
						if(methods[tt_name]){
							ttt_variable = this.compute(methods[tt_name], tt_variable.split(","));
							t_found = true;
							break;
						}
					}
					if(!t_found){
						throw Perror.method;
					}
					this.record(ttt_variable);
					ttt_com = ttt_com.substr(0, t_i) + ttt_variable;
					t_commands.last(ttt_com);
				}else{
					if(tt_variable == "null"){
						var ttt_com = t_commands.last();
						if(!ttt_com){
							throw Perror.grammar;
						}
						var ttt_variable, t_found = false;
						for(var t_i = ttt_com.length - 1; t_i >= 0; t_i--){
							var tt_name = ttt_com.substr(t_i);
							if(methods[tt_name]){
								ttt_variable = this.compute(methods[tt_name], []);
								t_found = true;
								break;
							}
						}
						if(!t_found){
							throw Perror.method;
						}
						this.record(ttt_variable);
						ttt_com = ttt_com.substr(0, t_i) + ttt_variable;
						t_commands.last(ttt_com);
					}else{
						this.record(tt_variable);
						t_commands.push("(");
						t_commands.push(tt_variable);
						t_commands.push(")");
					}
				}
			}
		}
		if(t_comma_sign){
			if(t_commands.length > 1){
				var tt_commands = t_commands.join("");
				tt_commands = tt_commands.split(",");
				for(var i = 0; i < tt_commands.length; i++){	
					var tt_variable = this.operate([tt_commands[i]]);
					this.record(tt_variable);
					tt_commands[i] = tt_variable;
				}
				t_commands[0] = tt_commands.join(",");
			}
			return t_commands[0];
		}else{
			if(t_commands.length == 1 && this.variables[t_commands[0]]){
				return t_commands[0];
			}
			var t_result = this.operateUnit(t_commands.join(""));
			return t_result;
		}
	};

	function operateUnit(v_command){
		//是变量的话检查合法性，为空也要处理，有操作的话返回操作结果的变量名
		var t_short = "", t_commands = [], t_oprs = [], t_functions = [];
		var t_result = "null";
		for(var i = 0; i < v_command.length; i++){
			var t_char = v_command[i], t_opr = operators[t_char];
			if(t_opr){
				t_oprs.push(t_opr);
				if(t_short!=""){
					this.record(t_short);
					t_commands.push(t_short);
					t_short = "";
				}
				if(t_char == "="){
					if(t_commands.length != 1){
						throw Perror.grammar;
					}
				}
				t_commands.push(t_opr.name);
			}else{
				switch(t_char){
					case ",":
						throw Perror.grammar;
					break;
					case "(":
						if(t_short != ""){
							var t_func = methods[t_short];
							if(!t_func){
								throw Perror.method;
							}
							t_functions.push(t_func);
							t_short = "";
						}
					break;
					case ")":
						if(!t_functions.empty()){
							var t_func = t_functions.pop(), t_par = [];
							if(t_short != ""){
								t_par.push(t_short);
								this.record(t_short);
								t_short = "";
							}
							var t_variable = this.compute(t_func, t_par);
							this.record(t_variable);
							if(!t_functions.empty()){
								t_short = t_variable;
							}else{
								t_commands.push(t_variable);
							}
						}else{
							this.record(t_short);
							t_commands.push(t_short);
							t_short = "";
						}
					break;
					default:
						t_short += t_char;
					break;
				}
			}
		}
		if(t_short != ""){
			t_commands.push(t_short);
			this.record(t_short);
			t_short = "";
		}
		t_oprs.sort(function(ta, tb){return (tb.priority - ta.priority);});
		while(t_commands.length > 1 && t_oprs.length > 0){
			var t_opr = (t_oprs.splice(0,1))[0], t_ind = t_commands.indexOf(t_opr.name),
			t_par = t_opr.parameter.slice(0);
			for(var i = 0; i < t_par.length; i++){
				var tt_ind = t_ind + t_par[i];
				if(!t_commands[tt_ind]){
					throw Perror.data;
				}
				t_par[i] = t_commands[tt_ind];
			}
			var t_variable = this.compute(t_opr.name, t_par);			
			this.record(t_variable);
			var t_inds = t_opr.parameter.slice(0);
			t_inds.push(0);t_inds.sort();
			var j = 0;
			for(var i = 0; i < t_inds.length; i++){
				var tt_ind = t_ind + t_inds[i] - j;
				if(t_inds[i] == 0){
					t_commands.splice(tt_ind, 1, t_variable);
				}else{
					t_commands.splice(tt_ind, 1);
					j++;
				}
			}
		}
		if(!t_commands.empty()){
			t_result = t_commands[0];
		}
		return t_result;
	};

	function pandaCompute(v_command, v_parameters){
		//直接连接PandaMat，输出变量名
		// console.log(v_command, v_parameters);
		var t_variable_name = tempPrefix + tempCount;
		if(test){
			var t_data = [], t_result, t_length = v_parameters.length;
			for(var i = 0; i < t_length; i++){
				t_data.push(test_data[v_parameters[i]]);
			}
			switch(v_command){
				case "add":
					t_result = t_data[0] + t_data[1];
				break;
				case "multiply":
					t_result = t_data[0] * t_data[1];
				break;
				case "minus":
					t_result = t_data[0] - t_data[1];
				break;
				case "max":
					var tt_max = -Infinity;
					for(var i = 0; i < t_length; i++){
						tt_max = Math.max(t_data[i], tt_max);
					}
					t_result = tt_max;
				break;
				case "two":
					t_result = 2;
				break;
			}
			test_data[t_variable_name] = t_result;
			tempCount ++;
		}else{
			var t_result = this.panda.operate(v_command, v_parameters, t_variable_name, false);
			if(t_result.state){
				if(v_command == "equal"){
					t_variable_name = v_parameters[0];
				}
				tempCount ++;
			}else{
				throw t_result.message;
			}
		}
		return t_variable_name;
	};

	function check(v_variable){
		var t_state = this.variables.has(v_variable);
		if(t_state){
			return {
				state: true,
				message: "Success!",
				result: [],
			};
		}else{
			return {
				state: false,
				message: "Variable not found!",
				result: [],
			};
		}
	};

	function clear(){
		var t_origin = this.variables, t_variables = [], t_remove = [];
		for(var i = 0; i < t_origin.length; i++){
			if(!t_origin[i].has(tempPrefix)){
				t_variables.push(t_origin[i]);
			}else{
				t_remove.push(t_origin[i]);
			}
		}
		this.panda.remove(t_remove);
		tempCount = 0;
		this.variables = t_variables;
	};

	panda_js = {
		panda: new PandaMat(),
		data: data,
		get: getResult,
		operate: operate,
		operateUnit: operateUnit,
		parse: parse,
		check: check,
		remove: remove,
		clear: clear,
		compute: pandaCompute,
		record: record,
		variables: [],
		commands: [],
	};
	return panda_js;
}

exports.init = init;