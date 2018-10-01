google.maps.__gjsload__('stats', function(_){var j0=function(){this.b=new _.cl},k0=function(a){var b=0,c=0;a.b.forEach(function(a){b+=a.Io;c+=a.fo});return c?b/c:0},l0=function(a,b,c){var d=[];_.fb(a,function(a,c){d.push(c+b+a)});return d.join(c)},m0=function(a){var b={};_.fb(a,function(a,d){b[(0,window.encodeURIComponent)(d)]=(0,window.encodeURIComponent)(a).replace(/%7C/g,"|")});return l0(b,":",",")},n0=function(a,b,c){this.l=b;this.f=a+"/maps/gen_204";this.j=c},o0=function(a,b,c,d){var e={};e.host=window.document.location&&window.document.location.host||
window.location.host;e.v=a;e.r=Math.round(1/b);c&&(e.client=c);d&&(e.key=d);return e},p0=function(a,b,c,d,e){this.m=a;this.C=b;this.l=c;this.f=d;this.j=e;this.b=new _.cl;this.B=_.Ta()},r0=function(a,b,c,d,e){var f=_.N(_.R,23,500);var g=_.N(_.R,22,2);this.D=a;this.F=b;this.G=f;this.l=g;this.C=c;this.m=d;this.B=e;this.f=new _.cl;this.b=0;this.j=_.Ta();q0(this)},q0=function(a){window.setTimeout(function(){s0(a);q0(a)},Math.min(a.G*Math.pow(a.l,a.b),2147483647))},t0=function(a,b,c){a.f.set(b,c)},s0=function(a){var b=
o0(a.F,a.C,a.m,a.B);b.t=a.b+"-"+(_.Ta()-a.j);a.f.forEach(function(a,d){a=a();0<a&&(b[d]=Number(a.toFixed(2))+(_.Dm()?"-if":""))});a.D.b({ev:"api_snap"},b);++a.b},u0=function(a,b,c,d,e){this.B=a;this.C=b;this.m=c;this.j=d;this.l=e;this.f={};this.b=[]},v0=function(a,b,c,d){this.j=a;_.G.bind(this.j,"set_at",this,this.l);_.G.bind(this.j,"insert_at",this,this.l);this.B=b;this.C=c;this.m=d;this.f=0;this.b={};this.l()},w0=function(){this.j=_.O(_.R,6);this.C=_.yf();this.b=new n0(_.wg[35]?_.O(_.zf(_.R),11):
_.tw,_.lj,window.document);new v0(_.Zi,(0,_.t)(this.b.b,this.b),_.Ef,!!this.j);var a=_.O(new _.sf(_.R.data[3]),1);this.D=a.split(".")[1]||a;this.F={};this.B={};this.m={};this.G={};this.ea=_.N(_.R,0,1);_.kj&&(this.l=new r0(this.b,this.D,this.ea,this.j,this.C))};j0.prototype.f=function(a,b,c){this.b.set(_.Xc(a),{Io:b,fo:c})};
n0.prototype.b=function(a,b){b=b||{};var c=_.ik().toString(36);b.src="apiv3";b.token=this.l;b.ts=c.substr(c.length-6);a.cad=m0(b);a=l0(a,"=","&");a=this.f+"?target=api&"+a;this.j.createElement("img").src=a;(b=_.ob.__gm_captureCSI)&&b(a)};
p0.prototype.D=function(a,b){b=_.p(b)?b:1;this.b.isEmpty()&&window.setTimeout((0,_.t)(function(){var a=o0(this.C,this.l,this.f,this.j);a.t=_.Ta()-this.B;var b=this.b;_.Cl(b);for(var e={},f=0;f<b.b.length;f++){var g=b.b[f];e[g]=b.H[g]}_.Sz(a,e);this.b.clear();this.m.b({ev:"api_maprft"},a)},this),500);b=this.b.get(a,0)+b;this.b.set(a,b)};u0.prototype.D=function(a){this.f[a]||(this.f[a]=!0,this.b.push(a),2>this.b.length&&_.oA(this,this.F,500))};
u0.prototype.F=function(){for(var a=o0(this.C,this.m,this.j,this.l),b=0,c;c=this.b[b];++b)a[c]="1";a.hybrid=+_.hm();this.b.length=0;this.B.b({ev:"api_mapft"},a)};v0.prototype.l=function(){for(var a;a=this.j.removeAt(0);){var b=a.Jn;a=a.timestamp-this.C;++this.f;this.b[b]||(this.b[b]=0);++this.b[b];if(20<=this.f&&!(this.f%5)){var c={};c.s=b;c.sr=this.b[b];c.tr=this.f;c.te=a;c.hc=this.m?"1":"0";this.B({ev:"api_services"},c)}}};w0.prototype.V=function(a){a=_.Xc(a);this.F[a]||(this.F[a]=new u0(this.b,this.D,this.ea,this.j,this.C));return this.F[a]};w0.prototype.S=function(a){a=_.Xc(a);this.B[a]||(this.B[a]=new p0(this.b,this.D,_.N(_.R,0,1),this.j,this.C));return this.B[a]};w0.prototype.f=function(a){if(this.l){this.m[a]||(this.m[a]=new _.gB,t0(this.l,a,function(){return b.Za()}));var b=this.m[a];return b}};
w0.prototype.N=function(a){if(this.l){this.G[a]||(this.G[a]=new j0,t0(this.l,a,function(){return k0(b)}));var b=this.G[a];return b}};var x0=new w0;_.je("stats",x0);});

/*
     FILE ARCHIVED ON 05:18:01 Jan 01, 2018 AND RETRIEVED FROM THE
     INTERNET ARCHIVE ON 11:17:39 Oct 01, 2018.
     JAVASCRIPT APPENDED BY WAYBACK MACHINE, COPYRIGHT INTERNET ARCHIVE.

     ALL OTHER CONTENT MAY ALSO BE PROTECTED BY COPYRIGHT (17 U.S.C.
     SECTION 108(a)(3)).
*/
/*
playback timings (ms):
  LoadShardBlock: 168.636 (3)
  esindex: 0.009
  captures_list: 339.093
  CDXLines.iter: 31.383 (3)
  PetaboxLoader3.datanode: 177.877 (5)
  exclusion.robots: 0.2
  exclusion.robots.policy: 0.187
  RedisCDXSource: 0.571
  PetaboxLoader3.resolve: 119.665 (2)
  load_resource: 194.278
*/
