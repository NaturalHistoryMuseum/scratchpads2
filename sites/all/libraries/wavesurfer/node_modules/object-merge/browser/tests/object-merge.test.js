/*
License gpl-3.0 http://www.gnu.org/licenses/gpl-3.0-standalone.html
*/
describe('createOptions', function () {
    it('creates an options object with defaults for unspecified values',
        function () {
            var opts = objectMerge.createOptions();
            expect(opts).toEqual({
                depth : false,
                throwOnCircularRef : true
            });
        }
    );
    it('creates an options object with user specified values',
        function () {
            var opts = objectMerge.createOptions({
                depth : 9,
                throwOnCircularRef : false
            });
            expect(opts).toEqual({
                depth : 9,
                throwOnCircularRef : false
            });
        }
    );
});
describe('object-merge', function () {
    var w = {
        a : [
            {b : 'b'},
            {c : 'c'}
        ]
    };
    var x = {
        a : 'a',
        b : 'b',
        c : {
            d : 'd',
            e : 'e',
            f : {
                g : 'g'
            }
        }
    };
    var y = {
        a : '`a',
        b : '`b',
        c : {
            d : '`d'
        }
    };
    var z = {
        a : {
            b : '``b'
        },
        fun : function foo () {
            return 'foo';
        },
        aps : Array.prototype.slice
    };
    
    it('clones non native functions', function () {
        var out = objectMerge(x, y, z);
        expect(out.fun()).toEqual(z.fun());
        expect(z.fun).not.toEqual(out.fun);
    });
    it('clones function properties', function () {
        var func = function () {
            return null;
        };
        var func2 = function () {
            return 'hello';
        };
        func.wohoo = 'wohoo';
        func.obj = {a:'a'};
        func2.wee = 'wee';
        func2.obj = {b:'b'};
        func2.obj2 = {a:'a'};
        var out = objectMerge(func, func2);
        expect(out.wohoo).toEqual('wohoo');
        expect(out.wee).toEqual('wee');
        expect(out.obj.a).toEqual('a');
        expect(out.obj.b).toEqual('b');
        expect(out.obj2.a).toEqual('a');
        expect(out.obj === func.obj).toEqual(false);
        expect(out.obj2 === func2.obj2).toEqual(false);
        expect(out() === 'hello').toEqual(true);
    });
    it('references native functions', function () {
        var out = objectMerge(x, y, z);
        expect(out.aps(['a'],0)).toEqual(z.aps(['a'],0))
        expect(out.aps).toEqual(z.aps);
    });
    it('preserves array indexes', function () {
        var arr = [];
        arr[0] = '0';
        arr[5] = '5';
        var obj = {
            '0' : '1',
            '1' : '1'
        };
        var out = objectMerge(obj, arr);
        expect(arr.length).toEqual(6);
        expect(out.length).toEqual(6);
        expect(arr[0]).toEqual(out[0]);
        expect(obj[1]).toEqual(out[1]);
        expect(out[5]).toEqual(arr[5]);
        expect(out instanceof Array).toEqual(true);
    });
    it('clones array properties', function () {
        var arr = [];
        arr[0] = '0';
        var obj = {
            'wee' : 'wee',
            'wohoo' : 'wohoo',
            'obj' : {}
        };
        var out = objectMerge(obj, arr);
        expect(arr.length).toEqual(out.length);
        expect(arr[0]).toEqual(out[0]);
        expect(out.wee).toEqual(obj.wee);
        expect(arr.wee === undefined).toEqual(true);
        expect(out.wohoo).toEqual(obj.wohoo);
        expect(out instanceof Array).toEqual(true);
        expect(out.obj).toEqual({});
        expect(out.obj === obj.obj).toEqual(false);
    });
    it('merges arrays', function () {
        var arr1 = ['0','1','2'];
        var arr2 = [];
        arr2[2] = '`2';
        arr2[3] = '`3';
        var out = objectMerge(arr1, arr2);
        expect(arr1.length).toEqual(3);
        expect(arr2.length).toEqual(4);
        expect(out.length).toEqual(4);
        expect(arr1).toEqual(['0','1','2']);
        expect(arr2).toEqual([undefined, undefined, '`2','`3']);
        expect(out).toEqual(['0','1','`2','`3']);
    });
    it('clones array contents', function () {
        var out = objectMerge(x, w);
        expect(out.a instanceof Array).toEqual(true);
        expect(out.a === w.a).toEqual(false);
        expect(out.a[0] === w.a[0]).toEqual(false);
        expect(out.a[1] === w.a[1]).toEqual(false);
        expect(out.a[0].b === w.a[0].b).toEqual(true);
        expect(out.a[0].c === w.a[0].c).toEqual(true);
        expect(out.a[1].b === w.a[1].b).toEqual(true);
        expect(out.a[1].c === w.a[1].c).toEqual(true);
    });
    it('merges all objects recursively', function () {
        var out = objectMerge(x, y, z);
        expect(out.a === z.a).toEqual(false);
        expect(out.a).toEqual({'b' : '``b'});
        expect(out.b).toEqual('`b');
        expect(out.c).toEqual({
            'd' : '`d',
            'e' : 'e',
            'f' : {
                'g' : 'g'
            }
        });
        expect(out.fun).not.toEqual(z.fun);
        expect(out.fun()).toEqual(z.fun());
        expect(out.aps).toEqual(z.aps);
        out.c = {a:'wee'};
        expect(out.c.a == 'wee').toEqual(true);
        expect(x.c.a == 'wee').toEqual(false);
        expect(y.c.a == 'wee').toEqual(false);
    });
    it('throws an error on circular references to functions', function () {
        var x = {
            'a' : function () {return null},
        };
        x.b = x.a;
        function thrower () {
            return objectMerge(x);
        }
        expect(thrower).toThrow();
    });
    it('throws an error on circular references to arrays', function () {
        var x = {
            'a' : [],
        };
        x.b = x.a;
        function thrower () {
            return objectMerge(x);
        }
        expect(thrower).toThrow();
    });
    it('throws an error on circular references to objects', function () {
        var x = {
            'a' : {}
        };
        x.b = x.a;
        function thrower () {
            return objectMerge(x);
        }
        expect(thrower).toThrow();
    });
    it('throws an error on circular references to nested objects', function () {
        var x = {
            'a' : {}
        };
        x.b = {};
        x.b.c = x.a;
        function thrower () {
            return objectMerge(x);
        }
        expect(thrower).toThrow();
    });
    it('throws an error on circular references to string objects', function () {
        var x = {
            'a' : new String(),
        };
        x.b = x.a;
        function thrower () {
            return objectMerge(x);
        }
        expect(thrower).toThrow();
    });
    it('throws an error on circular references to number objects', function () {
        var x = {
            'a' : new Number(),
        };
        x.b = x.a;
        function thrower () {
            return objectMerge(x);
        }
        expect(thrower).toThrow();
    });
    it('does not throw an error on circular references to scalar string',
        function () {
            var x = {
                'a' : 'x',
            };
            x.b = x.a;
            function thrower () {
                return objectMerge(x);
            }
            expect(thrower).not.toThrow();
        }
    );
    it('does not throw an error on circular references to scalar number',
        function () {
            var x = {
                'a' : 5,
            };
            x.b = x.a;
            function thrower () {
                return objectMerge(x);
            }
            expect(thrower).not.toThrow();
        }
    );
    it('allows circular reference check to be disabled', function () {
        var x = {
            'a' : {}
        };
        x.b = x.a;
        function thrower () {
            var opts = objectMerge.createOptions({throwOnCircularRef : false});
            return objectMerge(opts, x);
        }
        expect(thrower).not.toThrow();
    });
    it('only considers ObjectMergeOptions obj @ arg[0] to be valid options',
        function () {
            var x = {
                'a' : {}
            };
            x.b = x.a;
            function thrower () {
                var opts = objectMerge.createOptions({throwOnCircularRef : false});
                return objectMerge(x, opts);
            }
            function thrower2 () {
                return objectMerge({throwOnCircularRef : false}, x);
            }
            expect(thrower).toThrow();
            expect(thrower2).toThrow();
        }
    );
    it('considers depth of false to mean no limit', function () {
        var a = {
            'a1' : {
                'a2' : {
                    'a3' : {}
                }
            }
        };
        var opts = objectMerge.createOptions({depth : false});
        var res = objectMerge(opts, a);
        expect(res).toEqual(a);
    });
    it('allows specifying depth of traversal', function () {
        var a = {
            'a1' : {
                'a2' : {
                    'a3' : {}
                }
            }
        };
        var b = {
            'b1' : {
                'b2' : {
                    'b3' : {}
                }
            }
        };
        var opts = objectMerge.createOptions({depth : 2});
        var res = objectMerge(opts, a, b);
        expect(res).toEqual({
            'a1' : {
                'a2' : {}
            },
            'b1' : {
                'b2' : {}
            }
        });
    });
});
