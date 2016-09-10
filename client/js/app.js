define([
    'require',
    'marionette',
    'backbone',
    'underscore',
    'jquery',
    'bootstrap',
    "AppView",
    "Controller",
    "Router"
], function (require, Mn, Backbone, _, $, Bootstrap, AppView, Controller, Router) {
    'use strict';

    var testing = false;
    var RootView = Mn.LayoutView.extend({
        el: 'body',
        regions: {
            'app': '#rootApp'
        }
    });

    var App = Mn.Application.extend({

        onStart: function() {
            this.appRoot = new RootView();
            window.router = this.router = new Router({
                controller: new Controller({appRoot: this.appRoot})
            });
            if (Backbone.history) {
                Backbone.history.start({
                    root: '/',
                    pushState: true
                });
            }
        },
    });
    return App;
});
