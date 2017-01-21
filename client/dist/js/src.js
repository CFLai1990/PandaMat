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

require.config({
    shim: {
         'bootstrap': ['jquery'],
         'backbone': {
            deps: ['jquery','underscore'],
        },
          'MDS': ['numeric'],
          'basicFunctions': ['numeric', 'MDS'],
          'data': ['MDS'],
          'datacenter': ['MDS'],
          'SubMap_Model': ['basicFunctions'],
          'Switch': ['jquery', 'bootstrap'],
          'AppView': ['Switch'],
          'tooltip': ['jquery', 'bootstrap'],
     },
    paths: {
        // libs loader
        'text': '../bower_components/text/text',
        'jquery': ['../bower_components/jquery/dist/jquery.min'],
        'underscore': ['../bower_components/underscore/underscore-min'],
        'bootstrap': ['../bower_components/bootstrap/dist/js/bootstrap.min'],
        'backbone': ['../bower_components/backbone/backbone-min'],
        'marionette': ['../bower_components/backbone.marionette/lib/backbone.marionette.min'],
        'backbone.relational': ['../bower_components/backbone-relational/backbone-relational'],
        'colorjs': ['../bower_components/jscolor/jscolor'],
        'd3': ['../bower_components/d3/d3.min'],
        'numeric': ['../bower_components/numericjs/src/numeric'],
        'hull': ['../bower_components/hull-js/dist/hull'],
        'PandaMat': ['../bower_components/pandamat/pandamat'],

        'Switch': 'libs/bootstrapSwitch',
        'tooltip': 'libs/tooltip',
        'MDS': 'libs/mds',
        'basicFunctions': 'libs/basicFunctions',
        'backbone.routefilter': '../bower_components/backbone.routefilter/dist/backbone.routefilter.min',
        // templates path
        'templates': '../templates',
        //controls
        'Router': 'controls/router',
        'Controller': 'controls/controller',
        //models
        'datacenter': 'models/datacenter.model',
        'config': 'models/config.model',
        'data': 'models/data.model',
        //collections
        //views
        'Base': 'views/base.view',
        'AppView': 'views/app.view',
    }
});

require(['app'], function (App) {
    'use strict';
    var app = new App();
    app.start();
});
