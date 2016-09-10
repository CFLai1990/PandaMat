/**
 * Created by aji on 15/7/13.
 */
define([
    'require',
    'marionette',
    'underscore',
    'jquery',
    'backbone',
    'datacenter',
    'config',
    'text!templates/app.tpl'
], function(require, Mn, _, $, Backbone, Datacenter, Config, Tpl) {
    'use strict';

    return Mn.LayoutView.extend({

        tagName: 'div',

        template: _.template(Tpl),

        attributes:{
            'style' : 'width: 100%; height: 100%;'
        },

        regions:{
            'submap': '#leftTop',
            'projection': '#rightTop',
        },

        initialize: function (options) {
            var self = this, t_default = {
                ready: false,
                layout: Config.get("layout")
            };
            options = options || {};
            _.extend(this, options);
            _.extend(this, t_default);
            this.bindAll();
            this.bindInteractions();
        },

        showChildViews: function(){
            var self = this;
            console.info("LayoutView: child views ready!");
            if(!self.ready){
                self.ready = true;
                self.bindInteractions();
            }
        },

        onShow: function(){
            var self = this;
            $(document).ready(function(){
                console.info('LayoutView: document ready!');
                self.getLayoutParameters();
                Datacenter.start();
            });
        },

        bindAll: function(){
            this.listenTo(Datacenter, "DataCenter__initialized", this.showChildViews);
        },

        bindInteractions: function(){
            var self = this;
        },

        getLayoutParameters: function(){
            var self = this, t_ly = this.layout;
            var t_navTop = parseFloat($("#Navbar").css("height")) + parseFloat($("#Navbar").css("margin-bottom"));
            t_ly.globalHeight = innerHeight;
            t_ly.globalWidth = innerWidth;
            t_ly.globalTop = t_navTop;
            t_ly.globalMargin = t_ly.globalWidth * t_ly.marginRatio;
            //Left Top View
            var t_leftWidth = t_ly.globalWidth * t_ly.leftWidthRatio;
            var t_leftTopHeight = (t_ly.globalHeight - t_ly.globalTop) * t_ly.leftTopHeightRatio;
            t_ly.leftTop = {
                top: t_ly.globalTop,
                left: t_ly.globalMargin,
                width: t_leftWidth,
                height: t_leftTopHeight,
                };
            this.updateView("#leftTop",t_ly.leftTop);
            var t_leftMidHeight = (t_ly.globalHeight - t_ly.globalTop) * 0;
            //Left Bottom View
            var t_leftWidth = t_ly.globalWidth * t_ly.leftWidthRatio;
            var t_leftBtmHeight = (t_ly.globalHeight - t_ly.globalTop) * (1 - t_ly.leftMidHeightRatio) - t_leftMidHeight - t_leftTopHeight;
            t_ly.leftBtm = {
                top: t_ly.globalTop + t_leftTopHeight + t_leftMidHeight,
                left: t_ly.globalMargin,
                width: t_leftWidth,
                height: t_leftBtmHeight,
                };
            this.updateView("#leftBottom",t_ly.leftBtm);
            //Right Top View
            var t_rightWidth = t_ly.globalWidth * t_ly.rightWidthRatio;
            var t_rightTopHeight = (t_ly.globalHeight - t_ly.globalTop) * t_ly.rightTopHeightRatio;
            t_ly.rightTop = {
                top: t_ly.globalTop,
                left: t_leftWidth + t_ly.globalMargin * 2,
                width: t_rightWidth,
                height: t_rightTopHeight,
            };
            this.updateView("#rightTop",t_ly.rightTop);
            var t_rightMidHeight = (t_ly.globalHeight - t_ly.globalTop) * t_ly.rightMidHeightRatio;
            //Right Bottom View
            var t_rightWidth = t_ly.globalWidth * t_ly.rightWidthRatio;
            var t_rightBtmHeight = (t_ly.globalHeight - t_ly.globalTop) * t_ly.rightBtmHeightRatio;
            t_ly.rightBtm = {
                top: t_ly.globalTop + t_rightTopHeight + t_rightMidHeight,
                left: t_leftWidth + t_ly.globalMargin * 2,
                width: t_rightWidth,
                height: t_rightBtmHeight,
            };
            this.updateView("#rightBottom",t_ly.rightBtm);
            Config.set("layout",this.layout);
        },

        updateView: function(v_region, v_layout){
            $(v_region)
            .css("top",v_layout.top)
            .css("left",v_layout.left)
            .css("width",v_layout.width)
            .css("height",v_layout.height);
        },
    });
});
