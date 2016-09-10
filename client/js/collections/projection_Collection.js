define([
    'require',
    'marionette',
    'underscore',
    'jquery',
    'config',
    'backbone',
    'Projection_Model',
], function(require, Mn, _, $, Config, Backbone, Projection_Model) {
    'use strict';

    var dot=numeric.dot, trans=numeric.transpose, sub=numeric.sub, div=numeric.div, clone=numeric.clone, getBlock=numeric.getBlock,
        add=numeric.add, mul=numeric.mul, svd=numeric.svd, norm2=numeric.norm2, identity=numeric.identity, dim=numeric.dim,
        getDiag=numeric.getDiag, inv=numeric.inv;
    var abs = Math.abs, sqrt = Math.sqrt, pow = Math.pow;

    var Projection_Collection = Backbone.Collection.extend({
        model: Projection_Model,

        initialize: function(){
            var t_defaults = {
                count: 0,
                data: null,
                projection: null,
                coordinates: null,
                basis1: null,
                basis2: null,
                frames: 10,
                nowFrame: null,
                timer: null,
                transCoordinates: null,
                interval: Config.get("transition").interval,
                precision: 7,
            };
            _.extend(this, t_defaults);
        },

        getProjection: function(v_subspace){
            var self = this, t_data = trans(Config.get("data").array), t_array = [];
            if(!self.data){
                self.data = Config.get("data").array;
            }
            for(var i in v_subspace){
                if(v_subspace[i]){
                    t_array.push(t_data[i]);
                }
            }
            t_array = trans(t_array);
            var t_cords = MDS.getCoordinates(t_array, true);
            t_cords = self.fixCoordinates(t_cords, v_subspace);
            self.updateCoordinates(self.data, t_cords);
            // self.projection = MDS.byData(self.data);
            // self.trigger("ProjectionCollection__ShowProjection", self.projection);
        },

        fixCoordinates: function(v_cords, v_sub){
            var t_cords = [], t_count = 0;
            for(var i in v_sub){
                if(v_sub[i]){
                    t_cords[i] = v_cords[t_count];
                    t_count ++;
                }else{
                    t_cords[i] = [0,0];
                }
            }
            return t_cords;
        },

        updateCoordinates: function(v_data, v_cord){
            var self = this, v_sign = false;
            self.basis1 = self.coordinates;
            if(!self.basis1){
                self.basis1 = v_cord;
                v_sign = true;
            }
            self.coordinates = v_cord;
            self.basis2 = v_cord;
            if(self.basis1){
                var t_diff = self.getTransform();
                if(!t_diff && !v_sign){
                    self.coordinates = clone(self.basis1);
                    return;
                }else{
                    self.nowFrame = 0;
                    clearInterval(self.timer);
                    self.timer = setInterval(function(){
                        self.coordinates = self.transCoordinates[self.nowFrame];
                        self.projection = dot(v_data, self.coordinates);
                        self.trigger("ProjectionCollection__ShowProjection", (self.nowFrame == 0), (self.nowFrame == 0?self.transCoordinates[self.frames - 1]:null));
                        if(self.nowFrame == self.frames - 1){
                            clearInterval(self.timer);
                            self.nowFrame = 0;
                        }
                        self.nowFrame = self.nowFrame+1;
                    }, self.interval);
                }
            }
        },

        getTransform: function(){
            var self = this; self.transCoordinates = [];
            var B=[], inds=[], angles=[], is=[], t_different=false, same_inds=[], same_angles=[];
            var t = dot(trans(self.basis1), self.basis2);
            if(abs(abs(t[0][0]) - abs(t[0][1]))<1e-10) t[0][1] = t[0][0] * (t[0][1] * t[0][0] > 0?1:-1);
            if(abs(abs(t[1][0]) - abs(t[1][1]))<1e-10) t[1][1] = t[1][0] * (t[0][1] * t[0][0] > 0?1:-1);//Necessary to avoid NaN in SVD
            // if(abs(abs(t[1][0]) - abs(t[0][0]))<1e-10) {
            //     var t_sign = t[1][0] * t[0][0] > 0?1:-1, t_num = sqrt(1 - pow(t[1][0], 2));
            //     t[1][0] = t[0][0] * t_sign;
            //     t[1][1] = t_num * (t[1][1] * t_num > 0?1:-1);
            //     t[0][1] = t_num * (t[0][1] * t_num > 0?1:-1);
            // }
            // console.log(clone(t));
            t = svd(t, self.precision);
            Config.set("test", {a: clone(self.basis1), b: clone(self.basis2)});
            var G1=trans(dot(self.basis1, t.U)), G2=trans(dot(self.basis2, t.V));
            var rotG2=dot(t.V,trans(t.U));
            if(Math.pow(rotG2[0][0]-rotG2[1][1], 2)>0.000001 ||
                rotG2[1][0]*rotG2[0][1]>0){
                var t_basis=trans(self.basis2);
                t_basis=[t_basis[1],t_basis[0]];
                self.basis2=clone(trans(t_basis));
                t = svd(dot(trans(self.basis1), self.basis2));
                G1=trans(dot(self.basis1, t.U)), G2=trans(dot(self.basis2, t.V));
                rotG2=dot(t.V,trans(t.U))
            }
            for(var i in t.S){
                B.push(G1[i]);
                if(Math.abs(t.S[i])<1-1e-10){
                    t_different=true;
                    var g=sub(G2[i], mul(G1[i], div(dot(G1[i], G2[i]), dot(G1[i], G1[i]))));//orthogonalize
                    g=div(g, norm2(g));
                    B.push(g);
                    is.push(i);
                    same_inds.push([B.length-2, B.length-1]);
                    inds.push([B.length-2, B.length-1]);
                    angles.push(Math.acos(t.S[i]));
                    same_angles[i]=Math.acos(t.S[i]);
                }else{
                    same_inds.push(i);
                }
            }
            if(!t_different){
                self.basis2 = clone(self.basis1);
                for(var j = 0; j < self.frames; j++){
                    self.transCoordinates.push(self.basis2);
                }
                return t_different;
            }
            {
                var t_signs=[],t_basis=[],t_starts=[],t_ends=[];
                for(var i=0; i<same_inds.length; i++){
                    if(same_inds[i].length==1){
                        t_basis.push(B[same_inds[i]]);
                        continue;
                    }
                    t_starts[i]=B[same_inds[i][0]], t_ends[i]=B[same_inds[i][1]];
                    var tg1=add(mul(t_starts[i],Math.cos(same_angles[i])),mul(t_ends[i],Math.sin(same_angles[i]))),
                    tg2=G2[i];
                    t_signs[i]=norm2(sub(tg2,tg1))<0.000001?1:-1;
                    t_basis.push(t_starts[i]);
                }
                for(var j=0; j<self.frames; j++){
                    for(var i=0; i<same_inds.length; i++){
                        if(same_inds[i].length==1){
                            continue;
                        }
                        var t_ang=(j+1)/self.frames*same_angles[i];
                        t_basis[i]=add(mul(t_starts[i],Math.cos(t_ang)),mul(t_ends[i],t_signs[i]*Math.sin(t_ang)));
                    }
                    var t_b=dot(t.U,t_basis);
                    t_b=trans(t_b);
                    if(j==self.frames - 1){
                        // console.log(norm2(sub(t_basis,G2)));
                        Basic.optimizeData(t_b);
                        self.basis2 = clone(t_b);
                    }
                    self.transCoordinates.push(t_b);
                }
            }
            return t_different;
        },

        update: function(){
            var self = this;
            self.clearAll();
        },

        clearAll: function(){
            this.reset();
            var t_defaults = {
                count: 0,
                data: null,
                projection: null,
                coordinates: null,
                basis1: null,
                basis2: null,
                frames: 10,
                nowFrame: null,
                timer: null,
                transCoordinates: null,
                interval: Config.get("transition").interval,
            };
            _.extend(this, t_defaults);
            this.trigger("ProjectionCollection__ClearAll");
        },
    });
    return Projection_Collection;
});
