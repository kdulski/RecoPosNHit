/**
 *  @copyright Copyright 2017 The J-PET Framework Authors. All rights reserved.
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may find a copy of the License in the LICENCE file.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *  @file EventCategorizer.h
 */

#ifndef EVENTCATEGORIZER_H
#define EVENTCATEGORIZER_H

#include <JPetUserTask/JPetUserTask.h>
#include "EventCategorizerTools.h"
#include <JPetEvent/JPetEvent.h>
#include <JPetHit/JPetHit.h>
#include <JPetHitUtils/JPetHitUtils.h>
#include <vector>
#include <map>

class JPetWriter;

#ifdef __CINT__
#	define override
#endif

/**
 * @brief User Task categorizing Events
 *
 * Task attempts to add types of events to each event. Each category/type
 * has separate method for checking, if current event fulfills set of conditions.
 * These methods are defined in tools class. More than one type can be added to an event.
 * Set of controll histograms are created, unless the user decides not to produce them.
 */
class EventCategorizer : public JPetUserTask{
public:
	EventCategorizer(const char * name);
	virtual ~EventCategorizer(){}
	virtual bool init() override;
	virtual bool exec() override;
	virtual bool terminate() override;
	void PlotLifetimesForThresholds( JPetHit Hit1, JPetHit Hit2 );
	
	std::vector<double> TOTParamA;
	std::vector<double> TOTParamB;
	
	std::vector<double> TimeShiftSideA_thr1;
	std::vector<double> TimeShiftSideA_thr2;
	std::vector<double> TimeShiftSideA_thr3;
	std::vector<double> TimeShiftSideA_thr4;
	
	std::vector<double> TimeShiftSideB_thr1;
	std::vector<double> TimeShiftSideB_thr2;
	std::vector<double> TimeShiftSideB_thr3;
	std::vector<double> TimeShiftSideB_thr4;
	
	std::vector<double> PALSTDiff_thr1;
	std::vector<double> PALSTDiff_thr2;
	std::vector<double> PALSTDiff_thr3;
	std::vector<double> PALSTDiff_thr4;
	
	std::vector<double> PALSTDiff_thr1_iter2;
	std::vector<double> PALSTDiff_thr2_iter2;
	std::vector<double> PALSTDiff_thr3_iter2;
	std::vector<double> PALSTDiff_thr4_iter2;
	
	std::vector<double> TOTParamA_thr1;
	std::vector<double> TOTParamA_thr2;
	std::vector<double> TOTParamA_thr3;	
	std::vector<double> TOTParamA_thr4;	
	std::vector<double> TOTParamB_thr1;
	std::vector<double> TOTParamB_thr2;
	std::vector<double> TOTParamB_thr3;
	std::vector<double> TOTParamB_thr4;	
	
	void CalcTDiffAB( JPetHit Hit );
	void PlotTOTvsScatterAnglesForThresholds( const JPetHit& hit, double ScatterAngle );
	void calculateTOTforThresholdsMatched(const JPetHit& hit);	
protected:
	const std::string kScatterTOFTimeDiffParamKey = "Scatter_Categorizer_TOF_TimeDiff_float";
	const std::string kBack2BackSlotThetaDiffParamKey = "Back2Back_Categorizer_SlotThetaDiff_float";
	const std::string kDeexTOTCutMinParamKey = "Deex_Categorizer_TOT_Cut_Min_float";
	const std::string kDeexTOTCutMaxParamKey = "Deex_Categorizer_TOT_Cut_Max_float";
	const std::string kSaveControlHistosParamKey = "Save_Control_Histograms_bool";
	void saveEvents(const std::vector<JPetEvent>& event);
	double fScatterTOFTimeDiff = 2000.0;
	double fB2BSlotThetaDiff = 3.0;
	double fDeexTOTCutMin = 30000.0;
	double fDeexTOTCutMax = 50000.0;
	bool fSaveControlHistos = true;
	void initialiseHistograms();
};

TVector3 RecoPos2Hit( const JPetHit & Hit1, const JPetHit & Hit2);
void PlotLifetimesForThresholds( JPetHit Hit1, JPetHit Hit2 );
double NormalizeTimeToPoint( JPetHit Hit1, TVector3 Point );
double CalcEnergy( double TOT );
double CalcEnergy2( double TOT );

TVector3 RecoPosNHits( std::vector<JPetHit> Hits );
TVector3 FindIntersectionOfSpheres( std::vector<JPetHit> HitsSortedByAngle );
std::vector< std::vector<TVector3> > FindIntersectionPointsOfSpheres( std::vector<JPetHit> Hits, std::vector<double> Radiuses );
double CalcPhiAngle( JPetHit Hit );
std::vector < JPetHit > SortByAngle( std::vector < JPetHit > HitsToSort, std::vector<double> Angles );
double CalcDistanceBetweenHitPositions( JPetHit Hit1, JPetHit Hit2 );
double FindMinFromDerrivative( std::vector<double> Arg, std::vector<double> Val );
#endif /*  !EVENTCATEGORIZER_H */
