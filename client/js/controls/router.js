define([
    'marionette',
], function(Mn) {
    'use strict';

    var router = Mn.AppRouter.extend({
        appRoutes: {

            // default route
            "*default": "showApp"

        },

        before: function (route, params) {
            // window.NProgress.start();
        },

        after: function (route, params) {
            // window.NProgress.done();
        }
    });

    return router;
});
