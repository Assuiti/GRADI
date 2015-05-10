#include "FuzzyControl.h"
#include "jni.h"
#include "control_JavaFuzzyControl.h"

FuzzyControl::FuzzyControl(){

	SIZE_WEIGHT_IN_DELTA=0.5;
   PATTERN_WEIGHT_IN_DELTA=0.5;

   SIZE_WEIGHT_IN_LAMDA=0.2;
   PATTERN_WEIGHT_IN_LAMDA=0.3;
   LINKLOAD_WEIGHT_IN_LAMDA=0.3;
   MAXBW_WEIGHT_IN_LAMDA=0.2;  //THE WEIGHT OF MAXIMUM LINK BANDWIDTH IN LINK SATURATEION LEVEL CONTORL

    NUM_SIZE_MEMBERS=3;
    NUM_PATTERN_MEMBERS=2;
    NUM_LINKLOAD_MEMBERS=3;
    NUM_MAXBW_MEMBERS=3;
    NUM_LAMDA_MEMBERS=3;
    NUM_DELTA_MEMBERS=3;
	NUM_THRESHOLD_LEVELS=3;
    SMALL=0;
    MEDIUM=1;
    LARGE=2;
    MS=0;
    AA=1;
    LOW=0;
    HIGH=2;

  _sizeMembership=(double *)malloc(sizeof(double)*(NUM_SIZE_MEMBERS));


   _patternMembership=(double *)malloc(sizeof( double)*(NUM_PATTERN_MEMBERS));

   _linkLoadMembership=(double *)malloc(sizeof(double)*(NUM_LINKLOAD_MEMBERS));

   _maxBWMembership=(double *)malloc(sizeof(double)*(NUM_MAXBW_MEMBERS));


  /*
   _deltaMembership=new double(NUM_DELTA_MEMBERS);
  for(i=0; i<NUM_DELTA_MEMBERS;i++)
    _deltaMembership[i]=0;

   _lamdaMembership=new double(NUM_LAMDA_MEMBERS);
  for(i=0; i<NUM_LAMDA_MEMBERS;i++)
    _lamdaMembership[i]=0;
	*/


 //THRESHOLD_LEVEL_CENTERS[]= new double(3);
	//THRESHOLD_LEVEL_CENTERS={0.1,0.5,0.9};

}

FuzzyControl::~FuzzyControl(){
  delete _sizeMembership;
  delete _patternMembership;
  delete _linkLoadMembership;
  delete _maxBWMembership;
 // delete _deltaMembership;
  //delete _lamdaMembership;

}

void FuzzyControl::setSizeMembershipFunction(int sizeOriginal){
/* following is original size membership function
  if(size <50)
    _sizeMembership[SMALL]=1;

  else if( size>=50 && size <=100){
    _sizeMembership[SMALL]=(-0.02)*size+2;
    _sizeMembership[MEDIUM]=0.02*size-1;
  }

  else if(size>100 && size <150)
    _sizeMembership[MEDIUM]=1;

  else if(size>=150 && size <=200){
    _sizeMembership[MEDIUM]=(-0.02)*size+4;
    _sizeMembership[LARGE]=0.02*size-3;
  }
  else
    _sizeMembership[LARGE]=1;

*/
	int i;
	double size;


	size= (sizeOriginal*0.1)/70.0*100.0/0.1;


	for(i=0; i<NUM_SIZE_MEMBERS;i++)
    _sizeMembership[i]=0;
	//cout<<" in setSizeMembershipFunction lllllllllllllll"<<endl;
	//cout<<" begin size members:"<<_sizeMembership[0]<<" "<<_sizeMembership[1]<<" "<<_sizeMembership[2]<<endl;
	if(size <=20) {
		_sizeMembership[SMALL]=1;
		//cout<<" size members:"<<_sizeMembership[0]<<" "<<_sizeMembership[1]<<" "<<_sizeMembership[2]<<endl;
	}
	else if( size>20 && size <40){
		_sizeMembership[SMALL]=(-0.05)*size+2;
		_sizeMembership[MEDIUM]=0.05*size-1;
		//cout<<" size members:"<<_sizeMembership[0]<<" "<<_sizeMembership[1]<<" "<<_sizeMembership[2]<<endl;
	}
	else if(size>=40 && size <=60) {
		_sizeMembership[MEDIUM]=1;
		//cout<<" size members:"<<_sizeMembership[0]<<" "<<_sizeMembership[1]<<" "<<_sizeMembership[2]<<endl;
	}
	else if(size>60 && size <80){
		_sizeMembership[MEDIUM]=(-0.05)*size+4;
		_sizeMembership[LARGE]=0.05*size-3;
		//cout<<" size members:"<<_sizeMembership[0]<<" "<<_sizeMembership[1]<<" "<<_sizeMembership[2]<<endl;
	}
	else {
		_sizeMembership[LARGE]=1;
	//cout<<" size members:"<<_sizeMembership[0]<<" "<<_sizeMembership[1]<<" "<<_sizeMembership[2]<<endl;
	}
}

void FuzzyControl::setPatternMembershipFunction(TYPE t){
	//cout<<" in setPatternMembershipFunction lllllllllllllll"<<endl;
	int i;
	for(i=0; i<NUM_PATTERN_MEMBERS;i++)
    _patternMembership[i]=0;

  if(t==MS_TYPE)
    _patternMembership[MS]=1;
  else if(t==AA_TYPE)
    _patternMembership[AA]=1;
  else{
    //cout<<"Wrong pattern in FuzzyControl::SetPatternMembershipFunction()!"<<endl;
    exit(1);
  }
  //cout<<" in setPatternMembershipFunction lllllllllllllll"<<endl;
}

void FuzzyControl::setLinkLoadMembershipFunction(double load){
	int i;
  for(i=0; i<NUM_LINKLOAD_MEMBERS;i++)
    _linkLoadMembership[i]=0;

  if(load <=20)
    _linkLoadMembership[LOW]=1;

  else if( load>20 && load <40){
    _linkLoadMembership[LOW]=(-0.05)*load+2;
    _linkLoadMembership[MEDIUM]=0.05*load-1;
  }

  else if(load>=40 && load <=60)
    _linkLoadMembership[MEDIUM]=1;

  else if(load>60 && load <80){
    _linkLoadMembership[MEDIUM]=(-0.05)*load+4;
    _linkLoadMembership[HIGH]=0.05*load-3;
  }
  else
    _linkLoadMembership[HIGH]=1;

}

//maxBW value is in Mbps
void FuzzyControl::setMaxBWMembershipFunction(double maxBW){
	int i;
	for(i=0; i<NUM_MAXBW_MEMBERS;i++)
    _maxBWMembership[i]=0;
	//cout<<" begin maxBW members:"<<_maxBWMembership[0]<<" "<<_maxBWMembership[1]<<" "<<_maxBWMembership[2]<<endl;
  if(maxBW<=55) 			// antes estava maxBW==10
    _maxBWMembership[LOW]=1;
  else if(maxBW>55 && maxBW<=550) 	// antes estava maxBW==100
    _maxBWMembership[MEDIUM]=1;
  else if(maxBW>550) 			// antes estava maxBW==1000
    _maxBWMembership[HIGH]=1;
  else{
    //cout<<"Wrong maxBW in FuzzyControl::SetMaxBWMembershipFunction()!"<<endl;
    exit(1);
  }
   //cout<<" after maxBW members:"<<_maxBWMembership[0]<<" "<<_maxBWMembership[1]<<" "<<_maxBWMembership[2]<<endl;
}


/*
implement the rull:
if size=samll, then delta=samll
if size=medium then delta=medium
if size=large then delta=large

and set the size contribution to the delta by the weight
*/
void FuzzyControl::setDeltaBySize(int size, double delta[]){
	setSizeMembershipFunction(size);
	delta[SMALL]+= SIZE_WEIGHT_IN_DELTA *_sizeMembership[SMALL];
    delta[MEDIUM]+= SIZE_WEIGHT_IN_DELTA *_sizeMembership[MEDIUM];
    delta[LARGE]+= SIZE_WEIGHT_IN_DELTA *_sizeMembership[LARGE];
	//cout<<"delta weight adding size:"<< delta[0]<<" "<<delta[1]<<" "<<delta[2]<<endl;
}

/*
implement the rull and set the pattern contribution to the delta by the weight:
if pattern=MS then delta=small
if pattern=AA then delta= large

*/
void FuzzyControl::setDeltaByPattern(TYPE t, double delta[]){
	setPatternMembershipFunction(t);
	delta[SMALL]+= PATTERN_WEIGHT_IN_DELTA * _patternMembership[MS];
	delta[LARGE]+= PATTERN_WEIGHT_IN_DELTA * _patternMembership[AA];

  //cout<<"delta weight adding pattern:"<< delta[0]<<" "<<delta[1]<<" "<<delta[2]<<endl;
}
/*
implement the rull:
if size=samll, then lamda=large
if size=medium then lamda=medium
if size=large then lamda=small

and set the size contribution to the lamda by the weight
*/
void FuzzyControl::setLamdaBySize(int size, double lamda[]){
	setSizeMembershipFunction(size);
	//cout<<"in setLamdaBySize kkkkkkkkkkkkkkkkkkk"<<endl;
	lamda[LARGE]+=SIZE_WEIGHT_IN_LAMDA * _sizeMembership[SMALL];//cout<<"in setLamdaBySize ssssssssssssssssss"<<endl;
	lamda[MEDIUM]+=SIZE_WEIGHT_IN_LAMDA * _sizeMembership[MEDIUM];
	lamda[SMALL]+= SIZE_WEIGHT_IN_LAMDA * _sizeMembership[LARGE];
	//cout<<"lamda weight adding size:"<< lamda[0]<<" "<<lamda[1]<<" "<<lamda[2]<<endl;
}
/*
implement the rull and set the pattern contribution to the lamda by the weight:
if pattern=MS then lamda=large
if pattern=AA then lamda=small

*/
void FuzzyControl::setLamdaByPattern(TYPE t, double lamda[]){
	setPatternMembershipFunction(t);
	lamda[LARGE]+=PATTERN_WEIGHT_IN_LAMDA*_patternMembership[MS];
	lamda[SMALL]+=PATTERN_WEIGHT_IN_LAMDA*_patternMembership[AA];
	//cout<<"lamda weight adding pattern:"<< lamda[0]<<" "<<lamda[1]<<" "<<lamda[2]<<endl;
}

/*
implement the rull and set the linkLoad contribution to the lamda by the weight:
if load=low then lamda=large
if load=medium then lamda=medium
if load=high then lamda=small
*/
void FuzzyControl::setLamdaByLinkLoad(double load, double lamda[]){
	setLinkLoadMembershipFunction(load);
	lamda[LARGE]+=LINKLOAD_WEIGHT_IN_LAMDA*_linkLoadMembership[LOW];
    lamda[MEDIUM]+=LINKLOAD_WEIGHT_IN_LAMDA*_linkLoadMembership[MEDIUM];
    lamda[SMALL]+=LINKLOAD_WEIGHT_IN_LAMDA*_linkLoadMembership[HIGH];
	//cout<<"lamda weight adding load:"<< lamda[0]<<" "<<lamda[1]<<" "<<lamda[2]<<endl;

}
/*
implement the rull and set the maxBW contribution to the lamda by the weight:
if maxBW=low then lamda=small
if maxBW=medium then lamda=medium
if maxBW=high then lamda=large
*/
void FuzzyControl::setLamdaByMaxBW(double maxBW, double lamda[]){
	setMaxBWMembershipFunction(maxBW);
	lamda[SMALL]+=MAXBW_WEIGHT_IN_LAMDA*_maxBWMembership[LOW];
    lamda[MEDIUM]+=MAXBW_WEIGHT_IN_LAMDA*_maxBWMembership[MEDIUM];
    lamda[LARGE]+=MAXBW_WEIGHT_IN_LAMDA*_maxBWMembership[HIGH];
	//cout<<"lamda weight adding maxBW:"<< lamda[0]<<" "<<lamda[1]<<" "<<lamda[2]<<endl;

}



double FuzzyControl::getFuzzyControlDelta(int size, TYPE t){
	double delta[NUM_DELTA_MEMBERS];
	//int memberLevel;
	//double confidence;
	double value;
	int i;
	for(i=0;i<NUM_DELTA_MEMBERS;i++)
		delta[i]=0;

	setDeltaBySize(size,delta);
	setDeltaByPattern(t,delta);

	/* FOLLOWING TWO LINES ARE NOT USED, WE USE CENTER OF GRAVITY OF DO THE DEFUZZIFICATION SINCE JUNE 14, 2007
	setResultMemberConfidence(delta,NUM_DELTA_MEMBERS, &memberLevel, &confidence);
	return getThresholdByMemberConfidence(memberLevel, confidence);
	*/

	value= cog(delta, NUM_DELTA_MEMBERS);

	//cout<<"--------size="<<size<<", type="<<t;
	//cout<<" delta weight:"<<delta[0]<<", "<<delta[1]<<", "<<delta[2]<<" delta: "<<value<<endl;
	return value;
}

double FuzzyControl::getFuzzyControlLambda(int size, TYPE t, double linkload, double linkMaxBW){
    double lamda[NUM_LAMDA_MEMBERS];
	//int memberLevel;
	//double confidence;
	double value;
	int i;

	//cout<<" in getFuzzyControlLamda kjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjj"<<endl;
	for(i=0;i<NUM_LAMDA_MEMBERS;i++)
		lamda[i]=0;
	//cout<<" in getFuzzyControlLamda kkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkk"<<endl;
	setLamdaBySize(size,lamda);
	setLamdaByPattern(t,lamda);
	setLamdaByLinkLoad(linkload, lamda);
	setLamdaByMaxBW(linkMaxBW,lamda);

	/* FOLLOWING TWO LINES ARE NOT USED, WE USE CENTER OF GRAVITY OF DO THE DEFUZZIFICATION SINCE JUNE 14, 2007
	setResultMemberConfidence(lamda,NUM_LAMDA_MEMBERS, &memberLevel, &confidence);
	return getThresholdByMemberConfidence(memberLevel, confidence);
    */
    value= cog(lamda, NUM_LAMDA_MEMBERS);
    //cout<<"--------size="<<size<<", type="<<t<<", load="<<linkload<<", maxBW="<<linkMaxBW;
		//cout<<" lmada weight:"<<lamda[0]<<", "<<lamda[1]<<", "<<lamda[2]<<" lamda: "<<value<<endl;
	return value;

}
/*
perform the center of gravity (cog) defuzzifation for the threshold
*/
double FuzzyControl::cog(double thresholdLevels[], int numLevels){
	int i;
	double value=0, total_weight=0;
    //double w=1.0;
	for(i=0;i<numLevels;i++){
		//printf("weight=%8.9lf\n", total_weight);
		value+=THRESHOLD_LEVEL_CENTERS[i]*thresholdLevels[i];
		total_weight+= thresholdLevels[i];
	}
	//cout<<"--------weight="<<total_weight<<", threshold="<<value/total_weight<<endl;
	//if(total_weight != w){
		//cout<<" Total weight is not 1 in calculating delta!"<<endl;
		//printf("weight=%8.9lf\n", total_weight);
		//cout<<"&&&&&&&&&&& weight="<<total_weight<<endl;

	//}

	return value/total_weight;

}

double FuzzyControl::getDefaultDelta(){
	return 0;
}
double FuzzyControl::getDefaultLamda(){
	return 1;
}

const double FuzzyControl::THRESHOLD_LEVEL_CENTERS[]={0.1,0.5,0.9};



JNIEXPORT jdouble JNICALL Java_control_JavaFuzzyControl_getFuzzyControlDelta
  (JNIEnv *env, jobject obj, jint size, jint t)
{
	TYPE k = (TYPE) t;
	return fuzzy->getFuzzyControlDelta(size, k);
}


JNIEXPORT jdouble JNICALL Java_control_JavaFuzzyControl_getFuzzyControlLambda
  (JNIEnv *env, jobject obj, jint size, jint t, jdouble linkload, jdouble linkMaxBW)
{
	TYPE k = (TYPE) t;
	return fuzzy->getFuzzyControlLambda(size, k, linkload, linkMaxBW);
}


/*
extern "C" double call_getFuzzyControlDelta(int size, int t){
	TYPE k = (TYPE) t;
	return fuzzy->getFuzzyControlDelta(size, k);
}

extern "C" double call_getFuzzyControlLamda(int size, int t, double linkload, double linkMaxBW){
	TYPE k = (TYPE) t;
	return fuzzy->getFuzzyControlLamda(size, k, linkload, linkMaxBW);
}

extern "C" void call_FuzzyControlDestroy(){
	fuzzy->~FuzzyControl();
}*/




