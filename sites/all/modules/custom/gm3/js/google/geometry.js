google.maps.__gjsload__('geometry', function(_){var lx=function(a,b){return Math.abs(_.Jb(b-a,-180,180))},nx=function(a,b,c,d,e){if(!d){c=lx(a.lng(),c)/lx(a.lng(),b.lng());if(!e)return e=Math.sin(_.Cb(a.lat())),e=Math.log((1+e)/(1-e))/2,b=Math.sin(_.Cb(b.lat())),_.Db(2*Math.atan(Math.exp(e+c*(Math.log((1+b)/(1-b))/2-e)))-Math.PI/2);a=e.fromLatLngToPoint(a);b=e.fromLatLngToPoint(b);return e.fromPointToLatLng(new _.z(a.x+c*(b.x-a.x),a.y+c*(b.y-a.y))).lat()}e=_.Cb(a.lat());a=_.Cb(a.lng());d=_.Cb(b.lat());b=_.Cb(b.lng());c=_.Cb(c);return _.Jb(_.Db(Math.atan2(Math.sin(e)*
Math.cos(d)*Math.sin(c-b)-Math.sin(d)*Math.cos(e)*Math.sin(c-a),Math.cos(e)*Math.cos(d)*Math.sin(a-b))),-90,90)},ox=_.k(),px={containsLocation:function(a,b){var c=_.Jb(a.lng(),-180,180),d=!!b.get("geodesic"),e=b.get("latLngs"),f=b.get("map");f=!d&&f?f.getProjection():null;for(var g=!1,h=0,l=e.getLength();h<l;++h)for(var n=e.getAt(h),q=0,r=n.getLength();q<r;++q){var v=n.getAt(q),C=n.getAt((q+1)%r),A=_.Jb(v.lng(),-180,180),y=_.Jb(C.lng(),-180,180),B=Math.max(A,y);A=Math.min(A,y);(180<B-A?c>=B||c<A:
c<B&&c>=A)&&nx(v,C,c,d,f)<a.lat()&&(g=!g)}return g||px.isLocationOnEdge(a,b)},isLocationOnEdge:function(a,b,c){c=c||1E-9;var d=_.Jb(a.lng(),-180,180),e=b instanceof _.Lg,f=!!b.get("geodesic"),g=b.get("latLngs");b=b.get("map");b=!f&&b?b.getProjection():null;for(var h=0,l=g.getLength();h<l;++h)for(var n=g.getAt(h),q=n.getLength(),r=e?q:q-1,v=0;v<r;++v){var C=n.getAt(v),A=n.getAt((v+1)%q),y=_.Jb(C.lng(),-180,180),B=_.Jb(A.lng(),-180,180),E=Math.max(y,B),I=Math.min(y,B);if(y=1E-9>=Math.abs(_.Jb(y-B,-180,
180))&&(Math.abs(_.Jb(y-d,-180,180))<=c||Math.abs(_.Jb(B-d,-180,180))<=c)){y=a.lat();B=Math.min(C.lat(),A.lat())-c;var H=Math.max(C.lat(),A.lat())+c;y=y>=B&&y<=H}if(y)return!0;if(180<E-I?d+c>=E||d-c<=I:d+c>=I&&d-c<=E)if(C=nx(C,A,d,f,b),Math.abs(C-a.lat())<c)return!0}return!1}};var rx={computeHeading:function(a,b){var c=_.zc(a),d=_.Ac(a);a=_.zc(b);b=_.Ac(b)-d;return _.Jb(_.Db(Math.atan2(Math.sin(b)*Math.cos(a),Math.cos(c)*Math.sin(a)-Math.sin(c)*Math.cos(a)*Math.cos(b))),-180,180)},computeOffset:function(a,b,c,d){b/=d||6378137;c=_.Cb(c);var e=_.zc(a);a=_.Ac(a);d=Math.cos(b);b=Math.sin(b);var f=Math.sin(e);e=Math.cos(e);var g=d*f+b*e*Math.cos(c);return new _.F(_.Db(Math.asin(g)),_.Db(a+Math.atan2(b*e*Math.sin(c),d-f*g)))},computeOffsetOrigin:function(a,b,c,d){c=_.Cb(c);b/=
d||6378137;d=Math.cos(b);var e=Math.sin(b)*Math.cos(c);b=Math.sin(b)*Math.sin(c);c=Math.sin(_.zc(a));var f=e*e*d*d+d*d*d*d-d*d*c*c;if(0>f)return null;var g=e*c+Math.sqrt(f);g/=d*d+e*e;var h=(c-e*g)/d;g=Math.atan2(h,g);if(g<-Math.PI/2||g>Math.PI/2)g=e*c-Math.sqrt(f),g=Math.atan2(h,g/(d*d+e*e));if(g<-Math.PI/2||g>Math.PI/2)return null;a=_.Ac(a)-Math.atan2(b,d*Math.cos(g)-e*Math.sin(g));return new _.F(_.Db(g),_.Db(a))},interpolate:function(a,b,c){var d=_.zc(a),e=_.Ac(a),f=_.zc(b),g=_.Ac(b),h=Math.cos(d),
l=Math.cos(f);b=rx.Pe(a,b);var n=Math.sin(b);if(1E-6>n)return new _.F(a.lat(),a.lng());a=Math.sin((1-c)*b)/n;c=Math.sin(c*b)/n;b=a*h*Math.cos(e)+c*l*Math.cos(g);e=a*h*Math.sin(e)+c*l*Math.sin(g);return new _.F(_.Db(Math.atan2(a*Math.sin(d)+c*Math.sin(f),Math.sqrt(b*b+e*e))),_.Db(Math.atan2(e,b)))},Pe:function(a,b){var c=_.zc(a);a=_.Ac(a);var d=_.zc(b);b=_.Ac(b);return 2*Math.asin(Math.sqrt(Math.pow(Math.sin((c-d)/2),2)+Math.cos(c)*Math.cos(d)*Math.pow(Math.sin((a-b)/2),2)))},computeDistanceBetween:function(a,
b,c){c=c||6378137;return rx.Pe(a,b)*c},computeLength:function(a,b){b=b||6378137;var c=0;a instanceof _.gd&&(a=a.getArray());for(var d=0,e=a.length-1;d<e;++d)c+=rx.computeDistanceBetween(a[d],a[d+1],b);return c},computeArea:function(a,b){return Math.abs(rx.computeSignedArea(a,b))},computeSignedArea:function(a,b){b=b||6378137;a instanceof _.gd&&(a=a.getArray());for(var c=a[0],d=0,e=1,f=a.length-1;e<f;++e)d+=rx.Nk(c,a[e],a[e+1]);return d*b*b},Nk:function(a,b,c){return rx.Ok(a,b,c)*rx.Rl(a,b,c)},Ok:function(a,
b,c){var d=[a,b,c,a];a=[];for(c=b=0;3>c;++c)a[c]=rx.Pe(d[c],d[c+1]),b+=a[c];b/=2;d=Math.tan(b/2);for(c=0;3>c;++c)d*=Math.tan((b-a[c])/2);return 4*Math.atan(Math.sqrt(Math.abs(d)))},Rl:function(a,b,c){a=[a,b,c];b=[];for(c=0;3>c;++c){var d=a[c],e=_.zc(d);d=_.Ac(d);var f=b[c]=[];f[0]=Math.cos(e)*Math.cos(d);f[1]=Math.cos(e)*Math.sin(d);f[2]=Math.sin(e)}return 0<b[0][0]*b[1][1]*b[2][2]+b[1][0]*b[2][1]*b[0][2]+b[2][0]*b[0][1]*b[1][2]-b[0][0]*b[2][1]*b[1][2]-b[1][0]*b[0][1]*b[2][2]-b[2][0]*b[1][1]*b[0][2]?
1:-1}};var sx={decodePath:function(a){for(var b=_.Eb(a),c=Array(Math.floor(a.length/2)),d=0,e=0,f=0,g=0;d<b;++g){var h=1,l=0;do{var n=a.charCodeAt(d++)-63-1;h+=n<<l;l+=5}while(31<=n);e+=h&1?~(h>>1):h>>1;h=1;l=0;do n=a.charCodeAt(d++)-63-1,h+=n<<l,l+=5;while(31<=n);f+=h&1?~(h>>1):h>>1;c[g]=new _.F(1E-5*e,1E-5*f,!0)}c.length=g;return c},encodePath:function(a){a instanceof _.gd&&(a=a.getArray());return sx.kn(a,function(a){return[Math.round(1E5*a.lat()),Math.round(1E5*a.lng())]})},kn:function(a,b){for(var c=
[],d=[0,0],e,f=0,g=_.Eb(a);f<g;++f)e=b?b(a[f]):a[f],sx.Oh(e[0]-d[0],c),sx.Oh(e[1]-d[1],c),d=e;return c.join("")},Oh:function(a,b){return sx.ln(0>a?~(a<<1):a<<1,b)},ln:function(a,b){for(;32<=a;)b.push(String.fromCharCode((32|a&31)+63)),a>>=5;b.push(String.fromCharCode(a+63));return b}};_.ob.google.maps.geometry={encoding:sx,spherical:rx,poly:px};_.m=ox.prototype;_.m.decodePath=sx.decodePath;_.m.encodePath=sx.encodePath;_.m.computeDistanceBetween=rx.computeDistanceBetween;_.m.interpolate=rx.interpolate;_.m.computeHeading=rx.computeHeading;_.m.computeOffset=rx.computeOffset;_.m.computeOffsetOrigin=rx.computeOffsetOrigin;_.je("geometry",new ox);});

/*
     FILE ARCHIVED ON 10:26:53 Feb 08, 2018 AND RETRIEVED FROM THE
     INTERNET ARCHIVE ON 08:45:01 Oct 02, 2018.
     JAVASCRIPT APPENDED BY WAYBACK MACHINE, COPYRIGHT INTERNET ARCHIVE.

     ALL OTHER CONTENT MAY ALSO BE PROTECTED BY COPYRIGHT (17 U.S.C.
     SECTION 108(a)(3)).
*/
/*
playback timings (ms):
  LoadShardBlock: 90.773 (3)
  esindex: 0.005
  captures_list: 112.568
  CDXLines.iter: 15.501 (3)
  PetaboxLoader3.datanode: 144.99 (4)
  exclusion.robots: 0.161
  exclusion.robots.policy: 0.152
  RedisCDXSource: 0.499
  PetaboxLoader3.resolve: 41.777
  load_resource: 143.671
*/
