var t_KNN = function (vv_d, vv_k){
   var tt_data = [], tt_indeces = [], tt_sort = function(a,b){return a.data - b.data;}, t_i = 0, tt_count = 0;
   while(tt_count < vv_k && t_i < vv_d.length){
      if(vv_d[t_i] != 0){
            tt_data.push({data: vv_d[t_i], index: t_i});
            tt_count++;
        }
        t_i++;
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
    };

var KNNG = function (v_dist, v_k) {
        var self = this, t_dist = v_dist, t_KNNG = [];
        for(var i in t_dist){
          var t_d = t_dist[i], tt_KNN = t_KNN(t_d, v_k);
           t_KNNG[i] = tt_KNN;
    }
     return t_KNNG;
    };

onmessage = function(event){
    var t_d = event.data, t_dist = t_d.dist, t_k = t_d.k;
    var t_KNNG = KNNG(t_dist, t_k);
    postMessage(t_KNNG);
};