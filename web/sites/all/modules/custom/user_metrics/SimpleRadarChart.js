/*      SimpleRadarChart v0.1 <http://code.google.com/p/simple-radar-chart/>
        is released under the MIT License <http://www.opensource.org/licenses/mit-license.php>
*/

/*
 * @author tonylua@sina.com
 * @descibe a simple javascript radar chart
 * @sample
        var radar = new SimpleRadarChart({
                containerId: 'myContainer',
                width: 500,
                height: 500,
                radius: 200,
                nameColor: '#999',
                lineColor: '#ff3399',
                lineCap: 'round',
                lineWidth: 2,
                backLineColor: '#eee',
                backLineWidth: 1,
                vertexRadius: 4,
                maxValue: 200,
                referCount: 4,
                descArr: [
                        '$0 描述文字',
                        '描$0 述文字',
                        '$0 描述文字',
                        '描述 $0 文字',
                        '$0 描述文字',
                        '$0 描述文字'
                ],
                nameArr: [
                        '文学',
                        '艺术',
                        '经济学',
                        '人文科技',
                        '体育',
                        '卫生和人权'
                ],
                valueArr: [140, 120, 190, 135, 150, 125]
        });
        radar.render();

 */

var SimpleRadarChart = (function(document, Math){
        var Point = function(x,y){
                        this.x = x||0;
                        this.y = y||0;
                },
                degreesToRadians = function(degrees) {
                        return degrees * Math.PI / 180;
                },
                rotatePoint = function(fromPoint, centerPoint, radians, clockwise) {
                        if (typeof clockwise == 'undefined') clockwise = true;
                        var p = new Point;
                        var x1 = fromPoint.x - centerPoint.x;
                        var y1 = fromPoint.y - centerPoint.y;  
                        if (clockwise)
                        {
                                p.x = Math.cos(radians) * x1 - Math.sin(radians) * y1;
                                p.y = Math.cos(radians) * y1 + Math.sin(radians) * x1;
                        }
                        else
                        {
                                p.x = Math.cos(radians) * x1 + Math.sin(radians) * y1;
                                p.y = Math.cos(radians) * y1 - Math.sin(radians) * x1;
                        }      
                        p.x += centerPoint.x;
                        p.y += centerPoint.y;  
                        return p;
                },
                NEED_VML = !document.createElement('canvas').getContext,
                Const;
       
        Const = function(config){
                this._cfg = config;
        };
        Const.prototype = {
                render: function(){
                        this._$ctn = document.getElementById(this._cfg.containerId);
                        this._$inn = document.createElement('div');
                        this._$inn.style.position = 'relative';
                        this._$ctn.appendChild(this._$inn);
                        this._$inn.style.width = this._cfg.width+'px';
                        this._$inn.style.height = this._cfg.height+'px';
                        this._$cvs = document.createElement('canvas');
                        if(window.G_vmlCanvasManager){
                                window.G_vmlCanvasManager.initElement(this._$cvs);
                        }
                        this._$cvs.id = this._cfg.containerId + '_cvs';
                        this._$cvs.style.position = 'absolute';
                        this._$cvs.style.zIndex = 0;
                        this._$inn.appendChild(this._$cvs);                    
                        this._$cvs.width = this._cfg.width;
                        this._$cvs.height = this._cfg.height;
                        this._$ctx = this._$cvs.getContext('2d');
                        this._c = new Point(.5*this._cfg.width, .5*this._cfg.height); //圆心
                        this._r = this._cfg.radius; //半径
                        this._n = this._cfg.valueArr.length;
                        this._a = 360/this._n; //两个顶点之间的角度
                        this._pArr = []; //顶点数组
                        this._vArr = []; //值点数组
                       
                        this._pArr[0] = new Point(this._c.x, this._c.y-this._r);
                        for (var i=1;i<this._n;i++) {
                                this._pArr[i] = rotatePoint(this._pArr[i-1], this._c, degreesToRadians(this._a));
                        }

                        for (var i=0;i<this._n;i++) {
                                var to = i<this._n-1 ? i+1 : 0;
                                this._drawBackLine(this._$ctx, this._pArr[i], this._pArr[to]); //外围框线
                                this._drawBackLine(this._$ctx, this._pArr[i], this._c); //到中心的线
                               
                                var d1 = this._r/this._cfg.referCount; //内层参考线
                                for (var j=1;j<this._cfg.referCount;j++) {
                                        var from1 = new Point(this._c.x, this._c.y-d1*j);
                                        from1 = rotatePoint(from1, this._c, degreesToRadians(this._a)*i);
                                        var to1 = rotatePoint(from1, this._c, degreesToRadians(this._a));
                                        this._drawBackLine(this._$ctx, from1, to1);
                                }
                               
                                this._drawName(this._$ctx, this._pArr[i], i); //名称
                        }
                       
                        for (var i=0;i<this._cfg.referCount+1;i++) {
                                var n0 = parseInt(i*this._cfg.maxValue/this._cfg.referCount);
                                var d0 = this._r/this._cfg.referCount;
                                var p0 = new Point(this._c.x, this._c.y - i*d0);
                                this._drawReferText(this._$ctx, n0, p0);
                        }

                        for (i=0;i<this._n;i++) { //画线
                          var p1 = new Point( this._c.x, this._c.y - (this._cfg.valueArr[i]/this._cfg.maxValue)*this._cfg.radius );
                                this._vArr[i] = rotatePoint(p1, this._c, degreesToRadians(this._a)*i);
                        }
                        this._drawLine(this._$ctx, this._vArr);
                        this._drawVertex(this._$ctx, this._vArr);
                },
                getVertexes: function(){
                        return this._pArr;
                },
               
                _drawBackLine: function(ctx, p1, p2){ //画背景线
                        ctx.beginPath();
                        ctx.moveTo(p1.x, p1.y);
                        ctx.lineTo(p2.x, p2.y);
                        ctx.strokeStyle = this._cfg.backLineColor;
                        ctx.lineCap = this._cfg.lineCap;
                        ctx.lineWidth = this._cfg.backLineWidth;
                        ctx.stroke();
                        ctx.closePath();
                },
                _drawLine: function(ctx, arr){ //画线
                        ctx.beginPath();
                        for (var i=0;i<this._n;i++) {
                                var from = arr[i];
                                var to = i<this._n-1? arr[i+1] :arr[0];
                                ctx.moveTo( from.x, from.y );
                                ctx.lineTo( to.x, to.y );
                        }
                        ctx.strokeStyle = this._cfg.lineColor;
                        ctx.lineCap = this._cfg.lineCap;
                        ctx.lineWidth = this._cfg.lineWidth;
                        ctx.stroke();
                        ctx.closePath();
                },
                _drawVertex: function(ctx, arr){ //画顶点
                        for (var i=0;i<this._n;i++) {
                                ctx.beginPath();
                                ctx.arc(arr[i].x, arr[i].y, this._cfg.vertexRadius, 0, degreesToRadians(360));
                                ctx.fillStyle = this._cfg.lineColor;
                                ctx.fill();
                                ctx.closePath();
                        }
                       
                        for (i=0;i<this._n;i++) {
                                var ele = document.createElement('div');
                                ele.style.width = 4*this._cfg.vertexRadius + 'px';
                                ele.style.height = 4*this._cfg.vertexRadius + 'px';
                                ele.style.lineHeight = 6*this._cfg.vertexRadius + 'px';
                                ele.style.overflow = 'hidden';
                                ele.style.position = 'absolute';
                                ele.style.zIndex = 1;
                                ele.style.top = parseInt(arr[i].y - 2*this._cfg.vertexRadius) + 'px';
                                ele.style.left = parseInt(arr[i].x - 2*this._cfg.vertexRadius) + 'px';
                                ele.style.cursor = 'pointer';
                                ele.title = this._cfg.descArr[i]
                                        ? this._cfg.descArr[i].replace('$0', this._cfg.valueArr[i]||'')
                                        : '';
                                this._$inn.appendChild(ele);
                               
                                ele.innerHTML = '&nbsp;&nbsp;&nbsp;&nbsp;';
                        }
                },
                _drawName: function(ctx, p, i){ //画名称
                        ctx.beginPath();
                        ctx.font = '12px/2 Unknown Font, sans-serif';
                       
                        var txt = this._cfg.nameArr[i]||'';
                        var disX = 10; //距离顶点的位置
                        var disY = 5;
                        var mis = 2; //计算出的点会有误差        
                        var w = this._$ctx.measureText(txt).width;
                        var p1 = new Point(p.x, p.y);
                       
                        var dx = p1.x - this._c.x;
                        if (dx>=-mis && dx<=mis){
                                p1.x -= .5*w;
                        }else if (dx>mis){
                                p1.x += disX;
                        }else if (dx<mis){
                                p1.x -= w+disX;
                        }
                       
                        var dy = p1.y - this._c.y;
                        if (dy>=-mis && dy<=mis){
                                ctx.textBaseline = 'middle';
                                if(window.G_vmlCanvasManager){
                                        p1.y += 12;
                                }
                        }else if (dy>mis){
                                ctx.textBaseline = 'top';
                                p1.y += disY;
                                if(window.G_vmlCanvasManager){
                                        p1.y += 24;
                                }
                        }else if (dy<mis){
                                ctx.textBaseline = 'bottom';
                                p1.y -= disY;
                        }
                       
                        ctx.fillStyle = this._cfg.nameColor;
                        ctx.fillText(txt, p1.x, p1.y);
                        ctx.closePath();
                },
                _drawReferText: function(ctx, txt, p){ //画参考线的刻度数
                        ctx.beginPath();
                        ctx.font = '12px/2 Unknown Font, sans-serif';                  
                        ctx.fillStyle = this._cfg.backLineColor;
                        ctx.fillText(txt, p.x + 15, p.y + 7);
                        ctx.closePath();
                }
        };
       
        return Const;
}(document, Math));