describe('object-foreach', function () {
    it('executes callback on each property', function () {
        var x = {
            a : 'v',
            b : 'v',
            c : 'v',
            d : 'v'
        };
        objectForeach(x, function (val, prop, obj) {
            obj[prop] = 'y';
        });
        expect(x).toEqual({
            a : 'y',
            b : 'y',
            c : 'y',
            d : 'y'
        });
    });
});



