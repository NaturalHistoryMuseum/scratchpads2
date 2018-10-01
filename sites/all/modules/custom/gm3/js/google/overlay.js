google.maps.__gjsload__('overlay', function(_){var Fz=_.oa("b"),Gz=_.k(),Hz=function(){var a=this.Bf;if(this.getPanes()){if(this.getProjection()){if(!a.b&&this.onAdd)this.onAdd();a.b=!0;this.draw()}}else{if(a.b)if(this.onRemove)this.onRemove();else this.remove();a.b=!1}},Iz=function(a){a.Bf=a.Bf||new Gz;return a.Bf},Jz=function(a){_.Sf.call(this);this.da=(0,_.t)(Hz,a)};_.u(Fz,_.K);
Fz.prototype.changed=function(a){"outProjection"!=a&&(a=!!(this.get("offset")&&this.get("projectionTopLeft")&&this.get("projection")&&_.x(this.get("zoom"))),a==!this.get("outProjection")&&this.set("outProjection",a?this.b:null))};_.u(Jz,_.Sf);_.je("overlay",{sk:function(a){if(a){a.getMap();var b=a.getMap(),c=Iz(a),d=c.xm;c.xm=b;d&&(c=Iz(a),(d=c.aa)&&d.unbindAll(),(d=c.Qh)&&d.unbindAll(),a.unbindAll(),a.set("panes",null),a.set("projection",null),_.w(c.P,_.G.removeListener),c.P=null,c.me&&(c.me.da(),c.me=null),_.sn("Ox","-p",a));if(b){c=Iz(a);d=c.me;d||(d=c.me=new Jz(a));_.w(c.P||[],_.G.removeListener);var e=c.aa=c.aa||new _.Yk,f=b.__gm;e.bindTo("zoom",f);e.bindTo("offset",f);e.bindTo("center",f,"projectionCenterQ");e.bindTo("projection",
b);e.bindTo("projectionTopLeft",f);e=c.Qh=c.Qh||new Fz(e);e.bindTo("zoom",f);e.bindTo("offset",f);e.bindTo("projection",b);e.bindTo("projectionTopLeft",f);a.bindTo("projection",e,"outProjection");a.bindTo("panes",f);e=(0,_.t)(d.O,d);c.P=[_.G.addListener(a,"panes_changed",e),_.G.addListener(f,"zoom_changed",e),_.G.addListener(f,"offset_changed",e),_.G.addListener(b,"projection_changed",e),_.G.addListener(f,"projectioncenterq_changed",e),_.G.forward(b,"forceredraw",d)];d.O();b instanceof _.Ud&&(_.pn(b,
"Ox"),_.rn("Ox","-p",a,!!b.Y))}}}});});

/*
     FILE ARCHIVED ON 05:17:57 Jan 01, 2018 AND RETRIEVED FROM THE
     INTERNET ARCHIVE ON 11:17:35 Oct 01, 2018.
     JAVASCRIPT APPENDED BY WAYBACK MACHINE, COPYRIGHT INTERNET ARCHIVE.

     ALL OTHER CONTENT MAY ALSO BE PROTECTED BY COPYRIGHT (17 U.S.C.
     SECTION 108(a)(3)).
*/
/*
playback timings (ms):
  LoadShardBlock: 693.294 (3)
  esindex: 0.006
  captures_list: 743.381
  CDXLines.iter: 12.84 (3)
  PetaboxLoader3.datanode: 92.308 (5)
  exclusion.robots: 0.113
  exclusion.robots.policy: 0.105
  RedisCDXSource: 0.358
  PetaboxLoader3.resolve: 82.878 (2)
  load_resource: 154.738
*/
