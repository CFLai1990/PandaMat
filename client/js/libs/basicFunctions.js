(function () {
	var dot=numeric.dot, trans=numeric.transpose, sub=numeric.sub, div=numeric.div, clone=numeric.clone, getBlock=numeric.getBlock,
	add=numeric.add, mul=numeric.mul, svd=numeric.svd, norm2=numeric.norm2, identity=numeric.identity, dim=numeric.dim,
	getDiag=numeric.getDiag, inv=numeric.inv, det = numeric.det, norm2Squared = numeric.norm2Squared, norm1 = numeric.norm1;

	window['Basic'] = {

            KNNG: function (v_data, v_k) {
                var self = this, t_dist = MDS.getSquareDistances(v_data),
                t_KNN = function (vv_d, vv_k){
                	var tt_data = [], tt_indeces = [], tt_sort = function(a,b){return a.data - b.data;}, t_i = 0, tt_count = 0;
                	while(tt_count < vv_k && t_i < vv_d.length){
                		if(vv_d[t_i] != 0){
                            tt_data.push({data: vv_d[t_i], index: t_i});
                            tt_count ++;
                		}
                        t_i ++;
                	}
                	tt_data.sort(tt_sort);
                	var tt_last = tt_data[vv_k - 1].data;
                	for(var i = vv_k; i < vv_d.length; i++){
                		if(vv_d[i] == 0 || vv_d[i] >= tt_last){
                			continue;
                		}else{
                			tt_data.push({data: vv_d[i], index: i});
                			tt_data.sort(tt_sort);
                			tt_data.splice(vv_k);
                			tt_last = tt_data[vv_k - 1].data;
                		}
                	}
                	for(var i in tt_data){
                		tt_indeces.push(tt_data[i].index);
                	}
                	return tt_indeces;
                }, t_KNNG = [];
                for(var i in t_dist){
                	var t_d = t_dist[i], tt_KNN = t_KNN(t_d, v_k);
                	t_KNNG[i] = tt_KNN;
                }
                return t_KNNG;
            },

            KNNGDistance: function(v_ga, v_gb){
                var t_dist = 0, t_n = v_ga.length, t_k = v_ga[0].length;
                for(var i in v_ga){
                    var t_a = v_ga[i], t_b = v_gb[i], t_same = 0, t_all = 0;
                    for(var j in t_a){
                        if(t_b.indexOf(t_a[j]) >= 0){
                            t_same ++;
                            t_all ++;
                        }else{
                            t_all += 2;
                        }
                    }
                    var t_d = 1 - t_same / t_all;
                    t_dist += t_d;
                }
                t_dist = t_dist / t_n * 100
                return t_dist;
            },

            scale: function(v_scales, v_data){
                return [v_scales.x(v_data[0]), v_scales.y(v_data[1])];
            },

            trimArray: function(v_arr, v_prec){
                var t_arr = [];
                for(var i in v_arr){
                    var tt_arr = [];
                    for(var j in v_arr[i]){
                        tt_arr[j] = parseFloat(v_arr[i][j].toFixed(v_prec));
                    }
                    t_arr[i] = tt_arr;
                }
                return t_arr;
            },

            optimizeData: function(v_data){
                for(var i in v_data){
                    for(var j in v_data[i]){
                        var t = v_data[i][j];
                        if(Math.abs(t) < 1e-10){
                            v_data[i][j] =0;
                        }
                    }
                }
            },
	};
})();