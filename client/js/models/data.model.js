define([
    'require',
    'marionette',
    'underscore',
    'jquery',
    'config',
    'backbone',
], function(require, Mn, _, $, Config, Backbone) {
    'use strict';

    var dot=numeric.dot, trans=numeric.transpose, sub=numeric.sub, div=numeric.div, clone=numeric.clone, getBlock=numeric.getBlock,
        add=numeric.add, mul=numeric.mul, svd=numeric.svd, norm2=numeric.norm2, identity=numeric.identity, dim=numeric.dim,
        getDiag=numeric.getDiag, inv=numeric.inv, det = numeric.det, norm2Squared = numeric.norm2Squared, norm1 = numeric.norm1;

    var data =  Backbone.Model.extend({
        defaults: {
        },

        initialize: function(){
        },

        update: function(options){
            var t_defaults = {
            };
            _.extend(this, options);
            _.extend(this, t_defaults);
        },
    });
    return data;
});
