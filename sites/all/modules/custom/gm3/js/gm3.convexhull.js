(function($){
  // Copyright 2011, SDRycroft
  // GPLv2
  // Copyright 2001, softSurfer (www.softsurfer.com)
  // This code may be freely used and modified for any purpose
  // providing that this copyright notice is included with it.
  // SoftSurfer makes no warranty for this code, and cannot be held
  // liable for any real or imagined damage resulting from its use.
  // Users of this code must verify correctness for their application.
  // http://geometryalgorithms.com/Archive/algorithm_0109/algorithm_0109.htm
  Drupal.gm3 = Drupal.gm3 || {};
  Drupal.gm3.convexhull = Drupal.gm3.convexhull || {};
  Drupal.behaviors.gm3_convexhull = {attach: function(context, settings){
    for(map_id in Drupal.settings.gm3.maps) {
      if(Drupal.settings.gm3.maps[map_id]['initialized'] && Drupal.settings.gm3.maps[map_id]['libraries']['point']) {
        Drupal.settings.gm3.maps[map_id]['libraries']['point']['finished'] = Drupal.settings.gm3.maps[map_id]['libraries']['point']['finished'] || new Array();
        Drupal.settings.gm3.maps[map_id]['libraries']['point']['finished'][Drupal.settings.gm3.maps[map_id]['libraries']['point']['finished'].length] = function(map_id){
          Drupal.gm3.convexhull.initialize(map_id);
        }
      }
    }
  }};
  Drupal.gm3.convexhull.initialize = function(map_id){
    Drupal.settings.gm3.maps[map_id]['point']['points'].sort(Drupal.gm3.convexhull.sortPointY);
    Drupal.settings.gm3.maps[map_id]['point']['points'].sort(Drupal.gm3.convexhull.sortPointX);
    var hullPoints = [];
    Drupal.gm3.convexhull.chainHull_2D(Drupal.settings.gm3.maps[map_id]['point']['points'], Drupal.settings.gm3.maps[map_id]['point']['points'].length, hullPoints);
    for(i in hullPoints){
      console.log(hullPoints[i]);
    }
    /*google.maps.Polygon({
      map: Drupal.settings.gm3.maps[map_id]['google_map'],
      paths: hullPoints
    });*/
    //polyline = new GPolygon(hullPoints, "#FF0000", 2, 0.5, "#0000FF");
    //map.addOverlay(polyline);
  }

  Drupal.gm3.convexhull.sortPointX = function(a, b){
    return a.x - b.x;
  }
  Drupal.gm3.convexhull.sortPointY = function(a, b){
    return a.y - b.y;
  }
  // isLeft(): tests if a point is Left|On|Right of an infinite line.
  // Input: three points P0, P1, and P2
  // Return: >0 for P2 left of the line through P0 and P1
  // =0 for P2 on the line
  // <0 for P2 right of the line
  // See: the January 2001 Algorithm on Area of Triangles
  Drupal.gm3.convexhull.isLeft = function(P0, P1, P2){
    return (P1.x - P0.x) * (P2.y - P0.y) - (P2.x - P0.x) * (P1.y - P0.y);
  }
  // chainHull_2D(): Andrew's monotone chain 2D convex hull algorithm
  // Input: P[] = an array of 2D points
  // presorted by increasing x- and y-coordinates
  // n = the number of points in P[]
  // Output: H[] = an array of the convex hull vertices (max is n)
  // Return: the number of points in H[]
  Drupal.gm3.convexhull.chainHull_2D = function(P, n, H){
    // the output array H[] will be used as the stack
    var bot = 0, top = (-1); // indices for bottom and top of the stack
    var i; // array scan index

    // Get the indices of points with min x-coord and min|max y-coord
    var minmin = 0, minmax;
    var xmin = P[0].x;
    for(i = 1; i < n; i++)
      if(P[i].x != xmin) break;
    minmax = i - 1;
    if(minmax == n - 1) { // degenerate case: all x-coords == xmin
      H[++top] = P[minmin];
      if(P[minmax].y != P[minmin].y) // a nontrivial segment
      H[++top] = P[minmax];
      H[++top] = P[minmin]; // add polygon endpoint
      return top + 1;
    }

    // Get the indices of points with max x-coord and min|max y-coord
    var maxmin, maxmax = n - 1;
    var xmax = P[n - 1].x;
    for(i = n - 2; i >= 0; i--)
      if(P[i].x != xmax) break;
    maxmin = i + 1;

    // Compute the lower hull on the stack H
    H[++top] = P[minmin]; // push minmin point onto stack
    i = minmax;
    while(++i <= maxmin) {
      // the lower line joins P[minmin] with P[maxmin]
      if(Drupal.gm3.convexhull.isLeft(P[minmin], P[maxmin], P[i]) >= 0 && i < maxmin) continue;
      // ignore P[i] above or on the lower line
      while(top > 0) // there are at least 2 points on the stack
      {
        // test if P[i] is left of the line at the stack top
        if(Drupal.gm3.convexhull.isLeft(H[top - 1], H[top], P[i]) > 0)
          break; // P[i] is a new hull vertex
        else
          top--; // pop top point off stack
      }
      H[++top] = P[i]; // push P[i] onto stack
    }

    // Next, compute the upper hull on the stack H above the bottom hull
    if(maxmax != maxmin) // if distinct xmax points
    H[++top] = P[maxmax]; // push maxmax point onto stack
    bot = top; // the bottom point of the upper hull stack
    i = maxmin;
    while(--i >= minmax) {
      // the upper line joins P[maxmax] with P[minmax]
      if(Drupal.gm3.convexhull.isLeft(P[maxmax], P[minmax], P[i]) >= 0 && i > minmax) continue;
      // ignore P[i] below or on the upper line
      while(top > bot) // at least 2 points on the upper stack
      {
        // test if P[i] is left of the line at the stack top
        if(Drupal.gm3.convexhull.isLeft(H[top - 1], H[top], P[i]) > 0)
          break; // P[i] is a new hull vertex
        else
          top--; // pop top point off stack
      }
      H[++top] = P[i]; // push P[i] onto stack
    }
    if(minmax != minmin) H[++top] = P[minmin]; // push joining endpoint onto
    // stack

    return top + 1;
  }
})(jQuery);

var gmarkers = [];
var points = [];
var hullPoints = [];
var map;
var polyline;