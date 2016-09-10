 define([
    'require',
    'marionette',
    'underscore',
    'jquery',
    'backbone',
    'config',
    'data',
    'PandaMat',
    ], function(require, Mn, _, $, Backbone, Config, Data, PandaMat){
        'use strict';

        var dot=numeric.dot, trans=numeric.transpose, sub=numeric.sub, div=numeric.div, clone=numeric.clone, getBlock=numeric.getBlock,
        add=numeric.add, mul=numeric.mul, svd=numeric.svd, norm2=numeric.norm2, identity=numeric.identity, dim=numeric.dim,
        getDiag=numeric.getDiag, inv=numeric.inv, det = numeric.det, norm2Squared = numeric.norm2Squared, norm1 = numeric.norm1;

        return window.Datacenter = new (Backbone.Model.extend({
            defaults: function(){
                return {
                };
            },

            initialize: function(url){
                var self = this;
                var t_default = {
                };
                _.extend(this, t_default);
                this.bindAll();
            },

            bindAll: function(){
            },

            start: function(){
                this.trigger("DataCenter__initialized");
            },

            loadData: function(v_path){
                var self = this;
                d3.csv(v_path, function(d){
                });
            },

            panda: function(v_data, v_command, v_callback){
                console.time("PandaMat: ");
                PandaMat.get({
                    data: v_data,
                    command: v_command,
                }, function(v_result){
                    console.timeEnd("PandaMat: ");
                    v_callback(v_result);
                });
            },          
    }))();
});
