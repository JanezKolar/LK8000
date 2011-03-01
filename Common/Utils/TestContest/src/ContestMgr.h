/*
   LK8000 Tactical Flight Computer -  WWW.LK8000.IT
   Released under GNU/GPL License v.2
   See CREDITS.TXT file for authors and copyrights

   $Id: $
*/

#ifndef __COMPETITIONMGR_H__
#define __COMPETITIONMGR_H__

#include "Trace.h"


class CContestMgr {
public:
  enum TType {
    TYPE_OLC_CLASSIC,
    TYPE_OLC_FAI,
    TYPE_OLC_LEAGUE,
    TYPE_NUM
  };
  
  class CRules {
    unsigned _tpNum;
    unsigned _timeLimit;
    double _finishAltDiff;
  public:
    CRules(unsigned tpNum, unsigned timeLimit, double finishAltDiff):
      _tpNum(tpNum), _finishAltDiff(finishAltDiff) {}
    unsigned TPNum() const       { return _tpNum; }
    unsigned TimeLimit() const   { return _timeLimit; }
    double FinishAltDiff() const { return _finishAltDiff; }
  };
  
  class CSolution {
    double         _distance;
    double         _score;
    CPointGPSArray _pointArray;
  public:
    CSolution(): _distance(0), _score(0) {}
    CSolution(double distance, double score, const CPointGPSArray &pointArray):
      _distance(distance), _score(score), _pointArray(pointArray) {}
    double Distance() const                  { return _distance; }
    double Score() const                     { return _score / 1000; }
    double Duration() const                  { return _pointArray.empty() ? 0 : (_pointArray.back().Time() - _pointArray.front().Time()); }
    double Speed() const                     { return _pointArray.size() ? (_distance / Duration()) : 0; }
    const CPointGPSArray &PointArray() const { return _pointArray; }
  };
  
  typedef std::vector<CSolution> CSolutionArray;
  
private:
  const unsigned _handicap;
  CTrace _trace;
  CTrace _traceSprint;
  CSolutionArray _solutionArray;
  
  void UpdateOLCClassic(const CRules &rules);
  
public:
  static const unsigned TRACE_FIX_LIMIT = 250;
  static const unsigned TRACE_SPRINT_FIX_LIMIT = 100;
  static const unsigned TRACE_SPRINT_TIME_LIMIT = 150 * 60;
  //  static const unsigned COMPRESSION_ALGORITHM = CTrace::ALGORITHM_TRIANGLES | CTrace::ALGORITHM_TIME_DELTA;
  static const unsigned COMPRESSION_ALGORITHM = CTrace::ALGORITHM_DISTANCE  | CTrace::ALGORITHM_TIME_DELTA;
  //static const unsigned COMPRESSION_ALGORITHM = CTrace::ALGORITHM_DISTANCE  | CTrace::ALGORITHM_INHERITED | CTrace::ALGORITHM_TIME_DELTA;
  
  CContestMgr(unsigned handicap, unsigned startAltitudeLoss);
  
  const CTrace &Trace() const { return _trace; }
  
  void Add(const CPointGPSSmart &gps);
  const CSolution &Solution(TType type) const { return _solutionArray[type]; }
};

#endif /* __CONTESTMGR_H__ */