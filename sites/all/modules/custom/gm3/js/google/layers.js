google.maps.__gjsload__('layers', function(_){var rU=function(a,b,c){_.vn.call(this);this.l=null!=c?(0,_.t)(a,c):a;this.j=b;this.f=(0,_.t)(this.zl,this);this.b=[]},sU=function(a){a.ec=_.eo(a.f,a.j);a.l.apply(null,a.b)},tU=function(a,b,c,d){this.B=a;this.F=b;this.D=c;this.f=this.b=null;this.j=this.l=d;this.m=new _.fo(this.C,0,this)},vU=function(a,b,c,d){this.B=d;this.H=null;this.D=!1;this.l=!0;this.F=Math.floor(_.Ta()/uU());this.b=null;this.m=new tU(a,b,c,this.Rd());this.C=!0;this.f=new rU(this.G,uU(),this);this.f.ph();this.j()},wU=function(a){var b=
  a.m;a=a.Rd();b.j=a;_.go(b.m)},uU=function(){return window.__gm_trafficAutoRefreshPeriod||6E5},xU=_.k(),BU=function(a,b,c,d){var e=new _.su;e.ma=b;b=new _.tk;b.data[0]=68;var f=_.uk(b);_.sk(f,"set");f.data[1]=d;e.Zh=b;var g=new tU(yU(c,a),zU,AU(c,a),e);_.mk(a,"map_changed",function(){g.setMap(a.getMap())})},yU=function(a,b){return function(c,d){var e=c.__gm.b;e.set(_.yj(e.get(),d));_.pn(c,a);_.rn(a,"-p",b,!!c.Y)}},zU=function(a,b,c){a=a.__gm.b;a.set(_.yj(a.get().gb(b),c))},AU=function(a,b){return function(c,
  d){c=c.__gm.b;c.set(c.get().gb(d));_.sn(a,"-p",b)}};_.u(rU,_.vn);_.m=rU.prototype;_.m.Ec=!1;_.m.bd=0;_.m.ec=null;_.m.ph=function(a){this.b=arguments;this.ec||this.bd?this.Ec=!0:sU(this)};_.m.stop=function(){this.ec&&(_.ob.clearTimeout(this.ec),this.ec=null,this.Ec=!1,this.b=[])};_.m.Ha=function(){rU.ob.Ha.call(this);this.stop()};_.m.zl=function(){this.ec=null;this.Ec&&!this.bd&&(this.Ec=!1,sU(this))};tU.prototype.setMap=function(a){this.f=a;_.go(this.m)};tU.prototype.C=function(){this.f==this.b?this.b&&this.F(this.b,this.l,this.j):(this.b&&this.D(this.b,this.l),this.f&&this.B(this.f,this.j));this.b=this.f;this.l=this.j};vU.prototype.setMap=function(a){this.m.setMap(a);this.H=a;this.j();a?this.b||(this.b=_.G.X(this.B,"visibilitychange",this,this.j)):this.b&&(_.G.removeListener(this.b),this.b=null)};vU.prototype.j=function(){var a=this.l&&!!this.H&&!this.B.hidden;if(a!=this.C){if(a){var b=this.f;b.bd--;b.bd||!b.Ec||b.ec||(b.Ec=!1,sU(b))}else this.f.bd++;this.C=a}};vU.prototype.G=function(){this.F=Math.floor(_.Ta()/uU());wU(this);this.f.ph()};
  vU.prototype.Rd=function(){var a=new _.su;a.ma="traffic";this.D&&(a.parameters.incidents="1",a.parameters.incidents_text="1");a.parameters.t=this.F;return a};xU.prototype.f=function(a){var b=new vU(yU("Lt",a),zU,AU("Lt",a),window.document);_.mk(a,"map_changed",function(){b.setMap(a.getMap())});_.mk(a,"showincidents_changed",function(){var c=a.get("showIncidents");b.D=c;wU(b)});_.mk(a,"autorefresh_changed",function(){var c=0!=a.get("autoRefresh");b.l=c;b.j()})};xU.prototype.b=function(a){BU(a,"bike","Lb","NonRoadmap")};xU.prototype.j=function(a){BU(a,"transit:comp","Lr","TransitFocused")};_.je("layers",new xU);});

  /*
       FILE ARCHIVED ON 10:06:39 Feb 07, 2018 AND RETRIEVED FROM THE
       INTERNET ARCHIVE ON 08:47:03 Oct 02, 2018.
       JAVASCRIPT APPENDED BY WAYBACK MACHINE, COPYRIGHT INTERNET ARCHIVE.

       ALL OTHER CONTENT MAY ALSO BE PROTECTED BY COPYRIGHT (17 U.S.C.
       SECTION 108(a)(3)).
  */
  /*
  playback timings (ms):
    LoadShardBlock: 56.594 (3)
    esindex: 0.006
    captures_list: 70.231
    CDXLines.iter: 9.755 (3)
    PetaboxLoader3.datanode: 113.934 (5)
    exclusion.robots: 0.136
    exclusion.robots.policy: 0.126
    RedisCDXSource: 0.672
    PetaboxLoader3.resolve: 47.11 (2)
    load_resource: 111.133
  */
