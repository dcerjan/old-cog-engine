#include "cogcatmullrom3.h"

static CogVector3 _evalSegmentPosition3(const CogCatmullRom3 spline, const float t, const int segment) {
  int i = segment + 1;
  float t2 = t * t;
  float t3 = t2 * t;
  
  return (CogVector3) {
    .x = 0.5f * ( (2.0f * spline->points->data[i].x) + 
                  (      -spline->points->data[i - 1].x +        spline->points->data[i + 1].x) * t +
                  (2.0f * spline->points->data[i - 1].x - 5.0f * spline->points->data[i].x + 4.0f * spline->points->data[i + 1].x - spline->points->data[i + 2].x) * t2 +
                  (      -spline->points->data[i - 1].x + 3.0f * spline->points->data[i].x - 3.0f * spline->points->data[i + 1].x + spline->points->data[i + 2].x)  * t3 ),
    .y = 0.5f * ( (2.0f * spline->points->data[i].y) + 
                  (      -spline->points->data[i - 1].y +        spline->points->data[i + 1].y) * t +
                  (2.0f * spline->points->data[i - 1].y - 5.0f * spline->points->data[i].y + 4.0f * spline->points->data[i + 1].y - spline->points->data[i + 2].y) * t2 +
                  (      -spline->points->data[i - 1].y + 3.0f * spline->points->data[i].y - 3.0f * spline->points->data[i + 1].y + spline->points->data[i + 2].y)  * t3 ),
    .z = 0.5f * ( (2.0f * spline->points->data[i].z) + 
                  (      -spline->points->data[i - 1].z +        spline->points->data[i + 1].z) * t +
                  (2.0f * spline->points->data[i - 1].z - 5.0f * spline->points->data[i].z + 4.0f * spline->points->data[i + 1].z - spline->points->data[i + 2].z) * t2 +
                  (      -spline->points->data[i - 1].z + 3.0f * spline->points->data[i].z - 3.0f * spline->points->data[i + 1].z + spline->points->data[i + 2].z)  * t3 )
  };
}

static CogVector3 _evalSegmentDerivative3(const CogCatmullRom3 spline,const float t, const int segment) {
  int i = segment + 1;
  float t2 = t * t;
  // calculate the first derivative approach
  return (CogVector3) {
    .x = 0.5f * ( (        -spline->points->data[i - 1].x +        spline->points->data[i + 1].x) +
                  (  4.0f * spline->points->data[i -1].x - 10.0f * spline->points->data[i].x + 8.0f * spline->points->data[i + 1].x - 2.0f * spline->points->data[i + 2].x) * t +
                  (- 3.0f * spline->points->data[i-1].x +   9.0f * spline->points->data[i].x - 9.0f * spline->points->data[i + 1].x + 3.0f * spline->points->data[i + 2].x) * t2 ),
    .y = 0.5f * ( (        -spline->points->data[i - 1].y +        spline->points->data[i + 1].y) +
                  (  4.0f * spline->points->data[i -1].y - 10.0f * spline->points->data[i].y + 8.0f * spline->points->data[i + 1].y - 2.0f * spline->points->data[i + 2].y) * t +
                  (- 3.0f * spline->points->data[i-1].y +   9.0f * spline->points->data[i].y - 9.0f * spline->points->data[i + 1].y + 3.0f * spline->points->data[i + 2].y) * t2 ),
    .z = 0.5f * ( (        -spline->points->data[i - 1].z +        spline->points->data[i + 1].z) +
                  (  4.0f * spline->points->data[i -1].z - 10.0f * spline->points->data[i].z + 8.0f * spline->points->data[i + 1].z - 2.0f * spline->points->data[i + 2].z) * t +
                  (- 3.0f * spline->points->data[i-1].z +   9.0f * spline->points->data[i].z - 9.0f * spline->points->data[i + 1].z + 3.0f * spline->points->data[i + 2].z) * t2 ),
  };
}

static CogVector3 _evalSegmentTangent3(const CogCatmullRom3 spline, const float t, const float eps, const int segment) {
  if(t <= 0.0f) {
    CogVector3 ret = CogVector3Sub(spline->points->data[segment - 1], spline->points->data[segment + 1]);
    return CogVector3Normalized(ret);
  } else  if(t > (1.0f - eps)) {
    CogVector3 ret = CogVector3Sub(spline->points->data[segment], spline->points->data[segment + 2]);
    return CogVector3Normalized(ret);
  } else {
    CogVector3 ret, src, dest;
    src = _evalSegmentPosition3(spline, t, segment);
    dest = _evalSegmentPosition3(spline, t + eps, segment);
    ret = CogVector3Sub(dest, src);
    return ret;
  }
}


/*********************/
/** Class Functions **/
/*********************/
CogCatmullRom3 CogCatmullRom3Alloc(void) {
  CogCatmullRom3 self = malloc(sizeof(struct CogCatmullRom3)); 
  
  self->points = CogArrayAlloc(CogVector3);
  self->arcs = CogArrayAlloc(float);
  self->len = 0.0f;
  
  return self;
}

void CogCatmullRom3Free(CogCatmullRom3 self) {
  CogArrayFree(self->points);
  CogArrayFree(self->arcs);
  free(self);
}

void CogCatmullRom3AddPoint(CogCatmullRom3 self, const CogVector3 point) {
  CogArrayPush(self->points, point);
}

void CogCatmullRom3Clear(CogCatmullRom3 self) {
  CogArrayFree(self->points);
  CogArrayFree(self->arcs);
  self->points = CogArrayAlloc(CogVector3);
  self->arcs = CogArrayAlloc(float);
  self->len = 0.0f;
}


void CogCatmullRom3Compile(CogCatmullRom3 self, const int resolution, const Bool closed) {
  assert(self->points->size > 3 && resolution > 0);
  
  if(closed) {
    CogArrayPush(self->points, self->points->data[0]);
    CogArrayPush(self->points, self->points->data[1]);
    CogArrayPush(self->points, self->points->data[2]);
  }
  
  int numSegments = self->points->size - 3;
  self->len = 0.0f;
  // we have to rebuild lenghts table for normalized access and transformation of input interpolant factor
  CogArrayFree(self->arcs);
  self->arcs = CogArrayAlloc(float);
  int i, j;
  float len;
  
  CogVector3 nv, ov, tmp;
  
  float step = 1.0f / (float)resolution;
  
  // iterate over segments
  for(i = 0; i < numSegments; ++i) {
    ov = nv = _evalSegmentPosition3(self, 0.0, i);
    // aproximate lenghts for current segment
    len = 0.0f;
    for(j = 1; j <= resolution; ++j) {
      float t = (float)j * step;
      // protect from extrapolation float error
      if(t > 1.0f) t = 1.0f;
      nv = _evalSegmentPosition3(self, t, i);
      tmp = CogVector3Sub(nv, ov);
      len += CogVector3Len(tmp);
      ov = nv;
    } 
    CogArrayPush(self->arcs, len);
    self->len += len;
  }
}

CogVector3 CogCatmullRom3Position(const CogCatmullRom3 self, const float t){
    // first we have to find in which segment our t lies
    float len = t * self->len;
    float tlen = 0.0f;
    int i;
    for(i = 0; i < self->arcs->size; ++i) {
        // if target len is in between curent segments start len and end len we have found a target segment
        if( (len > tlen) && (len  <= (tlen + self->arcs->data[i])) ) break;
        tlen += self->arcs->data[i];
    }
    
    float nt = (len - tlen) / (self->arcs->data[i]);
    // get the position at target interpolation factor for target segment
    return _evalSegmentPosition3(self, nt, i);
}

CogVector3 CogCatmullRom3Tangent(const CogCatmullRom3 self, const float t, const float epsilon) {
    // first we have to find in which segment our t lies
    float len = t * self->len;
    float tlen = 0.0f;
    int i;
    for(i = 0; i < self->arcs->size; ++i) {
        // if target len is in between curent segments start len and end len we have found a target segment
        if( (tlen >= len) && (tlen < (len + self->arcs->data[i]) )) break;
        tlen += self->arcs->data[i];
    }
    
    // transform our interpolation factor to target segment's lenght span
    float nt = (len - tlen) / (self->arcs->data[i]);
    
    // get the position at target interpolation factor for target segment
    return _evalSegmentTangent3(self, nt, epsilon, i);
}

void CogCatmullRom3Inspect(const CogCatmullRom3 self) {
  printf("<CogCatmullRom3 Length: %f, Control Points: %d, Segments: %d [\n", self->len, self->points->size, self->arcs->size);
  printf("Points: "); CogArrayInspect(CogVector3, self->points);
  printf("Arc Lenghts: "); CogArrayInspect(float, self->arcs);
  printf("]>\n");
}