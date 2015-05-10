/*
 for fuzzy control attribute Delta, which is for chunk size control during splitting a job in Co-allocation

 In this control, size has 3 levels, pattern has 2 levels, linkLoad has 3 levels, maxBW has 3 levels,
 lamda and delta all have 3 levels

 The rull table:

 small size -> samll delta
 medium size -> medium delta
 large size -> large delta

 m-s pattern -> small delta
 a-a pattern -> large delta

 small size -> large lamda
 medium size -> medium lamda
 large size -> small lamda

 m-s pattern -> large lamda
 a-a pattern -> small lamda

 low workload -> large lamda
 medium workload -> medium lamda
 high workload -> small lamda

 low maxBW -> small lamda
 medium maxBW -> medium lamda
 high maxBW -> large lamda


 factor support weight (fsw):

  fsw(size, delta) = 0.5
  fsw(pattern, delta) = 0.5

  fsw(size, lamda) = 0.2
  fsw(pattern, lamda) = 0.3
  fsw(workload, lamda) = 0.2
  fsw(maxBW, lamda)=0.3

  using center of gravity to do defuzzification
   center(small) = 0.1
   center(medium)=0.5
   center(large)=0.9

   This code is modefied on June 14,2007 by using cog to do defuzzification
*/

# ifndef FuzzyControl_h
# define FuzzyControl_h

#include <stdio.h>
#include <stdlib.h>

typedef int TYPE;
 TYPE MS_TYPE = 2;
 TYPE AA_TYPE = 3;

class FuzzyControl {
public:
  FuzzyControl();
  ~FuzzyControl();

  double getFuzzyControlDelta(int size, TYPE t);
  double getFuzzyControlLambda(int size, TYPE t, double linkload, double linkMaxBW);
  double getDefaultDelta();
  double getDefaultLamda();


  void setSizeMembershipFunction(int size);
  void setPatternMembershipFunction(TYPE t);
  void setLinkLoadMembershipFunction(double load);
  void setMaxBWMembershipFunction(double maxBW);

  void setDeltaBySize(int size, double delta[]);
  void setDeltaByPattern(TYPE t, double delta[]);
  void setLamdaBySize(int size, double lamda[]);
  void setLamdaByPattern(TYPE t, double lamda[]);
  void setLamdaByLinkLoad(double load, double lamda[]);
  void setLamdaByMaxBW(double maxBW, double lamda[]);

  /*
  FOLLOWING TWO LINES ARE NOT USED, WE USE CENTER OF GRAVITY OF DO THE DEFUZZIFICATION SINCE JUNE 14, 2007
  double getThresholdByMemberConfidence(int memberLevel, double confidence);
  void setResultMemberConfidence(double membership[],int numMembers,
	  int *memberLevel, double *confidence);
  */
  double cog(double thresholdLevels[], int numLevles);
  void printCenters();

private:
  double *_sizeMembership;
  double *_patternMembership;
  double *_linkLoadMembership;
  double *_maxBWMembership;

  double *_deltaMembership;
  double *_lamdaMembership;

  /*
  static const double SIZE_WEIGHT_IN_DELTA=0.5;
  static const double PATTERN_WEIGHT_IN_DELTA=0.5;

  static const double SIZE_WEIGHT_IN_LAMDA=0.2;
  static const double PATTERN_WEIGHT_IN_LAMDA=0.3;
  static const double LINKLOAD_WEIGHT_IN_LAMDA=0.2;
  static const double MAXBW_WEIGHT_IN_LAMDA=0.3;  //THE WEIGHT OF MAXIMUM LINK BANDWIDTH IN LINK SATURATEION LEVEL CONTORL

  static const int NUM_SIZE_MEMBERS=3;
  static const int NUM_PATTERN_MEMBERS=2;
  static const int NUM_LINKLOAD_MEMBERS=3;
  static const int NUM_MAXBW_MEMBERS=3;
  static const int NUM_LAMDA_MEMBERS=3;
  static const int NUM_DELTA_MEMBERS=3;
  static const int SMALL=0;
  static const int MEDIUM=1;
  static const int LARGE=2;
  static const int MS=0;
  static const int AA=1;
  static const int LOW=0;
  static const int HIGH=2;
  static const double THRESHOLD_LEVEL_CENTERS[];
  */
   double SIZE_WEIGHT_IN_DELTA;
   double PATTERN_WEIGHT_IN_DELTA;

   double SIZE_WEIGHT_IN_LAMDA;
   double PATTERN_WEIGHT_IN_LAMDA;
   double LINKLOAD_WEIGHT_IN_LAMDA;
   double MAXBW_WEIGHT_IN_LAMDA;  //THE WEIGHT OF MAXIMUM LINK BANDWIDTH IN LINK SATURATEION LEVEL CONTORL

   int NUM_SIZE_MEMBERS;
   int NUM_PATTERN_MEMBERS;
   int NUM_LINKLOAD_MEMBERS;
   int NUM_MAXBW_MEMBERS;
   int NUM_LAMDA_MEMBERS;
   int NUM_DELTA_MEMBERS;
   int NUM_THRESHOLD_LEVELS;
   int SMALL;
   int MEDIUM;
   int LARGE;
   int MS;
   int AA;
   int LOW;
   int HIGH;
   static const double THRESHOLD_LEVEL_CENTERS[];

};

FuzzyControl* fuzzy = (FuzzyControl*) new FuzzyControl();
extern "C" double call_getFuzzyControlDelta(int size, int t);
extern "C" double call_getFuzzyControlLamda(int size, int t, double linkload, double linkMaxBW);
extern "C" void call_FuzzyControlDestroy();
//
//double getFuzzyControlDelta(int size, TYPE t);
//double getFuzzyControlLamda(int size, TYPE t, double linkload, double linkMaxBW);

#endif

