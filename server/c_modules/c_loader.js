function load(v_name){
	try{
		return require('./' + v_name + '/' + v_name);
	}catch(err){
		console.log(err);	
		try {
		  return require('./' + v_name + '/build/Release/' + v_name);
		} catch (err) {
		  return require('./' + v_name + '/build/Debug/' + v_name);
		}
	}
}

exports.require = load;