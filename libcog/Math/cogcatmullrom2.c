#include "cogcatmullrom2.h"


static CogVector2 _evalSegmentPosition2(const CogCatmullRom2 spline, const float t, const int segment) {
  int i = segment + 1;
  float t2 = t * t;
  float t3 = t2 * t;
  
  return (CogVector2) {
    .x = 0.5f * ( (2.0f * spline->points->data[i].x) + 
                  (      -spline->points->data[i - 1].x +        spline->points->data[i + 1].x) * t +
                  (2.0f * spline->points->data[i - 1].x - 5.0f * spline->points->data[i].x + 4.0f * spline->points->data[i + 1].x - spline->points->data[i + 2].x) * t2 +
                  (      -spline->points->data[i - 1].x + 3.0f * spline->points->data[i].x - 3.0f * spline->points->data[i + 1].x + spline->points->data[i + 2].x)  * t3 ),
    .y = 0.5f * ( (2.0f * spline->points->data[i].y) + 
                  (      -spline->points->data[i - 1].y +        spline->points->data[i + 1].y) * t +
                  (2.0f * spline->points->data[i - 1].y - 5.0f * spline->points->data[i].y + 4.0f * spline->points->data[i + 1].y - spline->points->data[i + 2].y) * t2 +
                  (      -spline->points->data[i - 1].y + 3.0f * spline->points->data[i].y - 3.0f * spline->points->data[i + 1].y + spline->points->data[i + 2].y)  * t3 )
  };
}

static CogVector2 _evalSegmentDerivative2(const CogCatmullRom2 spline,const float t, const int segment) {
  int i = segment + 1;
  float t2 = t * t;
  // calculate the first derivative approach
  return (CogVector2) {
    .x = 0.5f * ( (        -spline->points->data[i - 1].x +        spline->points->data[i + 1].x) +
                  (  4.0f * spline->points->data[i -1].x - 10.0f * spline->points->data[i].x + 8.0f * spline->points->data[i + 1].x - 2.0f * spline->points->data[i + 2].x) * t +
                  (- 3.0f * spline->points->data[i-1].x +   9.0f * spline->points->data[i].x - 9.0f * spline->points->data[i + 1].x + 3.0f * spline->points->data[i + 2].x) * t2 ),
    .y = 0.5f * ( (        -spline->points->data[i - 1].y +        spline->points->data[i + 1].y) +
                  (  4.0f * spline->points->data[i -1].y - 10.0f * spline->points->data[i].y + 8.0f * spline->points->data[i + 1].y - 2.0f * spline->points->data[i + 2].y) * t +
                  (- 3.0f * spline->points->data[i-1].y +   9.0f * spline->points->data[i].y - 9.0f * spline->points->data[i + 1].y + 3.0f * spline->points->data[i + 2].y) * t2 )
  };
}

static CogVector2 _evalSegmentTangent2(const CogCatmullRom2 spline, const float t, const float eps, const int segment) {
  if(t <= 0.0f) {
    CogVector2 ret = CogVector2Sub(spline->points->data[segment - 1], spline->points->data[segment + 1]);
    return CogVector2Normalized(ret);
  } else  if(t > (1.0f - eps)) {
    CogVector2 ret = CogVector2Sub(spline->points->data[segment], spline->points->data[segment + 2]);
    return CogVector2Normalized(ret);
  } else {
    CogVector2 ret, src, dest;
    src = _evalSegmentPosition2(spline, t, segment);
    dest = _evalSegmentPosition2(spline, t + eps, segment);
    ret = CogVector2Sub(dest, src);
    return ret;
  }
}


/*********************/
/** Class Functions **/
/*********************/
CogCatmullRom2 CogCatmullRom2Alloc(void) {
  CogCatmullRom2 self = malloc(sizeof(struct CogCatmullRom2)); 
  
  self->points = CogArrayAlloc(CogVector2);
  self->arcs = CogArrayAlloc(float);
  self->len = 0.0f;
  
  return self;
}

void CogCatmullRom2Free(CogCatmullRom2 self) {
  CogArrayFree(self->points);
  CogArrayFree(self->arcs);
  free(self);
}

void CogCatmullRom2AddPoint(CogCatmullRom2 self, const CogVector2 point) {
  CogArrayPush(self->points, point);
}

void CogCatmullRom2Clear(CogCatmullRom2 self) {
  CogArrayFree(self->points);
  CogArrayFree(self->arcs);
  self->points = CogArrayAlloc(CogVector2);
  self->arcs = CogArrayAlloc(float);
  self->len = 0.0f;
}


void CogCatmullRom2Compile(CogCatmullRom2 self, const int resolution, const Bool closed) {
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
  
  CogVector2 nv, ov, tmp;
  
  float step = 1.0f / (float)resolution;
  
  // iterate over segments
  for(i = 0; i < numSegments; ++i) {
    ov = nv = _evalSegmentPosition2(self, 0.0, i);
    // aproximate lenghts for current segment
    len = 0.0f;
    for(j = 1; j <= resolution; ++j) {
      float t = (float)j * step;
      // protect from extrapolation float error
      if(t > 1.0f) t = 1.0f;
      nv = _evalSegmentPosition2(self, t, i);
      tmp = CogVector2Sub(nv, ov);
      len += CogVector2Len(tmp);
      ov = nv;
    } 
    CogArrayPush(self->arcs, len);
    self->len += len;
  }
}

CogVector2 CogCatmullRom2Position(const CogCatmullRom2 self, const float t){
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
    return _evalSegmentPosition2(self, nt, i);
}

CogVector2 CogCatmullRom2Tangent(const CogCatmullRom2 self, const float t, const float epsilon) {
    // first we have to find in which segment our t lies
    float len = t * self->len;
    float tlen = 0.0f;
    int i;
    for(i = 0; i < self->arcs->size; ++i) {
        // if target len is in between curent segments start len and end len we have found a target segment
        if( (tlen >= len && tlen) < (len + self->arcs->data[i]) ) break;
        tlen += self->arcs->data[i];
    }
    
    // transform our interpolation factor to target segment's lenght span
    float nt = (len - tlen) / (self->arcs->data[i]);
    
    // get the position at target interpolation factor for target segment
    return _evalSegmentTangent2(self, nt, epsilon, i);
}

void CogCatmullRom2Inspect(const CogCatmullRom2 self) {
  printf("<CogCatmullRom2 Length: %f, Control Points: %d, Segments: %d [\n", self->len, self->points->size, self->arcs->size);
  printf("Points: "); CogArrayInspect(CogVector2, self->points);
  printf("Arc Lenghts: "); CogArrayInspect(float, self->arcs);
  printf("]>\n");
}