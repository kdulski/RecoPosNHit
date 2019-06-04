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
 *  @file EventCategorizer.cpp
 */

#include <JPetOptionsTools/JPetOptionsTools.h>
#include <JPetWriter/JPetWriter.h>
#include "EventCategorizerTools.h"
#include "EventCategorizer.h"
#include <iostream>

using namespace jpet_options_tools;
using namespace std;

EventCategorizer::EventCategorizer(const char* name): JPetUserTask(name) {}

bool EventCategorizer::init()
{
	INFO("Event categorization started.");
	// Parameter for back to back categorization
	if (isOptionSet(fParams.getOptions(), kBack2BackSlotThetaDiffParamKey))
		fB2BSlotThetaDiff = getOptionAsFloat(fParams.getOptions(), kBack2BackSlotThetaDiffParamKey);
	else
		WARNING(Form("No value of the %s parameter provided by the user. Using default value of %lf.",
		  kBack2BackSlotThetaDiffParamKey.c_str(), fB2BSlotThetaDiff));
	// Parameter for scattering determination
	if (isOptionSet(fParams.getOptions(), kScatterTOFTimeDiffParamKey))
		fScatterTOFTimeDiff = getOptionAsFloat(fParams.getOptions(), kScatterTOFTimeDiffParamKey);
	else
		WARNING(Form("No value of the %s parameter provided by the user. Using default value of %lf.",
		  kScatterTOFTimeDiffParamKey.c_str(), fScatterTOFTimeDiff));
	// Parameters for deexcitation TOT cut
	if (isOptionSet(fParams.getOptions(), kDeexTOTCutMinParamKey))
		  fDeexTOTCutMin = getOptionAsFloat(fParams.getOptions(), kDeexTOTCutMinParamKey);
	else
		  WARNING(Form("No value of the %s parameter provided by the user. Using default value of %lf.",
		    kDeexTOTCutMinParamKey.c_str(), fDeexTOTCutMin));
	if (isOptionSet(fParams.getOptions(), kDeexTOTCutMaxParamKey))
		  fDeexTOTCutMax = getOptionAsFloat(fParams.getOptions(), kDeexTOTCutMaxParamKey);
	else
		  WARNING(Form("No value of the %s parameter provided by the user. Using default value of %lf.",
		    kDeexTOTCutMaxParamKey.c_str(), fDeexTOTCutMax));
	// Getting bool for saving histograms
	if (isOptionSet(fParams.getOptions(), kSaveControlHistosParamKey))
		  fSaveControlHistos = getOptionAsBool(fParams.getOptions(), kSaveControlHistosParamKey);

	// Input events type
	fOutputEvents = new JPetTimeWindow("JPetEvent");
	// Initialise hisotgrams
	if(fSaveControlHistos) initialiseHistograms();
	
	//std::ifstream TOTPar;
	//TOTPar.open( "Parameters_TOT.dat" );
	double count, count2, count3, count4;
	/*while( TOTPar >> count >> count2 )
	{
		TOTParamA.push_back(count);
		TOTParamB.push_back(count2);
	}
        TOTPar.close();*/
	
	std::ifstream TDiffAB_thr1;
	TDiffAB_thr1.open( "Parameters_TimeDiff_BA_thr1_withUnc_07_03.dat" );
	while( TDiffAB_thr1 >> count >> count2 >> count3 >> count4 )
	{
		TimeShiftSideA_thr1.push_back(count);
		TimeShiftSideB_thr1.push_back(count3);
	}
        TDiffAB_thr1.close();
	
	std::ifstream TDiffAB_thr2;
	TDiffAB_thr2.open( "Parameters_TimeDiff_BA_thr2_withUnc_07_03.dat" );
	while( TDiffAB_thr2 >> count >> count2 >> count3 >> count4 )
	{
		TimeShiftSideA_thr2.push_back(count);
		TimeShiftSideB_thr2.push_back(count3);
	}
        TDiffAB_thr2.close();
	
	std::ifstream TDiffAB_thr3;
	TDiffAB_thr3.open( "Parameters_TimeDiff_BA_thr3_withUnc_07_03.dat" );
	while( TDiffAB_thr3 >> count >> count2 >> count3 >> count4 )
	{
		TimeShiftSideA_thr3.push_back(count);
		TimeShiftSideB_thr3.push_back(count3);
	}
        TDiffAB_thr3.close();
	
	std::ifstream TDiffAB_thr4;
	TDiffAB_thr4.open( "Parameters_TimeDiff_BA_thr4_withUnc_07_03.dat" );
	while( TDiffAB_thr4 >> count >> count2 >> count3 >> count4 )
	{
		TimeShiftSideA_thr4.push_back(count);
		TimeShiftSideB_thr4.push_back(count3);
	}
        TDiffAB_thr4.close();
	
	std::ifstream PALST_thr1;
	PALST_thr1.open( "Parameters_PALSTimeDiff_thr1_Unc_imp_07_03.dat" );
	while( PALST_thr1 >> count >> count2 )
	{
		PALSTDiff_thr1.push_back(count);
	}
        PALST_thr1.close();
	
	std::ifstream PALST_thr2;
	PALST_thr2.open( "Parameters_PALSTimeDiff_thr2_Unc_imp_07_03.dat" );
	while( PALST_thr2 >> count >> count2 )
	{
		PALSTDiff_thr2.push_back(count);
	}
        PALST_thr2.close();
	
	std::ifstream PALST_thr3;
	PALST_thr3.open( "Parameters_PALSTimeDiff_thr3_Unc_imp_07_03.dat" );
	while( PALST_thr3 >> count >> count2 )
	{
		PALSTDiff_thr3.push_back(count);
	}
        PALST_thr3.close();
	
	std::ifstream PALST_thr4;
	PALST_thr4.open( "Parameters_PALSTimeDiff_thr4_Unc_imp_07_03.dat" );
	while( PALST_thr4 >> count >> count2 )
	{
		PALSTDiff_thr4.push_back(count);
	}
        PALST_thr4.close();
	
	std::ifstream PALST_thr1_it2;
	PALST_thr1_it2.open( "Parameters_PALSTimeDiff_thr1_Unc_imp_07_03_iter2.dat" );
	while( PALST_thr1_it2 >> count >> count2 )
	{
		PALSTDiff_thr1_iter2.push_back(count);
	}
        PALST_thr1_it2.close();
	
	std::ifstream PALST_thr2_it2;
	PALST_thr2_it2.open( "Parameters_PALSTimeDiff_thr2_Unc_imp_07_03_iter2.dat" );
	while( PALST_thr2_it2 >> count >> count2 )
	{
		PALSTDiff_thr2_iter2.push_back(count);
	}
        PALST_thr2_it2.close();
	
	std::ifstream PALST_thr3_it2;
	PALST_thr3_it2.open( "Parameters_PALSTimeDiff_thr3_Unc_imp_07_03_iter2.dat" );
	while( PALST_thr3_it2 >> count >> count2 )
	{
		PALSTDiff_thr3_iter2.push_back(count);
	}
        PALST_thr3_it2.close();
	
	std::ifstream PALST_thr4_it2;
	PALST_thr4_it2.open( "Parameters_PALSTimeDiff_thr4_Unc_imp_07_03_iter2.dat" );
	while( PALST_thr4_it2 >> count >> count2 )
	{
		PALSTDiff_thr4_iter2.push_back(count);
	}
        PALST_thr4_it2.close();
	
	/*std::ifstream TOT_sideA_thr1;
	TOT_sideA_thr1.open( "Parameters_TOT_sideA_thr1.dat" );
	while( TOT_sideA_thr1 >> count  )
	{
		TOTParamA_thr1.push_back(count);
	}
        TOT_sideA_thr1.close();
	
	std::ifstream TOT_sideA_thr2;
	TOT_sideA_thr2.open( "Parameters_TOT_sideA_thr2.dat" );
	while( TOT_sideA_thr2 >> count  )
	{
		TOTParamA_thr2.push_back(count);
	}
        TOT_sideA_thr2.close();

	std::ifstream TOT_sideA_thr3;
	TOT_sideA_thr3.open( "Parameters_TOT_sideA_thr3.dat" );
	while( TOT_sideA_thr3 >> count  )
	{
		TOTParamA_thr3.push_back(count);
	}
        TOT_sideA_thr3.close();

	std::ifstream TOT_sideA_thr4;
	TOT_sideA_thr4.open( "Parameters_TOT_sideA_thr4.dat" );
	while( TOT_sideA_thr4 >> count  )
	{
		TOTParamA_thr4.push_back(count);
	}
        TOT_sideA_thr4.close();	
	std::ifstream TOT_sideB_thr1;
	TOT_sideB_thr1.open( "Parameters_TOT_sideB_thr1.dat" );
	while( TOT_sideB_thr1 >> count  )
	{
		TOTParamB_thr1.push_back(count);
	}
        TOT_sideB_thr1.close();
	
	std::ifstream TOT_sideB_thr2;
	TOT_sideB_thr2.open( "Parameters_TOT_sideB_thr2.dat" );
	while( TOT_sideB_thr2 >> count  )
	{
		TOTParamB_thr2.push_back(count);
	}
        TOT_sideB_thr2.close();

	std::ifstream TOT_sideB_thr3;
	TOT_sideB_thr3.open( "Parameters_TOT_sideB_thr3.dat" );
	while( TOT_sideB_thr3 >> count  )
	{
		TOTParamB_thr3.push_back(count);
	}
        TOT_sideB_thr3.close();

	std::ifstream TOT_sideB_thr4;
	TOT_sideB_thr4.open( "Parameters_TOT_sideB_thr4.dat" );
	while( TOT_sideB_thr4 >> count  )
	{
		TOTParamB_thr4.push_back(count);
	}
        TOT_sideB_thr4.close();	*/	
	
	return true;
}

bool EventCategorizer::exec()
{
	JPetHit HitToReco1, HitToReco2, HitToReco3, HitToReco4;
	HitToReco1.setPosX( 50 );
	HitToReco1.setPosY( 0 );
	HitToReco1.setPosZ( 2 );
	HitToReco1.setTime( 1500 );
	
	HitToReco2.setPosX( 0 );
	HitToReco2.setPosY( 50 );
	HitToReco2.setPosZ( 3 );
	HitToReco2.setTime( 1530 );	
	
	HitToReco3.setPosX( 0 );
	HitToReco3.setPosY( -50 );
	HitToReco3.setPosZ( -3 );
	HitToReco3.setTime( 1450 );	
	
	HitToReco4.setPosX( -50 );
	HitToReco4.setPosY( 0 );
	HitToReco4.setPosZ( 1 );
	HitToReco4.setTime( 1510 );

	std::vector<JPetHit> Hits;
	Hits.push_back( HitToReco1 );
	Hits.push_back( HitToReco2 );
	Hits.push_back( HitToReco3 );
	Hits.push_back( HitToReco4 );
  
	TVector3 Pos =  RecoPosNHits( Hits );
	
	if (auto timeWindow = dynamic_cast<const JPetTimeWindow* const>(fEvent)) 
	{
		vector<JPetEvent> events;
		for (uint i = 0; i < timeWindow->getNumberOfEvents(); i++) 
		{
			const auto& event = dynamic_cast<const JPetEvent&>(timeWindow->operator[](i));

			// Check types of current event
			/*bool is2Gamma = EventCategorizerTools::checkFor2Gamma(event, getStatistics(), fSaveControlHistos, fB2BSlotThetaDiff);
			bool is3Gamma = EventCategorizerTools::checkFor3Gamma(event, getStatistics(), fSaveControlHistos);
			bool isPrompt = EventCategorizerTools::checkForPrompt(event, getStatistics(), fSaveControlHistos, fDeexTOTCutMin, fDeexTOTCutMax);
			bool isScattered = EventCategorizerTools::checkForScatter(event, getStatistics(), fSaveControlHistos, fScatterTOFTimeDiff);

			JPetEvent newEvent = event;
			if(is2Gamma) newEvent.addEventType(JPetEventType::k2Gamma);
			if(is3Gamma) newEvent.addEventType(JPetEventType::k3Gamma);
			if(isPrompt) newEvent.addEventType(JPetEventType::kPrompt);
			if(isScattered) newEvent.addEventType(JPetEventType::kScattered);*/
			
			vector<JPetHit> AnnihilationCandidates, DeexcitationCandidates;
			
			if(fSaveControlHistos)
			{
				for(auto hit : event.getHits())
				{
					//getStatistics().getHisto2D("All_XYpos")->Fill(hit.getPosX(), hit.getPosY());
					int ScintID = hit.getScintillator().getID();
					double TOTofTheHit = EventCategorizerTools::calculateTOT( hit )/1000;
					double TOT_new = TOTofTheHit;//*20/( TOTParamB[ScintID-1] - TOTParamA[ScintID-1] ) + 30 - TOTParamA[ScintID-1]*20/( TOTParamB[ScintID-1] - TOTParamA[ScintID-1] );
					getStatistics().getHisto2D("TOT_vs_ID_matched")->Fill( TOT_new, ScintID );
					getStatistics().getHisto2D("AllHit_Z_vs_ID")->Fill( hit.getPosZ(), ScintID );
					CalcTDiffAB( hit );					
					//calculateTOTforThresholdsMatched( hit );
					
					std::vector<JPetSigCh> sigALead = hit.getSignalA().getRecoSignal().getRawSignal().getPoints(JPetSigCh::Leading, JPetRawSignal::ByThrNum);
					std::vector<JPetSigCh> sigBLead = hit.getSignalB().getRecoSignal().getRawSignal().getPoints(JPetSigCh::Leading, JPetRawSignal::ByThrNum);
					if (sigALead.size() > 0 && sigBLead.size() > 0)
					{
						double TimeDiffOn1Thr = (sigBLead.at(0).getValue() - sigALead.at(0).getValue())/1000;
						double CorrectedZ = ( TimeDiffOn1Thr - ( TimeShiftSideB_thr1[ScintID-1] + TimeShiftSideA_thr1[ScintID-1] )/2 )*
										    48 /*scintillation length - mean*/ /( TimeShiftSideB_thr1[ScintID-1] - TimeShiftSideA_thr1[ScintID-1] );
						hit.setPosZ( CorrectedZ );
						getStatistics().getHisto2D("AllHit_Z_vs_ID_corr")->Fill( hit.getPosZ(), ScintID );
						/*double TempTime = hit.getTime();
						hit.setTime( TempTime - (TimeShiftSideB_thr1[ ScintID - 1 ] + TimeShiftSideA_thr1[ ScintID - 1 ])/4 );
						getStatistics().getHisto2D("AllHit_TimeDiff_vs_ID")->Fill( hit.getTime(), ScintID );*/
					}					
					if( 20 < TOTofTheHit && TOTofTheHit < 30 && fabs( hit.getPosZ() ) < 23 )
					{
						AnnihilationCandidates.push_back( hit );
					}
					else if( 35 < TOTofTheHit && TOTofTheHit < 50 && fabs( hit.getPosZ() ) < 23 )
					{
						DeexcitationCandidates.push_back( hit );
					}					
				}
			}
			
			if( event.getHits().size() == 2 )
			{
				JPetHit primaryHit, scatterHit;
			  
				primaryHit = event.getHits().at(0);
				scatterHit = event.getHits().at(1);			  
				double Theta = (180/3.1415)* acos( ( primaryHit.getPosX() * scatterHit.getPosX() + primaryHit.getPosY() * scatterHit.getPosY() ) / 
									  ( sqrt( primaryHit.getPosX()*primaryHit.getPosX() + primaryHit.getPosY()*primaryHit.getPosY() ) 
									  * sqrt( scatterHit.getPosX()*scatterHit.getPosX() + scatterHit.getPosY()*scatterHit.getPosY() ) ) );
				if( Theta > 177 && Theta < 183 )
				{
					getStatistics().getHisto2D("TOF_vs_ID")->Fill( fabs( primaryHit.getTime() - scatterHit.getTime() )/1000, primaryHit.getScintillator().getID() );
					getStatistics().getHisto2D("TOF_vs_ID")->Fill( - fabs( primaryHit.getTime() - scatterHit.getTime() )/1000, scatterHit.getScintillator().getID() );
				}
				/*JPetHit primaryHit, scatterHit;

				double TimeDiff = ( scatterHit.getTime() - primaryHit.getTime() )/1000;
				
				double CorrIter1 = (PALSTDiff_thr1[ scatterHit.getScintillator().getID() - 1 ] - PALSTDiff_thr1[ primaryHit.getScintillator().getID() - 1 ])/2;
				double CorrIter2 = (PALSTDiff_thr1_iter2[ scatterHit.getScintillator().getID() - 1 ] - PALSTDiff_thr1_iter2[ primaryHit.getScintillator().getID() - 1 ])/2;
				double Corr = CorrIter1 + CorrIter2;
				
				if( fabs( TimeDiff - Corr - EventCategorizerTools::calculateScatteringTime( primaryHit, scatterHit )/1000 ) < 0.4 )
				{
					PlotTOTvsScatterAnglesForThresholds( primaryHit, EventCategorizerTools::calculateScatteringAngle( primaryHit, scatterHit ) );
					double TOTofTheHit = EventCategorizerTools::calculateTOT( primaryHit )/1000;
					getStatistics().getHisto2D("TOT_vs_ScatterAngle_notmatched")->Fill( TOTofTheHit, EventCategorizerTools::calculateScatteringAngle( primaryHit, scatterHit ) );
					int ScintID = primaryHit.getScintillator().getID();
					double TOT_new = TOTofTheHit*20/( TOTParamB[ScintID-1] - TOTParamA[ScintID-1] ) + 30 - TOTParamA[ScintID-1]*20/( TOTParamB[ScintID-1] - TOTParamA[ScintID-1] );
					getStatistics().getHisto2D("TOT_vs_ScatterAngle")->Fill( TOT_new, EventCategorizerTools::calculateScatteringAngle( primaryHit, scatterHit ) );
				}*/
			}
			
			if( AnnihilationCandidates.size() == 1 && DeexcitationCandidates.size() == 1 )
			{
				PlotLifetimesForThresholds( AnnihilationCandidates[0], DeexcitationCandidates[0] );
			}
			else if( AnnihilationCandidates.size() == 2 && DeexcitationCandidates.size() == 1 )
			{
				TVector3 XYZ = RecoPos2Hit( AnnihilationCandidates[0], AnnihilationCandidates[1] );
				getStatistics().getHisto2D("3Hit_XY")->Fill( XYZ(1), XYZ(0) );
				getStatistics().getHisto1D("3Hit_Z")->Fill( XYZ(2) );
				getStatistics().getHisto1D("3Hit_Lifetime")->Fill( ( NormalizeTimeToPoint( AnnihilationCandidates[0], XYZ ) + NormalizeTimeToPoint( AnnihilationCandidates[1], XYZ ) )/2 - NormalizeTimeToPoint( DeexcitationCandidates[0], XYZ ) );
			}
			  
			  
			//events.push_back(newEvent);
		}
		//saveEvents(events);
	} 
	else 
		return false;
	return true;
}

bool EventCategorizer::terminate()
{
	INFO("Event categorization completed.");
	return true;
}

void EventCategorizer::saveEvents(const vector<JPetEvent>& events)
{
	for (const auto& event : events) 
		fOutputEvents->add<JPetEvent>(event);
}

void EventCategorizer::initialiseHistograms()
{
  
	getStatistics().createHistogram( new TH2F("PALS_vs_AnnihilationID_thr1", "", 4000, -199.55, 200.45, 200, 0 , 200 ) );
	getStatistics().createHistogram( new TH2F("PALS_vs_AnnihilationID_thr2", "", 4000, -199.55, 200.45, 200, 0 , 200 ) );
	getStatistics().createHistogram( new TH2F("PALS_vs_AnnihilationID_thr3", "", 4000, -199.55, 200.45, 200, 0 , 200 ) );
	getStatistics().createHistogram( new TH2F("PALS_vs_AnnihilationID_thr4", "", 4000, -199.55, 200.45, 200, 0 , 200 ) );
	getStatistics().getHisto2D("PALS_vs_AnnihilationID_thr1")->GetXaxis()->SetTitle("Positronium lifetime for threshold 1 [ns]");
	getStatistics().getHisto2D("PALS_vs_AnnihilationID_thr2")->GetXaxis()->SetTitle("Positronium lifetime for threshold 2 [ns]");
	getStatistics().getHisto2D("PALS_vs_AnnihilationID_thr3")->GetXaxis()->SetTitle("Positronium lifetime for threshold 3 [ns]");
	getStatistics().getHisto2D("PALS_vs_AnnihilationID_thr4")->GetXaxis()->SetTitle("Positronium lifetime for threshold 4 [ns]");
	getStatistics().getHisto2D("PALS_vs_AnnihilationID_thr1")->GetYaxis()->SetTitle("ID of the scintillator with Annihilation Hit");
	getStatistics().getHisto2D("PALS_vs_AnnihilationID_thr2")->GetYaxis()->SetTitle("ID of the scintillator with Annihilation Hit");
	getStatistics().getHisto2D("PALS_vs_AnnihilationID_thr3")->GetYaxis()->SetTitle("ID of the scintillator with Annihilation Hit");
	getStatistics().getHisto2D("PALS_vs_AnnihilationID_thr4")->GetYaxis()->SetTitle("ID of the scintillator with Annihilation Hit");
	
	getStatistics().createHistogram( new TH2F("PALS_vs_DeexcitationID_thr1", "", 4000, -199.55, 200.45, 200, 0 , 200 ) );
	getStatistics().createHistogram( new TH2F("PALS_vs_DeexcitationID_thr2", "", 4000, -199.55, 200.45, 200, 0 , 200 ) );
	getStatistics().createHistogram( new TH2F("PALS_vs_DeexcitationID_thr3", "", 4000, -199.55, 200.45, 200, 0 , 200 ) );
	getStatistics().createHistogram( new TH2F("PALS_vs_DeexcitationID_thr4", "", 4000, -199.55, 200.45, 200, 0 , 200 ) );
	getStatistics().getHisto2D("PALS_vs_DeexcitationID_thr1")->GetXaxis()->SetTitle("Positronium lifetime for threshold 1 [ns]");
	getStatistics().getHisto2D("PALS_vs_DeexcitationID_thr2")->GetXaxis()->SetTitle("Positronium lifetime for threshold 2 [ns]");
	getStatistics().getHisto2D("PALS_vs_DeexcitationID_thr3")->GetXaxis()->SetTitle("Positronium lifetime for threshold 3 [ns]");
	getStatistics().getHisto2D("PALS_vs_DeexcitationID_thr4")->GetXaxis()->SetTitle("Positronium lifetime for threshold 4 [ns]");
	getStatistics().getHisto2D("PALS_vs_DeexcitationID_thr1")->GetYaxis()->SetTitle("ID of the scintillator with Annihilation Hit");
	getStatistics().getHisto2D("PALS_vs_DeexcitationID_thr2")->GetYaxis()->SetTitle("ID of the scintillator with Annihilation Hit");
	getStatistics().getHisto2D("PALS_vs_DeexcitationID_thr3")->GetYaxis()->SetTitle("ID of the scintillator with Annihilation Hit");
	getStatistics().getHisto2D("PALS_vs_DeexcitationID_thr4")->GetYaxis()->SetTitle("ID of the scintillator with Annihilation Hit");
	
	getStatistics().createHistogram( new TH2F("3Hit_XY", "", 1000, -49.55, 50.45, 1000, -49.55, 50.45) );
	getStatistics().createHistogram( new TH1F("3Hit_Z", "", 1000, -49.55, 50.45 ) );
	getStatistics().getHisto2D("3Hit_XY")->GetXaxis()->SetTitle("Y [cm]");
	getStatistics().getHisto2D("3Hit_XY")->GetYaxis()->SetTitle("X [cm]");
	getStatistics().getHisto1D("3Hit_Z")->GetXaxis()->SetTitle("Z [cm]");
	getStatistics().getHisto1D("3Hit_Z")->GetYaxis()->SetTitle("Counts");
	getStatistics().createHistogram( new TH1F("3Hit_Lifetime", "", 4000, -199.55, 200.45 ) );
	getStatistics().getHisto1D("3Hit_Lifetime")->GetXaxis()->SetTitle("Positronium Lifetime [ns]");
	getStatistics().getHisto1D("3Hit_Lifetime")->GetYaxis()->SetTitle("Counts");
  
	getStatistics().createHistogram( new TH2F("TOT_vs_ID_matched", "", 1200, -19.5, 100.5, 200, 0 , 200 ) );
	getStatistics().createHistogram( new TH2F("AllHit_Z_vs_ID", "", 1000, -49.55, 50.45, 200, 0 , 200 ) );
	getStatistics().getHisto2D("AllHit_Z_vs_ID")->GetXaxis()->SetTitle("Z [cm]");
	getStatistics().getHisto2D("AllHit_Z_vs_ID")->GetYaxis()->SetTitle("Id of scintillator");	
	
	getStatistics().createHistogram( new TH2F("AllHit_Z_vs_ID_corr", "", 1000, -49.55, 50.45, 200, 0 , 200 ) );
	getStatistics().getHisto2D("AllHit_Z_vs_ID_corr")->GetXaxis()->SetTitle("Z [cm]");
	getStatistics().getHisto2D("AllHit_Z_vs_ID_corr")->GetYaxis()->SetTitle("Id of scintillator");

	getStatistics().createHistogram( new TH2F("TDiff_AB_vs_ID_thr1_nocorr", "", 411, -20.5, 20.5, 200, 0 , 200 ) );
	getStatistics().createHistogram( new TH2F("TDiff_AB_vs_ID_thr2_nocorr", "", 411, -20.5, 20.5, 200, 0 , 200 ) );
	getStatistics().createHistogram( new TH2F("TDiff_AB_vs_ID_thr3_nocorr", "", 411, -20.5, 20.5, 200, 0 , 200 ) );
	getStatistics().createHistogram( new TH2F("TDiff_AB_vs_ID_thr4_nocorr", "", 411, -20.5, 20.5, 200, 0 , 200 ) );
	getStatistics().getHisto2D("TDiff_AB_vs_ID_thr1_nocorr")->GetXaxis()->SetTitle("Time difference AB for threshold 1 [ns]");
	getStatistics().getHisto2D("TDiff_AB_vs_ID_thr2_nocorr")->GetXaxis()->SetTitle("Time difference AB for threshold 2 [ns]");
	getStatistics().getHisto2D("TDiff_AB_vs_ID_thr3_nocorr")->GetXaxis()->SetTitle("Time difference AB for threshold 3 [ns]");
	getStatistics().getHisto2D("TDiff_AB_vs_ID_thr4_nocorr")->GetXaxis()->SetTitle("Time difference AB for threshold 4 [ns]");
	getStatistics().getHisto2D("TDiff_AB_vs_ID_thr1_nocorr")->GetYaxis()->SetTitle("ID of the scintillator");
	getStatistics().getHisto2D("TDiff_AB_vs_ID_thr2_nocorr")->GetYaxis()->SetTitle("ID of the scintillator");
	getStatistics().getHisto2D("TDiff_AB_vs_ID_thr3_nocorr")->GetYaxis()->SetTitle("ID of the scintillator");
	getStatistics().getHisto2D("TDiff_AB_vs_ID_thr4_nocorr")->GetYaxis()->SetTitle("ID of the scintillator");		
	
	getStatistics().createHistogram( new TH2F("TDiff_AB_vs_ID_thr1_corr", "", 411, -20.5, 20.5, 200, 0 , 200 ) );
	getStatistics().createHistogram( new TH2F("TDiff_AB_vs_ID_thr2_corr", "", 411, -20.5, 20.5, 200, 0 , 200 ) );
	getStatistics().createHistogram( new TH2F("TDiff_AB_vs_ID_thr3_corr", "", 411, -20.5, 20.5, 200, 0 , 200 ) );
	getStatistics().createHistogram( new TH2F("TDiff_AB_vs_ID_thr4_corr", "", 411, -20.5, 20.5, 200, 0 , 200 ) );
	getStatistics().getHisto2D("TDiff_AB_vs_ID_thr1_corr")->GetXaxis()->SetTitle("Time difference AB for threshold 1 [ns]");
	getStatistics().getHisto2D("TDiff_AB_vs_ID_thr2_corr")->GetXaxis()->SetTitle("Time difference AB for threshold 2 [ns]");
	getStatistics().getHisto2D("TDiff_AB_vs_ID_thr3_corr")->GetXaxis()->SetTitle("Time difference AB for threshold 3 [ns]");
	getStatistics().getHisto2D("TDiff_AB_vs_ID_thr4_corr")->GetXaxis()->SetTitle("Time difference AB for threshold 4 [ns]");
	getStatistics().getHisto2D("TDiff_AB_vs_ID_thr1_corr")->GetYaxis()->SetTitle("ID of the scintillator");
	getStatistics().getHisto2D("TDiff_AB_vs_ID_thr2_corr")->GetYaxis()->SetTitle("ID of the scintillator");
	getStatistics().getHisto2D("TDiff_AB_vs_ID_thr3_corr")->GetYaxis()->SetTitle("ID of the scintillator");
	getStatistics().getHisto2D("TDiff_AB_vs_ID_thr4_corr")->GetYaxis()->SetTitle("ID of the scintillator");	
	
	getStatistics().createHistogram( new TH2F("PALS_vs_AnnihilationID_thr1_iter1", "", 4000, -199.55, 200.45, 200, 0 , 200 ) );
	getStatistics().createHistogram( new TH2F("PALS_vs_AnnihilationID_thr2_iter1", "", 4000, -199.55, 200.45, 200, 0 , 200 ) );
	getStatistics().createHistogram( new TH2F("PALS_vs_AnnihilationID_thr3_iter1", "", 4000, -199.55, 200.45, 200, 0 , 200 ) );
	getStatistics().createHistogram( new TH2F("PALS_vs_AnnihilationID_thr4_iter1", "", 4000, -199.55, 200.45, 200, 0 , 200 ) );
	getStatistics().getHisto2D("PALS_vs_AnnihilationID_thr1_iter1")->GetXaxis()->SetTitle("Positronium lifetime for threshold 1 [ns]");
	getStatistics().getHisto2D("PALS_vs_AnnihilationID_thr2_iter1")->GetXaxis()->SetTitle("Positronium lifetime for threshold 2 [ns]");
	getStatistics().getHisto2D("PALS_vs_AnnihilationID_thr3_iter1")->GetXaxis()->SetTitle("Positronium lifetime for threshold 3 [ns]");
	getStatistics().getHisto2D("PALS_vs_AnnihilationID_thr4_iter1")->GetXaxis()->SetTitle("Positronium lifetime for threshold 4 [ns]");
	getStatistics().getHisto2D("PALS_vs_AnnihilationID_thr1_iter1")->GetYaxis()->SetTitle("ID of the scintillator with Annihilation Hit");
	getStatistics().getHisto2D("PALS_vs_AnnihilationID_thr2_iter1")->GetYaxis()->SetTitle("ID of the scintillator with Annihilation Hit");
	getStatistics().getHisto2D("PALS_vs_AnnihilationID_thr3_iter1")->GetYaxis()->SetTitle("ID of the scintillator with Annihilation Hit");
	getStatistics().getHisto2D("PALS_vs_AnnihilationID_thr4_iter1")->GetYaxis()->SetTitle("ID of the scintillator with Annihilation Hit");
	
	getStatistics().createHistogram( new TH2F("PALS_vs_DeexcitationID_thr1_iter1", "", 4000, -199.55, 200.45, 200, 0 , 200 ) );
	getStatistics().createHistogram( new TH2F("PALS_vs_DeexcitationID_thr2_iter1", "", 4000, -199.55, 200.45, 200, 0 , 200 ) );
	getStatistics().createHistogram( new TH2F("PALS_vs_DeexcitationID_thr3_iter1", "", 4000, -199.55, 200.45, 200, 0 , 200 ) );
	getStatistics().createHistogram( new TH2F("PALS_vs_DeexcitationID_thr4_iter1", "", 4000, -199.55, 200.45, 200, 0 , 200 ) );
	getStatistics().getHisto2D("PALS_vs_DeexcitationID_thr1_iter1")->GetXaxis()->SetTitle("Positronium lifetime for threshold 1 [ns]");
	getStatistics().getHisto2D("PALS_vs_DeexcitationID_thr2_iter1")->GetXaxis()->SetTitle("Positronium lifetime for threshold 2 [ns]");
	getStatistics().getHisto2D("PALS_vs_DeexcitationID_thr3_iter1")->GetXaxis()->SetTitle("Positronium lifetime for threshold 3 [ns]");
	getStatistics().getHisto2D("PALS_vs_DeexcitationID_thr4_iter1")->GetXaxis()->SetTitle("Positronium lifetime for threshold 4 [ns]");
	getStatistics().getHisto2D("PALS_vs_DeexcitationID_thr1_iter1")->GetYaxis()->SetTitle("ID of the scintillator with Annihilation Hit");
	getStatistics().getHisto2D("PALS_vs_DeexcitationID_thr2_iter1")->GetYaxis()->SetTitle("ID of the scintillator with Annihilation Hit");
	getStatistics().getHisto2D("PALS_vs_DeexcitationID_thr3_iter1")->GetYaxis()->SetTitle("ID of the scintillator with Annihilation Hit");
	getStatistics().getHisto2D("PALS_vs_DeexcitationID_thr4_iter1")->GetYaxis()->SetTitle("ID of the scintillator with Annihilation Hit");
	
	getStatistics().createHistogram( new TH2F("PALS_vs_AnnihilationID_thr1_iter2", "", 4000, -199.55, 200.45, 200, 0 , 200 ) );
	getStatistics().createHistogram( new TH2F("PALS_vs_AnnihilationID_thr2_iter2", "", 4000, -199.55, 200.45, 200, 0 , 200 ) );
	getStatistics().createHistogram( new TH2F("PALS_vs_AnnihilationID_thr3_iter2", "", 4000, -199.55, 200.45, 200, 0 , 200 ) );
	getStatistics().createHistogram( new TH2F("PALS_vs_AnnihilationID_thr4_iter2", "", 4000, -199.55, 200.45, 200, 0 , 200 ) );
	getStatistics().getHisto2D("PALS_vs_AnnihilationID_thr1_iter2")->GetXaxis()->SetTitle("Positronium lifetime for threshold 1 [ns]");
	getStatistics().getHisto2D("PALS_vs_AnnihilationID_thr2_iter2")->GetXaxis()->SetTitle("Positronium lifetime for threshold 2 [ns]");
	getStatistics().getHisto2D("PALS_vs_AnnihilationID_thr3_iter2")->GetXaxis()->SetTitle("Positronium lifetime for threshold 3 [ns]");
	getStatistics().getHisto2D("PALS_vs_AnnihilationID_thr4_iter2")->GetXaxis()->SetTitle("Positronium lifetime for threshold 4 [ns]");
	getStatistics().getHisto2D("PALS_vs_AnnihilationID_thr1_iter2")->GetYaxis()->SetTitle("ID of the scintillator with Annihilation Hit");
	getStatistics().getHisto2D("PALS_vs_AnnihilationID_thr2_iter2")->GetYaxis()->SetTitle("ID of the scintillator with Annihilation Hit");
	getStatistics().getHisto2D("PALS_vs_AnnihilationID_thr3_iter2")->GetYaxis()->SetTitle("ID of the scintillator with Annihilation Hit");
	getStatistics().getHisto2D("PALS_vs_AnnihilationID_thr4_iter2")->GetYaxis()->SetTitle("ID of the scintillator with Annihilation Hit");
	
	getStatistics().createHistogram( new TH2F("PALS_vs_DeexcitationID_thr1_iter2", "", 4000, -199.55, 200.45, 200, 0 , 200 ) );
	getStatistics().createHistogram( new TH2F("PALS_vs_DeexcitationID_thr2_iter2", "", 4000, -199.55, 200.45, 200, 0 , 200 ) );
	getStatistics().createHistogram( new TH2F("PALS_vs_DeexcitationID_thr3_iter2", "", 4000, -199.55, 200.45, 200, 0 , 200 ) );
	getStatistics().createHistogram( new TH2F("PALS_vs_DeexcitationID_thr4_iter2", "", 4000, -199.55, 200.45, 200, 0 , 200 ) );
	getStatistics().getHisto2D("PALS_vs_DeexcitationID_thr1_iter2")->GetXaxis()->SetTitle("Positronium lifetime for threshold 1 [ns]");
	getStatistics().getHisto2D("PALS_vs_DeexcitationID_thr2_iter2")->GetXaxis()->SetTitle("Positronium lifetime for threshold 2 [ns]");
	getStatistics().getHisto2D("PALS_vs_DeexcitationID_thr3_iter2")->GetXaxis()->SetTitle("Positronium lifetime for threshold 3 [ns]");
	getStatistics().getHisto2D("PALS_vs_DeexcitationID_thr4_iter2")->GetXaxis()->SetTitle("Positronium lifetime for threshold 4 [ns]");
	getStatistics().getHisto2D("PALS_vs_DeexcitationID_thr1_iter2")->GetYaxis()->SetTitle("ID of the scintillator with Annihilation Hit");
	getStatistics().getHisto2D("PALS_vs_DeexcitationID_thr2_iter2")->GetYaxis()->SetTitle("ID of the scintillator with Annihilation Hit");
	getStatistics().getHisto2D("PALS_vs_DeexcitationID_thr3_iter2")->GetYaxis()->SetTitle("ID of the scintillator with Annihilation Hit");
	getStatistics().getHisto2D("PALS_vs_DeexcitationID_thr4_iter2")->GetYaxis()->SetTitle("ID of the scintillator with Annihilation Hit");
	
		getStatistics().createHistogram( new TH2F("TOT_vs_ScatterAngle_thr1", "", 400, -0.5, 39.5, 200, 0 , 200 ) );
		getStatistics().createHistogram( new TH2F("TOT_vs_ScatterAngle_thr2", "", 400, -0.5, 39.5, 200, 0 , 200 ) );
		getStatistics().createHistogram( new TH2F("TOT_vs_ScatterAngle_thr3", "", 400, -0.5, 39.5, 200, 0 , 200 ) );
		getStatistics().createHistogram( new TH2F("TOT_vs_ScatterAngle_thr4", "", 400, -0.5, 39.5, 200, 0 , 200 ) );
		getStatistics().getHisto2D("TOT_vs_ScatterAngle_thr1")->GetXaxis()->SetTitle("TOT [ns]");
		getStatistics().getHisto2D("TOT_vs_ScatterAngle_thr2")->GetXaxis()->SetTitle("TOT [ns]");
		getStatistics().getHisto2D("TOT_vs_ScatterAngle_thr3")->GetXaxis()->SetTitle("TOT [ns]");
		getStatistics().getHisto2D("TOT_vs_ScatterAngle_thr4")->GetXaxis()->SetTitle("TOT [ns]");
		getStatistics().getHisto2D("TOT_vs_ScatterAngle_thr1")->GetYaxis()->SetTitle("Scatter Angle [deg]");
		getStatistics().getHisto2D("TOT_vs_ScatterAngle_thr2")->GetYaxis()->SetTitle("Scatter Angle [deg]");
		getStatistics().getHisto2D("TOT_vs_ScatterAngle_thr3")->GetYaxis()->SetTitle("Scatter Angle [deg]");
		getStatistics().getHisto2D("TOT_vs_ScatterAngle_thr4")->GetYaxis()->SetTitle("Scatter Angle [deg]");
		
		getStatistics().createHistogram( new TH2F("TOT_vs_ScatterAngle", "", 900, -20.5, 69.5, 200, 0 , 200 ) );
		getStatistics().getHisto2D("TOT_vs_ScatterAngle")->GetXaxis()->SetTitle("TOT [ns]");
		getStatistics().getHisto2D("TOT_vs_ScatterAngle")->GetYaxis()->SetTitle("Scatter Angle [deg]");
		
		getStatistics().createHistogram( new TH2F("TOT_vs_ScatterAngle_notmatched", "", 900, -20.5, 69.5, 200, 0 , 200 ) );
		getStatistics().getHisto2D("TOT_vs_ScatterAngle_notmatched")->GetXaxis()->SetTitle("TOT [ns]");
		getStatistics().getHisto2D("TOT_vs_ScatterAngle_notmatched")->GetYaxis()->SetTitle("Scatter Angle [deg]");	
	
		getStatistics().createHistogram( new TH2F("TOT_vs_ID_sideA_thr1_matched", "", 200, -0.5, 19.5, 200, 0 , 200 ) );
		getStatistics().createHistogram( new TH2F("TOT_vs_ID_sideA_thr2_matched", "", 200, -0.5, 19.5, 200, 0 , 200 ) );
		getStatistics().createHistogram( new TH2F("TOT_vs_ID_sideA_thr3_matched", "", 200, -0.5, 19.5, 200, 0 , 200 ) );
		getStatistics().createHistogram( new TH2F("TOT_vs_ID_sideA_thr4_matched", "", 200, -0.5, 19.5, 200, 0 , 200 ) );
		getStatistics().createHistogram( new TH2F("TOT_vs_ID_sideB_thr1_matched", "", 200, -0.5, 19.5, 200, 0 , 200 ) );
		getStatistics().createHistogram( new TH2F("TOT_vs_ID_sideB_thr2_matched", "", 200, -0.5, 19.5, 200, 0 , 200 ) );
		getStatistics().createHistogram( new TH2F("TOT_vs_ID_sideB_thr3_matched", "", 200, -0.5, 19.5, 200, 0 , 200 ) );
		getStatistics().createHistogram( new TH2F("TOT_vs_ID_sideB_thr4_matched", "", 200, -0.5, 19.5, 200, 0 , 200 ) );
		getStatistics().getHisto2D("TOT_vs_ID_sideA_thr1_matched")->GetXaxis()->SetTitle("TOT [ns]");
		getStatistics().getHisto2D("TOT_vs_ID_sideA_thr2_matched")->GetXaxis()->SetTitle("TOT [ns]");
		getStatistics().getHisto2D("TOT_vs_ID_sideA_thr3_matched")->GetXaxis()->SetTitle("TOT [ns]");
		getStatistics().getHisto2D("TOT_vs_ID_sideA_thr4_matched")->GetXaxis()->SetTitle("TOT [ns]");
		getStatistics().getHisto2D("TOT_vs_ID_sideB_thr1_matched")->GetXaxis()->SetTitle("TOT [ns]");
		getStatistics().getHisto2D("TOT_vs_ID_sideB_thr2_matched")->GetXaxis()->SetTitle("TOT [ns]");
		getStatistics().getHisto2D("TOT_vs_ID_sideB_thr3_matched")->GetXaxis()->SetTitle("TOT [ns]");
		getStatistics().getHisto2D("TOT_vs_ID_sideB_thr4_matched")->GetXaxis()->SetTitle("TOT [ns]");
		getStatistics().getHisto2D("TOT_vs_ID_sideA_thr1_matched")->GetYaxis()->SetTitle("ID of the scintillator");
		getStatistics().getHisto2D("TOT_vs_ID_sideA_thr2_matched")->GetYaxis()->SetTitle("ID of the scintillator");
		getStatistics().getHisto2D("TOT_vs_ID_sideA_thr3_matched")->GetYaxis()->SetTitle("ID of the scintillator");
		getStatistics().getHisto2D("TOT_vs_ID_sideA_thr4_matched")->GetYaxis()->SetTitle("ID of the scintillator");
		getStatistics().getHisto2D("TOT_vs_ID_sideB_thr1_matched")->GetYaxis()->SetTitle("ID of the scintillator");
		getStatistics().getHisto2D("TOT_vs_ID_sideB_thr2_matched")->GetYaxis()->SetTitle("ID of the scintillator");
		getStatistics().getHisto2D("TOT_vs_ID_sideB_thr3_matched")->GetYaxis()->SetTitle("ID of the scintillator");
		getStatistics().getHisto2D("TOT_vs_ID_sideB_thr4_matched")->GetYaxis()->SetTitle("ID of the scintillator");
		
			getStatistics().createHistogram( new TH2F("TOT_vs_ID_matched_thr", "", 1200, -19.5, 100.5, 200, 0 , 200 ) );
			getStatistics().createHistogram( new TH2F("Energy_vs_ID_matched_thr", "", 4000, -19.5, 3980.5, 200, 0 , 200 ) );
			getStatistics().createHistogram( new TH2F("Energy_vs_ID_matched_thr2", "", 4000, -19.5, 3980.5, 200, 0 , 200 ) );
			
			getStatistics().createHistogram( new TH2F("TOT_vs_ID_matched_thr_sumTo2", "", 36000, -0.5, 359999.5, 200, 0 , 200 ) );
			getStatistics().createHistogram( new TH2F("TOT_vs_ID_matched_thr_sumTo3", "", 36000, -0.5, 359999.5, 200, 0 , 200 ) );
			getStatistics().createHistogram( new TH2F("TOT_vs_ID_matched_thr_sumTo4", "", 36000, -0.5, 3599999.5, 200, 0 , 200 ) );
			
			getStatistics().createHistogram( new TH2F("TOT_vs_ScatterAngle_thr1_matched", "", 400, -0.5, 39.5, 200, 0 , 200 ) );
			getStatistics().createHistogram( new TH2F("TOT_vs_ScatterAngle_thr2_matched", "", 400, -0.5, 39.5, 200, 0 , 200 ) );
			getStatistics().createHistogram( new TH2F("TOT_vs_ScatterAngle_thr3_matched", "", 400, -0.5, 39.5, 200, 0 , 200 ) );
			getStatistics().createHistogram( new TH2F("TOT_vs_ScatterAngle_thr4_matched", "", 400, -0.5, 39.5, 200, 0 , 200 ) );
			getStatistics().getHisto2D("TOT_vs_ScatterAngle_thr1_matched")->GetXaxis()->SetTitle("TOT [ns]");
			getStatistics().getHisto2D("TOT_vs_ScatterAngle_thr2_matched")->GetXaxis()->SetTitle("TOT [ns]");
			getStatistics().getHisto2D("TOT_vs_ScatterAngle_thr3_matched")->GetXaxis()->SetTitle("TOT [ns]");
			getStatistics().getHisto2D("TOT_vs_ScatterAngle_thr4_matched")->GetXaxis()->SetTitle("TOT [ns]");
			getStatistics().getHisto2D("TOT_vs_ScatterAngle_thr1_matched")->GetYaxis()->SetTitle("Scatter Angle [deg]");
			getStatistics().getHisto2D("TOT_vs_ScatterAngle_thr2_matched")->GetYaxis()->SetTitle("Scatter Angle [deg]");
			getStatistics().getHisto2D("TOT_vs_ScatterAngle_thr3_matched")->GetYaxis()->SetTitle("Scatter Angle [deg]");
			getStatistics().getHisto2D("TOT_vs_ScatterAngle_thr4_matched")->GetYaxis()->SetTitle("Scatter Angle [deg]");
			
			getStatistics().createHistogram( new TH2F("TOT_vs_ScatterAngle_matched_bythr", "", 900, -20.5, 69.5, 200, 0 , 200 ) );
			getStatistics().getHisto2D("TOT_vs_ScatterAngle_matched_bythr")->GetXaxis()->SetTitle("TOT [ns]");
			getStatistics().getHisto2D("TOT_vs_ScatterAngle_matched_bythr")->GetYaxis()->SetTitle("Scatter Angle [deg]");
			
	getStatistics().createHistogram( new TH2F("TOF_vs_ID", "", 400, -19.5, 19.5, 200, 0 , 200 ) );
	getStatistics().getHisto2D("TOF_vs_ID")->GetXaxis()->SetTitle("TOF [ns]");	
	getStatistics().getHisto2D("TOF_vs_ID")->GetYaxis()->SetTitle("ID of the scintillator");	
	/*// General histograms
	getStatistics().createHistogram( new TH2F("All_XYpos", "Hit position XY", 242, -60.5, 60.5, 121, -60.5, 60.5));
	getStatistics().getHisto2D("All_XYpos")->GetXaxis()->SetTitle("Hit X position [cm]");
	getStatistics().getHisto2D("All_XYpos")->GetYaxis()->SetTitle("Hit Y position [cm]");

	// Histograms for 2Gamama category
	getStatistics().createHistogram( new TH1F("2Gamma_Zpos", "B2B hits Z position", 200, -50.0, 50.0));
	getStatistics().getHisto1D("2Gamma_Zpos")->GetXaxis()->SetTitle("Z axis position [cm]");
	getStatistics().getHisto1D("2Gamma_Zpos")->GetYaxis()->SetTitle("Number of Hits");

	getStatistics().createHistogram( new TH1F("2Gamma_TimeDiff", "B2B hits time difference", 100, -10000.0, 10000.0));
	getStatistics().getHisto1D("2Gamma_TimeDiff")->GetXaxis()->SetTitle("Time Difference [ps]");
	getStatistics().getHisto1D("2Gamma_TimeDiff")->GetYaxis()->SetTitle("Number of Hit Pairs");

	getStatistics().createHistogram( new TH1F("2Gamma_Dist", "B2B hits distance", 150, 0.0, 150.0));
	getStatistics().getHisto1D("2Gamma_Dist")->GetXaxis()->SetTitle("Distance [cm]");
	getStatistics().getHisto1D("2Gamma_Dist")->GetYaxis()->SetTitle("Number of Hit Pairs");

	getStatistics().createHistogram( new TH1F("Annih_TOF", "Annihilation pairs Time of Flight", 200, -3000.0, 3000.0));
	getStatistics().getHisto1D("Annih_TOF")->GetXaxis()->SetTitle("Time of Flight [ps]");
	getStatistics().getHisto1D("Annih_TOF")->GetYaxis()->SetTitle("Number of Annihilation Pairs");

	getStatistics().createHistogram( new TH2F("AnnihPoint_XY", "XY position of annihilation point", 121, -60.5, 60.5, 121, -60.5, 60.5));
	getStatistics().getHisto2D("AnnihPoint_XY")->GetXaxis()->SetTitle("X position [cm]");
	getStatistics().getHisto2D("AnnihPoint_XY")->GetYaxis()->SetTitle("Y position [cm]");

	getStatistics().createHistogram( new TH2F("AnnihPoint_XZ", "XZ position of annihilation point", 121, -60.5, 60.5, 121, -60.5, 60.5));
	getStatistics().getHisto2D("AnnihPoint_XZ")->GetXaxis()->SetTitle("X position [cm]");
	getStatistics().getHisto2D("AnnihPoint_XZ")->GetYaxis()->SetTitle("Z position [cm]");

	getStatistics().createHistogram( new TH2F("AnnihPoint_YZ", "YZ position of annihilation point", 121, -60.5, 60.5, 121, -60.5, 60.5));
	getStatistics().getHisto2D("AnnihPoint_YZ")->GetXaxis()->SetTitle("Y position [cm]");
	getStatistics().getHisto2D("AnnihPoint_YZ")->GetYaxis()->SetTitle("Z position [cm]");

	// Histograms for 3Gamama category
	getStatistics().createHistogram( new TH2F("3Gamma_Angles", "Relative angles - transformed", 251, -0.5, 250.5, 201, -0.5, 200.5));
	getStatistics().getHisto2D("3Gamma_Angles")->GetXaxis()->SetTitle("Relative angle 1-2");
	getStatistics().getHisto2D("3Gamma_Angles")->GetYaxis()->SetTitle("Relative angle 2-3");

	// Histograms for scattering category
	getStatistics().createHistogram( new TH1F("ScatterTOF_TimeDiff", "Difference of Scatter TOF and hits time difference",
	    200, 0.0, 3.0*fScatterTOFTimeDiff));
	getStatistics().getHisto1D("ScatterTOF_TimeDiff")->GetXaxis()->SetTitle("Scat_TOF & time diff [ps]");
	getStatistics().getHisto1D("ScatterTOF_TimeDiff")->GetYaxis()->SetTitle("Number of Hit Pairs");

	getStatistics().createHistogram( new TH2F("ScatterAngle_PrimaryTOT", "Angle of scattering vs. TOT of primary hits",
	    181, -0.5, 180.5, 200, 0.0, 40000.0));
	getStatistics().getHisto2D("ScatterAngle_PrimaryTOT")->GetXaxis()->SetTitle("Scattering Angle");
	getStatistics().getHisto2D("ScatterAngle_PrimaryTOT")->GetYaxis()->SetTitle("TOT of primary hit [ps]");

	getStatistics().createHistogram( new TH2F("ScatterAngle_ScatterTOT", "Angle of scattering vs. TOT of scattered hits",
	    181, -0.5, 180.5, 200, 0.0, 40000.0));
	getStatistics().getHisto2D("ScatterAngle_ScatterTOT")->GetXaxis()->SetTitle("Scattering Angle");
	getStatistics().getHisto2D("ScatterAngle_ScatterTOT")->GetYaxis()->SetTitle("TOT of scattered hit [ps]");

	// Histograms for deexcitation
	getStatistics().createHistogram( new TH1F("Deex_TOT_cut", "TOT of all hits with deex cut (30,50) ns",
	    200, 25000.0, 55000.0));
	getStatistics().getHisto1D("Deex_TOT_cut")->GetXaxis()->SetTitle("TOT [ps]");
	getStatistics().getHisto1D("Deex_TOT_cut")->GetYaxis()->SetTitle("Number of Hits");*/
}

TVector3 RecoPos2Hit( const JPetHit & Hit1, const JPetHit & Hit2)
{
	double tof = fabs( Hit1.getTime() - Hit2.getTime() )/1000;
	double VecLength = sqrt( pow(Hit1.getPosX()-Hit2.getPosX(),2)	// Pos in cm
			+pow(Hit1.getPosY()-Hit2.getPosY(),2)
			+pow(Hit1.getPosZ()-Hit2.getPosZ(),2) );
	double middleX = (Hit1.getPosX()+Hit2.getPosX() )/2;
	double middleY = (Hit1.getPosY()+Hit2.getPosY() )/2;
	double middleZ = (Hit1.getPosZ()+Hit2.getPosZ() )/2;
	double Fraction = 2*(tof/2)*29.979246/VecLength;
	TVector3 vec1(1000,1000,1000);
	if( Hit1.getTime() <= Hit2.getTime() )
	{
		//TVector3 vec1( middleX + Fraction*( Hit1.getPosX()-middleX ), middleY + Fraction*( Hit1.getPosY()-middleY ), middleZ + Fraction*( Hit1.getPosZ()-middleZ ) );
		vec1(0) = middleX + Fraction*( Hit1.getPosX()-middleX );
		vec1(1) = middleY + Fraction*( Hit1.getPosY()-middleY );
		vec1(2) = middleZ + Fraction*( Hit1.getPosZ()-middleZ );
		return vec1;
	}
	else
	{
		//TVector3 vec1( middleX + Fraction*( Hit2.getPosX()-middleX ), middleY + Fraction*( Hit2.getPosY()-middleY ), middleZ + Fraction*( Hit2.getPosZ()-middleZ ) );
		vec1(0) = middleX + Fraction*( Hit2.getPosX()-middleX );
		vec1(1) = middleY + Fraction*( Hit2.getPosY()-middleY );
		vec1(2) = middleZ + Fraction*( Hit2.getPosZ()-middleZ );
		return vec1;
	}
	return vec1;
}

void EventCategorizer::PlotLifetimesForThresholds( JPetHit Hit1, JPetHit Hit2 )
{
	double TDiff = 0.;
	std::vector<JPetSigCh> sigALeadHit1 = Hit1.getSignalA().getRecoSignal().getRawSignal().getPoints(JPetSigCh::Leading, JPetRawSignal::ByThrNum);
	std::vector<JPetSigCh> sigBLeadHit1 = Hit1.getSignalB().getRecoSignal().getRawSignal().getPoints(JPetSigCh::Leading, JPetRawSignal::ByThrNum);
	
	std::vector<JPetSigCh> sigALeadHit2 = Hit2.getSignalA().getRecoSignal().getRawSignal().getPoints(JPetSigCh::Leading, JPetRawSignal::ByThrNum);
	std::vector<JPetSigCh> sigBLeadHit2 = Hit2.getSignalB().getRecoSignal().getRawSignal().getPoints(JPetSigCh::Leading, JPetRawSignal::ByThrNum);
	
	
	TVector3 AnniPos(0, 0, 0);
	double POF1 = (Hit1.getPos() - AnniPos).Mag(); // Path of Fly
	double POF2 = (Hit2.getPos() - AnniPos).Mag();
	
	double cvel = 29.9792458;
	
	double TOFDiff = (POF1 - POF2)/cvel;
	
	double CorrIter1 = 0., CorrIter2 = 0., ABCorr = 0.;
	
	if( sigALeadHit1.size() > 0 && sigBLeadHit1.size() > 0 && sigALeadHit2.size() > 0 && sigBLeadHit2.size() > 0 )
	{
		for (unsigned i = 0; i < sigALeadHit1.size() && i < sigBLeadHit1.size() && i < sigALeadHit2.size() && i < sigBLeadHit2.size(); i++)
		{
			TDiff = (sigBLeadHit1.at(i).getValue() + sigALeadHit1.at(i).getValue())/(2*1000) - (sigBLeadHit2.at(i).getValue() + sigALeadHit2.at(i).getValue())/(2*1000);		//TimeHit1 - TimeHit2
			switch(i)
			{
				case 0:
					ABCorr = (TimeShiftSideB_thr1[ Hit1.getScintillator().getID() - 1 ] + TimeShiftSideA_thr1[ Hit1.getScintillator().getID() - 1 ])/4;
					ABCorr = ABCorr - (TimeShiftSideB_thr1[ Hit2.getScintillator().getID() - 1 ] + TimeShiftSideA_thr1[ Hit2.getScintillator().getID() - 1 ])/4;
					
					ABCorr = 0;
					
					getStatistics().getHisto2D("PALS_vs_AnnihilationID_thr1")->Fill( TDiff - TOFDiff - ABCorr, Hit1.getScintillator().getID() );
					getStatistics().getHisto2D("PALS_vs_DeexcitationID_thr1")->Fill( TDiff - TOFDiff - ABCorr, Hit2.getScintillator().getID() );
					
					CorrIter1 = (PALSTDiff_thr1[ Hit1.getScintillator().getID() - 1 ] - PALSTDiff_thr1[ Hit2.getScintillator().getID() - 1 ])/2;
					getStatistics().getHisto2D("PALS_vs_AnnihilationID_thr1_iter1")->Fill( TDiff - TOFDiff - CorrIter1 - ABCorr, Hit1.getScintillator().getID() );
					getStatistics().getHisto2D("PALS_vs_DeexcitationID_thr1_iter1")->Fill( TDiff - TOFDiff - CorrIter1 - ABCorr, Hit2.getScintillator().getID() );
					
					CorrIter2 = (PALSTDiff_thr1_iter2[ Hit1.getScintillator().getID() - 1 ] - PALSTDiff_thr1_iter2[ Hit2.getScintillator().getID() - 1 ])/2;
					getStatistics().getHisto2D("PALS_vs_AnnihilationID_thr1_iter2")->Fill( TDiff - TOFDiff - CorrIter1 - CorrIter2 - ABCorr, Hit1.getScintillator().getID() );
					getStatistics().getHisto2D("PALS_vs_DeexcitationID_thr1_iter2")->Fill( TDiff - TOFDiff - CorrIter1 - CorrIter2 - ABCorr, Hit2.getScintillator().getID() );
					break;
				case 1:
					ABCorr = (TimeShiftSideB_thr2[ Hit1.getScintillator().getID() - 1 ] + TimeShiftSideA_thr2[ Hit1.getScintillator().getID() - 1 ])/4;
					ABCorr = ABCorr - (TimeShiftSideB_thr2[ Hit2.getScintillator().getID() - 1 ] + TimeShiftSideA_thr2[ Hit2.getScintillator().getID() - 1 ])/4;
					
					ABCorr = 0;
					
					getStatistics().getHisto2D("PALS_vs_AnnihilationID_thr2")->Fill( TDiff - TOFDiff - ABCorr, Hit1.getScintillator().getID() );
					getStatistics().getHisto2D("PALS_vs_DeexcitationID_thr2")->Fill( TDiff - TOFDiff - ABCorr, Hit2.getScintillator().getID() );
					
					CorrIter1 = (PALSTDiff_thr2[ Hit1.getScintillator().getID() - 1 ] - PALSTDiff_thr2[ Hit2.getScintillator().getID() - 1 ])/2;
					getStatistics().getHisto2D("PALS_vs_AnnihilationID_thr2_iter1")->Fill( TDiff - TOFDiff - CorrIter1 - ABCorr, Hit1.getScintillator().getID() );
					getStatistics().getHisto2D("PALS_vs_DeexcitationID_thr2_iter1")->Fill( TDiff - TOFDiff - CorrIter1 - ABCorr, Hit2.getScintillator().getID() );
					
					CorrIter2 = (PALSTDiff_thr2_iter2[ Hit1.getScintillator().getID() - 1 ] - PALSTDiff_thr2_iter2[ Hit2.getScintillator().getID() - 1 ])/2;
					getStatistics().getHisto2D("PALS_vs_AnnihilationID_thr2_iter2")->Fill( TDiff - TOFDiff - CorrIter1 - CorrIter2 - ABCorr, Hit1.getScintillator().getID() );
					getStatistics().getHisto2D("PALS_vs_DeexcitationID_thr2_iter2")->Fill( TDiff - TOFDiff - CorrIter1 - CorrIter2 - ABCorr, Hit2.getScintillator().getID() );
					break;						  
				case 2:
					ABCorr = (TimeShiftSideB_thr3[ Hit1.getScintillator().getID() - 1 ] + TimeShiftSideA_thr3[ Hit1.getScintillator().getID() - 1 ])/4;
					ABCorr = ABCorr - (TimeShiftSideB_thr3[ Hit2.getScintillator().getID() - 1 ] + TimeShiftSideA_thr3[ Hit2.getScintillator().getID() - 1 ])/4;
					
					ABCorr = 0;					
								
					getStatistics().getHisto2D("PALS_vs_AnnihilationID_thr3")->Fill( TDiff - TOFDiff - ABCorr, Hit1.getScintillator().getID() );
					getStatistics().getHisto2D("PALS_vs_DeexcitationID_thr3")->Fill( TDiff - TOFDiff - ABCorr, Hit2.getScintillator().getID() );
					
					CorrIter1 = (PALSTDiff_thr3[ Hit1.getScintillator().getID() - 1 ] - PALSTDiff_thr3[ Hit2.getScintillator().getID() - 1 ])/2;
					getStatistics().getHisto2D("PALS_vs_AnnihilationID_thr3_iter1")->Fill( TDiff - TOFDiff - CorrIter1 - ABCorr, Hit1.getScintillator().getID() );
					getStatistics().getHisto2D("PALS_vs_DeexcitationID_thr3_iter1")->Fill( TDiff - TOFDiff - CorrIter1 - ABCorr, Hit2.getScintillator().getID() );
					
					CorrIter2 = (PALSTDiff_thr3_iter2[ Hit1.getScintillator().getID() - 1 ] - PALSTDiff_thr3_iter2[ Hit2.getScintillator().getID() - 1 ])/2;
					getStatistics().getHisto2D("PALS_vs_AnnihilationID_thr3_iter2")->Fill( TDiff - TOFDiff - CorrIter1 - CorrIter2 - ABCorr, Hit1.getScintillator().getID() );
					getStatistics().getHisto2D("PALS_vs_DeexcitationID_thr3_iter2")->Fill( TDiff - TOFDiff - CorrIter1 - CorrIter2 - ABCorr, Hit2.getScintillator().getID() );
					break;						  
				case 3:
					ABCorr = (TimeShiftSideB_thr4[ Hit1.getScintillator().getID() - 1 ] + TimeShiftSideA_thr4[ Hit1.getScintillator().getID() - 1 ])/4;
					ABCorr = ABCorr - (TimeShiftSideB_thr4[ Hit2.getScintillator().getID() - 1 ] + TimeShiftSideA_thr4[ Hit2.getScintillator().getID() - 1 ])/4;
					
					ABCorr = 0;					
							  		
					getStatistics().getHisto2D("PALS_vs_AnnihilationID_thr4")->Fill( TDiff - TOFDiff - ABCorr, Hit1.getScintillator().getID() );
					getStatistics().getHisto2D("PALS_vs_DeexcitationID_thr4")->Fill( TDiff - TOFDiff - ABCorr, Hit2.getScintillator().getID() );
					
					CorrIter1 = (PALSTDiff_thr4[ Hit1.getScintillator().getID() - 1 ] - PALSTDiff_thr4[ Hit2.getScintillator().getID() - 1 ])/2;
					getStatistics().getHisto2D("PALS_vs_AnnihilationID_thr4_iter1")->Fill( TDiff - TOFDiff - CorrIter1 - ABCorr, Hit1.getScintillator().getID() );
					getStatistics().getHisto2D("PALS_vs_DeexcitationID_thr4_iter1")->Fill( TDiff - TOFDiff - CorrIter1 - ABCorr, Hit2.getScintillator().getID() );
					
					CorrIter2 = (PALSTDiff_thr4_iter2[ Hit1.getScintillator().getID() - 1 ] - PALSTDiff_thr4_iter2[ Hit2.getScintillator().getID() - 1 ])/2;
					getStatistics().getHisto2D("PALS_vs_AnnihilationID_thr4_iter2")->Fill( TDiff - TOFDiff - CorrIter1 - CorrIter2 - ABCorr, Hit1.getScintillator().getID() );
					getStatistics().getHisto2D("PALS_vs_DeexcitationID_thr4_iter2")->Fill( TDiff - TOFDiff - CorrIter1 - CorrIter2 - ABCorr, Hit2.getScintillator().getID() );
					break;						  
			}
		}
	}
}

double NormalizeTimeToPoint( JPetHit Hit1, TVector3 Point )
{
	TVector3 vec1( Hit1.getPosX() - Point(0), Hit1.getPosY() - Point(1), Hit1.getPosZ() - Point(2) );
	double Length0 = vec1.Mag();
	return Hit1.getTime()/1000 - (Length0)/29.979246;
}

void EventCategorizer::CalcTDiffAB( JPetHit Hit )
{
	double TDiff_AB = 0., Correction = 0.;
	std::vector<JPetSigCh> sigALead = Hit.getSignalA().getRecoSignal().getRawSignal().getPoints(JPetSigCh::Leading, JPetRawSignal::ByThrNum);
	std::vector<JPetSigCh> sigBLead = Hit.getSignalB().getRecoSignal().getRawSignal().getPoints(JPetSigCh::Leading, JPetRawSignal::ByThrNum);
	int ScintID = Hit.getScintillator().getID();
	
	if (sigALead.size() > 0 && sigBLead.size() > 0)
	{
		for (unsigned i = 0; i < sigALead.size() && i < sigBLead.size(); i++)
		{
			TDiff_AB = (sigBLead.at(i).getValue() - sigALead.at(i).getValue())/1000;
			switch(i)
			{
				case 0:
					Correction = TimeShiftSideB_thr1[ScintID-1] + TimeShiftSideA_thr1[ScintID-1];
					
					getStatistics().getHisto2D("TDiff_AB_vs_ID_thr1_nocorr")->Fill( TDiff_AB, Hit.getScintillator().getID() );
					
					getStatistics().getHisto2D("TDiff_AB_vs_ID_thr1_corr")->Fill( TDiff_AB - Correction/2, Hit.getScintillator().getID() );
					//getStatistics().getHisto2D("AllHit_Z_vs_ID_corr")->Fill( (TDiff_AB - Correction)*49/( TimeShiftSideB_thr1[ScintID-1] - TimeShiftSideA_thr1[ScintID-1] ), ScintID );
					break;
				case 1:
					Correction = TimeShiftSideB_thr2[ScintID-1] + TimeShiftSideA_thr2[ScintID-1];
					
					getStatistics().getHisto2D("TDiff_AB_vs_ID_thr2_nocorr")->Fill( TDiff_AB, Hit.getScintillator().getID() );
					
					getStatistics().getHisto2D("TDiff_AB_vs_ID_thr2_corr")->Fill( TDiff_AB - Correction/2, Hit.getScintillator().getID() );
					break;						  
				case 2:
					Correction = TimeShiftSideB_thr3[ScintID-1] + TimeShiftSideA_thr3[ScintID-1];
					
					getStatistics().getHisto2D("TDiff_AB_vs_ID_thr3_nocorr")->Fill( TDiff_AB, Hit.getScintillator().getID() );
					
					getStatistics().getHisto2D("TDiff_AB_vs_ID_thr3_corr")->Fill( TDiff_AB - Correction/2, Hit.getScintillator().getID() );
					break;						  
				case 3:
					Correction = TimeShiftSideB_thr4[ScintID-1] + TimeShiftSideA_thr4[ScintID-1];
					
					getStatistics().getHisto2D("TDiff_AB_vs_ID_thr4_nocorr")->Fill( TDiff_AB, Hit.getScintillator().getID() );
					
					getStatistics().getHisto2D("TDiff_AB_vs_ID_thr4_corr")->Fill( TDiff_AB - Correction/2, Hit.getScintillator().getID() );
					break;						  
			}
		}
	}
}

void EventCategorizer::PlotTOTvsScatterAnglesForThresholds( const JPetHit& hit, double ScatterAngle )
{
	double tot = 0.0, tot2 = 0., tottemp = 0.;
	std::vector<JPetSigCh> sigALead = hit.getSignalA().getRecoSignal().getRawSignal().getPoints(JPetSigCh::Leading, JPetRawSignal::ByThrValue);
	std::vector<JPetSigCh> sigBLead = hit.getSignalB().getRecoSignal().getRawSignal().getPoints(JPetSigCh::Leading, JPetRawSignal::ByThrValue);
	std::vector<JPetSigCh> sigATrail = hit.getSignalA().getRecoSignal().getRawSignal().getPoints(JPetSigCh::Trailing, JPetRawSignal::ByThrValue);
	std::vector<JPetSigCh> sigBTrail = hit.getSignalB().getRecoSignal().getRawSignal().getPoints(JPetSigCh::Trailing, JPetRawSignal::ByThrValue);

	int ScintID = hit.getScintillator().getID();
	
	if( sigALead.size() > 0 && sigATrail.size() > 0 && sigBLead.size() > 0 && sigBTrail.size() > 0 )
	{
		for (unsigned i = 0; i < sigALead.size() && i < sigATrail.size() && i < sigBLead.size() && i < sigBTrail.size(); i++)
		{
			tot = (sigATrail.at(i).getValue() - sigALead.at(i).getValue());
			tot += (sigBTrail.at(i).getValue() - sigBLead.at(i).getValue());
			switch(i)
			{
				case 0:
					tottemp = (sigATrail.at(i).getValue() - sigALead.at(i).getValue()) * 9/TOTParamA_thr1[ ScintID - 1 ];
					tottemp += (sigBTrail.at(i).getValue() - sigBLead.at(i).getValue()) * 9/TOTParamB_thr1[ ScintID - 1 ];
					tot2 += tottemp;
					getStatistics().getHisto2D("TOT_vs_ScatterAngle_thr1")->Fill( tot/1000, ScatterAngle );
					getStatistics().getHisto2D("TOT_vs_ScatterAngle_thr1_matched")->Fill( tottemp/1000, ScatterAngle );
					break;
				case 1:
					tottemp = (sigATrail.at(i).getValue() - sigALead.at(i).getValue()) * 8/TOTParamA_thr2[ ScintID - 1 ];
					tottemp += (sigBTrail.at(i).getValue() - sigBLead.at(i).getValue()) * 8/TOTParamB_thr2[ ScintID - 1 ];
					tot2 += tottemp;
					getStatistics().getHisto2D("TOT_vs_ScatterAngle_thr2")->Fill( tot/1000, ScatterAngle );
					getStatistics().getHisto2D("TOT_vs_ScatterAngle_thr2_matched")->Fill( tottemp/1000, ScatterAngle );					
					break;						  
				case 2:
				  	tottemp = (sigATrail.at(i).getValue() - sigALead.at(i).getValue()) * 7/TOTParamA_thr3[ ScintID - 1 ];
					tottemp += (sigBTrail.at(i).getValue() - sigBLead.at(i).getValue()) * 7/TOTParamB_thr3[ ScintID - 1 ];
					tot2 += tottemp;
					getStatistics().getHisto2D("TOT_vs_ScatterAngle_thr3")->Fill( tot/1000, ScatterAngle );
					getStatistics().getHisto2D("TOT_vs_ScatterAngle_thr3_matched")->Fill( tottemp/1000, ScatterAngle );	
					break;						  
				case 3:
					tottemp = (sigATrail.at(i).getValue() - sigALead.at(i).getValue()) * 6/TOTParamA_thr4[ ScintID - 1 ];
					tottemp += (sigBTrail.at(i).getValue() - sigBLead.at(i).getValue()) * 6/TOTParamB_thr4[ ScintID - 1 ];
					tot2 += tottemp;
					getStatistics().getHisto2D("TOT_vs_ScatterAngle_thr4")->Fill( tot/1000, ScatterAngle );
					getStatistics().getHisto2D("TOT_vs_ScatterAngle_thr4_matched")->Fill( tottemp/1000, ScatterAngle );
					break;						  
			}
		}
	}
	getStatistics().getHisto2D("TOT_vs_ScatterAngle_matched_bythr")->Fill( tot2/1000, ScatterAngle );
}

void EventCategorizer::calculateTOTforThresholdsMatched(const JPetHit& hit)
{
	double tot = 0.0, totSum = 0., totSumTo2 = 0., totSumTo3 = 0., totSumTo4 = 0.;
	double Corr = 0.;
	std::vector<JPetSigCh> sigALead = hit.getSignalA().getRecoSignal().getRawSignal().getPoints(JPetSigCh::Leading, JPetRawSignal::ByThrValue);
	std::vector<JPetSigCh> sigBLead = hit.getSignalB().getRecoSignal().getRawSignal().getPoints(JPetSigCh::Leading, JPetRawSignal::ByThrValue);
	std::vector<JPetSigCh> sigATrail = hit.getSignalA().getRecoSignal().getRawSignal().getPoints(JPetSigCh::Trailing, JPetRawSignal::ByThrValue);
	std::vector<JPetSigCh> sigBTrail = hit.getSignalB().getRecoSignal().getRawSignal().getPoints(JPetSigCh::Trailing, JPetRawSignal::ByThrValue);

	int ScintID = hit.getScintillator().getID();	
	if (sigALead.size() > 0 && sigATrail.size() > 0)
	{
		for (unsigned i = 0; i < sigALead.size() && i < sigATrail.size(); i++)
		{
			tot = (sigATrail.at(i).getValue() - sigALead.at(i).getValue());
			switch(i)
			{
				case 0:
					Corr = 9/TOTParamA_thr1[ ScintID - 1 ];
					getStatistics().getHisto2D("TOT_vs_ID_sideA_thr1_matched")->Fill( Corr*tot/1000, hit.getScintillator().getID() );
					totSum += Corr*tot/1000;
					totSumTo2 += pow(20,2)*Corr*tot/1000;
					totSumTo3 += pow(20,3)*Corr*tot/1000;
					totSumTo4 += pow(20,4)*Corr*tot/1000;
					break;
				case 1:
					Corr = 8/TOTParamA_thr2[ ScintID - 1 ];				  
					getStatistics().getHisto2D("TOT_vs_ID_sideA_thr2_matched")->Fill( Corr*tot/1000, hit.getScintillator().getID() );
					totSum += Corr*tot/1000;
					totSumTo2 += pow(10,2)*Corr*tot/1000;
					totSumTo3 += pow(10,3)*Corr*tot/1000;
					totSumTo4 += pow(10,4)*Corr*tot/1000;
					break;						  
				case 2:
					Corr = 7/TOTParamA_thr3[ ScintID - 1 ];					  
					getStatistics().getHisto2D("TOT_vs_ID_sideA_thr3_matched")->Fill( Corr*tot/1000, hit.getScintillator().getID() );
					totSum += Corr*tot/1000;
					totSumTo2 += pow(5,2)*Corr*tot/1000;
					totSumTo3 += pow(5,3)*Corr*tot/1000;
					totSumTo4 += pow(5,4)*Corr*tot/1000;
					break;						  
				case 3:
					Corr = 6/TOTParamA_thr4[ ScintID - 1 ];					  
					getStatistics().getHisto2D("TOT_vs_ID_sideA_thr4_matched")->Fill( Corr*tot/1000, hit.getScintillator().getID() );
					totSum += Corr*tot/1000;
					totSumTo2 += pow(10/3,2)*Corr*tot/1000;
					totSumTo3 += pow(10/3,2)*Corr*tot/1000;
					totSumTo4 += pow(10/3,2)*Corr*tot/1000;
					break;						  
			}
		}
	}	
	if (sigBLead.size() > 0 && sigBTrail.size() > 0)
	{
		for (unsigned i = 0; i < sigBLead.size() && i < sigBTrail.size(); i++)
		{
			tot = (sigBTrail.at(i).getValue() - sigBLead.at(i).getValue());
			switch(i)
			{
				case 0:
					Corr = 9/TOTParamB_thr1[ ScintID - 1 ];					  
					getStatistics().getHisto2D("TOT_vs_ID_sideB_thr1_matched")->Fill( Corr*tot/1000, hit.getScintillator().getID() );
					totSum += Corr*tot/1000;
					totSumTo2 += pow(20,2)*Corr*tot/1000;
					totSumTo3 += pow(20,3)*Corr*tot/1000;
					totSumTo4 += pow(20,4)*Corr*tot/1000;
					break;
				case 1:
					Corr = 8/TOTParamB_thr2[ ScintID - 1 ];					  
					getStatistics().getHisto2D("TOT_vs_ID_sideB_thr2_matched")->Fill( Corr*tot/1000, hit.getScintillator().getID() );
					totSum += Corr*tot/1000;
					totSumTo2 += pow(10,2)*Corr*tot/1000;
					totSumTo3 += pow(10,3)*Corr*tot/1000;
					totSumTo4 += pow(10,4)*Corr*tot/1000;
					break;						  
				case 2:
					Corr = 7/TOTParamB_thr3[ ScintID - 1 ];					  
					getStatistics().getHisto2D("TOT_vs_ID_sideB_thr3_matched")->Fill( Corr*tot/1000, hit.getScintillator().getID() );
					totSum += Corr*tot/1000;
					totSumTo2 += pow(5,2)*Corr*tot/1000;
					totSumTo3 += pow(5,3)*Corr*tot/1000;
					totSumTo4 += pow(5,4)*Corr*tot/1000;
					break;						  
				case 3:
					Corr = 6/TOTParamB_thr4[ ScintID - 1 ];	
					getStatistics().getHisto2D("TOT_vs_ID_sideB_thr4_matched")->Fill( Corr*tot/1000, hit.getScintillator().getID() );
					totSum += Corr*tot/1000;
					totSumTo2 += pow(10/3,2)*Corr*tot/1000;
					totSumTo3 += pow(10/3,2)*Corr*tot/1000;
					totSumTo4 += pow(10/3,2)*Corr*tot/1000;
					break;						  
			}
		}
	}
	if( totSum != 0. )
	{
		getStatistics().getHisto2D("TOT_vs_ID_matched_thr")->Fill( totSum, hit.getScintillator().getID() );
		getStatistics().getHisto2D("TOT_vs_ID_matched_thr_sumTo2")->Fill( totSumTo2, hit.getScintillator().getID() );
		getStatistics().getHisto2D("TOT_vs_ID_matched_thr_sumTo3")->Fill( totSumTo3, hit.getScintillator().getID() );
		getStatistics().getHisto2D("TOT_vs_ID_matched_thr_sumTo4")->Fill( totSumTo4, hit.getScintillator().getID() );		
		double Energy = CalcEnergy( totSum );
		double Energy2 = CalcEnergy2( totSum );
		getStatistics().getHisto2D("Energy_vs_ID_matched_thr")->Fill( Energy, hit.getScintillator().getID() );
		getStatistics().getHisto2D("Energy_vs_ID_matched_thr2")->Fill( Energy2, hit.getScintillator().getID() );
	}
}

double CalcEnergy( double TOT )
{
	double A = 1.62212;
	double B = 0.12428;
	double C = 213.99398;
	double Amp1 = 16.97735;
	double InfPoint1 = 10.04516;
	double Mult1 = 5.32698;
	double Amp2 = 39.28567;
	double InfPoint2 = 19.33128;
	double Mult2 = 0.50518;
	/*double Amp3 = 6.19255;
	double InfPoint3 = 27.31491;
	double Mult3 = 0.802;	*/
	double y = A*exp( B*TOT ) + C;
	if( TOT > InfPoint1 )
	{
		y -= Amp1*exp( -Mult1*( TOT - InfPoint1 ) );
	}
	else
	{
		y -= Amp1*( 1 + log( -Mult1*( TOT - InfPoint1 ) + 1 ) );
	}
	if( TOT > InfPoint2 )
	{
		y -= Amp2*exp( -Mult2*( TOT - InfPoint2 ) );
	}
	else
	{
		y -= Amp2*( 1 + log( -Mult2*( TOT - InfPoint2 ) + 1 ) );
	}
	/*if( TOT > InfPoint3 )
	{
		y -= Amp3*exp( -Mult3*( TOT - InfPoint3 ) );
	}
	else
	{
		y -= Amp3*( 1 + log( -Mult3*( TOT - InfPoint3 ) + 1 ) );
	}*/
	return y;
}

double CalcEnergy2( double TOT )
{
	double y = 45.73269 + 0.3167*pow(TOT,2);
	return y;
}



TVector3 RecoPosNHits( std::vector<JPetHit> Hits )
{
	if( Hits.size() < 3 )
	{	
		TVector3 BadSize(-1000,-1000,-1000);
		return BadSize;
	}
	std::vector<JPetHit> HitsSortedByAngle;
	std::vector<double> Angles;
	for( unsigned i=0; i<Hits.size(); i++)
	{
		Angles.push_back( CalcPhiAngle( Hits[i] ) );
	}
	HitsSortedByAngle = SortByAngle( Hits, Angles );
	TVector3 ReconstructedPosition = FindIntersectionOfSpheres( HitsSortedByAngle );
	std::cout << "RecoPoint " << ReconstructedPosition(0) << " " << ReconstructedPosition(1) << " " << ReconstructedPosition(2) << std::endl;
	int i;
	std::cin >> i;
	return ReconstructedPosition;
}

TVector3 FindIntersectionOfSpheres( std::vector<JPetHit> HitsSortedByAngle )
{
	std::vector<double> AdditionsFromTimeDifference, Radiuses;
	double MinimalTime = HitsSortedByAngle[0].getTime()/1000;
	double tempTime;
	for( unsigned i=1; i<HitsSortedByAngle.size(); i++ )
	{
		tempTime = HitsSortedByAngle[i].getTime()/1000;
		if( tempTime < MinimalTime )
		{
			MinimalTime = tempTime;
		}
	}
	double R0 = 0, tempAddition, tempR0; // Starting Radius, the rest will be R0 + additions		
	for( unsigned i=0; i<HitsSortedByAngle.size(); i++ )
	{
		tempAddition = 29.979246*( HitsSortedByAngle[i].getTime() - 1000*MinimalTime)/1000;
		std::cout << tempAddition << std::endl;
		AdditionsFromTimeDifference.push_back( tempAddition ); //Additions in cm
	}	
	for( unsigned i=0; i<HitsSortedByAngle.size() - 1; i++ )
	{
		tempAddition = AdditionsFromTimeDifference[i+1] - AdditionsFromTimeDifference[i];
		tempR0 = ( CalcDistanceBetweenHitPositions( HitsSortedByAngle[i], HitsSortedByAngle[i+1] ) - fabs(tempAddition) )/2;
		if( tempR0 > R0 )
			R0 = tempR0;
	}
	std::cout << R0 << std::endl;
	R0 = R0 + 1; // To be sure that intersections are circles
	std::vector<TVector3> tempDoubleVector, tempDoubleVectorForNextStep;
	std::vector< std::vector<TVector3> > Points;
	TVector3 tempVector(0., 0., 0.);
	tempDoubleVector.push_back( tempVector );
	tempDoubleVector.push_back( tempVector );	
	
	for( unsigned i=0; i<HitsSortedByAngle.size(); i++ )
	{
		Radiuses.push_back( R0 + AdditionsFromTimeDifference[i] );
		Points.push_back( tempDoubleVector );
	}
	unsigned size = HitsSortedByAngle.size();
	unsigned leafTest, previousLeafTest;
	
	double Distance = 0.;
	double MinDistance = 0.;
	double PreviousDistance = 10000000.;
	int iteration = 0;
	int test = 1;
	while( test )
	{
		Points = FindIntersectionPointsOfSpheres( HitsSortedByAngle, Radiuses );
		MinDistance = 1000000.;
		for( unsigned i=0; i<2; i++ ) //First Row Treated as fixed -> then the distances can be calculated as next row minus previous
		{
			for( unsigned j=0; j<pow( 2, size-1); j++ ) // Number of permutations taking all rows besides first one > -1 came for
			{
				Distance = 0.;
				for( unsigned k=0; k<size-1; k++ )
				{
					unsigned branchSize = size/(k+1);
					unsigned leafNumber = j % branchSize;
					if( leafNumber < branchSize/2 )
						leafTest = 0;
					else
						leafTest = 1;
					previousLeafTest = leafTest;
					unsigned FirstIter = (k+1)%size;	//preventing from accesing elements that are not in the vector
					unsigned SecondIter = (k)%size;		//preventing from accesing elements that are not in the vector
					Distance += sqrt( pow( Points[FirstIter][leafTest](0) - Points[SecondIter][previousLeafTest](0), 2 ) 
								      + pow( Points[FirstIter][leafTest](1) - Points[SecondIter][previousLeafTest](1), 2 ) 
								      + pow( Points[FirstIter][leafTest](2) - Points[SecondIter][previousLeafTest](2), 2 ) );
				}
				if( Distance < MinDistance )
				{
					MinDistance = Distance;
				}
			}
		}
		if( iteration > 50 )
		{
			TVector3 BadVector(-1000, -1000, -1000);
			return BadVector;
		}
		std::cout << "Dist and Previous Dist "<< " " << MinDistance << " "  << PreviousDistance << std::endl;
		if( MinDistance > PreviousDistance )
			test = 0;
		else
		{
			PreviousDistance = MinDistance;	
		}
		for( unsigned i=0; i<Radiuses.size(); i++ )
		{
			R0 += 1;
			Radiuses[i] += 1;
		}
		iteration++;
	}
	std::vector<double> R0s, Distances;
	test = 1;
	PreviousDistance = 10000000.;
	while( test )
	{
		Points = FindIntersectionPointsOfSpheres( HitsSortedByAngle, Radiuses );
		MinDistance = 1000000.;
		for( unsigned i=0; i<2; i++ ) //First Row Treated as fixed -> then the distances can be calculated as next row minus previous
		{
			for( unsigned j=0; j<pow( 2, size-1); j++ ) // Number of permutations taking all rows besides first one > -1 came for
			{
				Distance = 0.;
				for( unsigned k=0; k<size-1; k++ )
				{
					unsigned branchSize = size/(k+1);
					unsigned leafNumber = j % branchSize;
					if( leafNumber < branchSize/2 )
						leafTest = 0;
					else
						leafTest = 1;
					previousLeafTest = leafTest;
					unsigned FirstIter = (k+1)%size;	//preventing from accesing elements that are not in the vector
					unsigned SecondIter = (k)%size;		//preventing from accesing elements that are not in the vector
					Distance += sqrt( pow( Points[FirstIter][leafTest](0) - Points[SecondIter][previousLeafTest](0), 2 ) 
								      + pow( Points[FirstIter][leafTest](1) - Points[SecondIter][previousLeafTest](1), 2 ) 
								      + pow( Points[FirstIter][leafTest](2) - Points[SecondIter][previousLeafTest](2), 2 ) );
				}
				if( Distance < MinDistance )
				{
					MinDistance = Distance;
				}
			}
		}
		std::cout << "Dist and Previous Dist "<< " " << MinDistance << " "  << PreviousDistance << std::endl;
		R0s.push_back( R0 );
		Distances.push_back( MinDistance );
		if( MinDistance > PreviousDistance )
			test = 0;
		else
		{
			PreviousDistance = MinDistance;	
		}
		for( unsigned i=0; i<Radiuses.size(); i++ )
		{
			R0 -= 0.1;
			Radiuses[i] -= 0.1;
		}
	}
	double R0Min; 
	double minEle = *std::min_element( std::begin(Distances), std::end(Distances) );
	if( minEle == Distances[0] )
	{
		R0Min = R0s[0];
	}
	else if( minEle == Distances[ Distances.size() - 1 ] )
	{
		R0Min = R0s[ R0s.size() -1 ];	  
	}
	else
	{
		R0Min = FindMinFromDerrivative( R0s, Distances );
	}
	for( unsigned i=0; i<Radiuses.size(); i++ )
	{
		Radiuses[i] += (R0Min - R0);		//Radiuses are too big -> substraction the difference, Radiuses too small -> adding the difference
	}
	Points = FindIntersectionPointsOfSpheres( HitsSortedByAngle, Radiuses );
	MinDistance = 1000000.;
	std::vector<unsigned> tempIndexes, ProperIndexes;
	for( unsigned i=0; i<size; i++ )
	{
		tempIndexes.push_back( 0 );
		ProperIndexes.push_back( 0 );		
	}
	for( unsigned i=0; i<2; i++ ) //First Row Treated as fixed -> then the distances can be calculated as next row minus previous
	{
		//tempIndexes[0] = i;
		for( unsigned j=0; j<pow( 2, size-1); j++ ) // Number of permutations taking all rows besides first one > -1 came for
		{
			Distance = 0.;
			for( unsigned k=0; k<size-1; k++ )
			{
				unsigned branchSize = size/(k+1);
				unsigned leafNumber = j % branchSize;
				if( leafNumber < branchSize/2 )
					leafTest = 0;
				else
					leafTest = 1;
				previousLeafTest = leafTest;
				unsigned FirstIter = (k+1)%size;	//preventing from accesing elements that are not in the vector
				unsigned SecondIter = (k)%size;		//preventing from accesing elements that are not in the vector
				Distance += sqrt( pow( Points[FirstIter][leafTest](0) - Points[SecondIter][previousLeafTest](0), 2 ) 
							      + pow( Points[FirstIter][leafTest](1) - Points[SecondIter][previousLeafTest](1), 2 ) 
							      + pow( Points[FirstIter][leafTest](2) - Points[SecondIter][previousLeafTest](2), 2 ) );
				tempIndexes[FirstIter] = leafTest;
				tempIndexes[SecondIter] = previousLeafTest;
			}
			if( Distance < MinDistance )
			{
				MinDistance = Distance;
				ProperIndexes = tempIndexes;
			}
		}
	}
	double XPos = 0, YPos = 0, ZPos = 0;
	for( unsigned i=0; i<ProperIndexes.size(); i++ )
	{
		XPos += ( Points[i][ ProperIndexes[i] ](0) );
		YPos += ( Points[i][ ProperIndexes[i] ](1) );
		ZPos += ( Points[i][ ProperIndexes[i] ](2) );
	}
	TVector3 RecoPoint( XPos/size, YPos/size, ZPos/size );
	return RecoPoint;
}
std::vector< std::vector<TVector3> > FindIntersectionPointsOfSpheres( std::vector<JPetHit> Hits, std::vector<double> Radiuses )
{
	std::vector< std::vector<TVector3> > ResultingPoints, BadCircleCenters;
	std::vector<TVector3> CircleCenters;
	std::vector<TVector3> Center;
	TVector3 Position(0,0,0);
	Center.push_back( Position );
	CircleCenters.push_back( Position );
	CircleCenters.push_back( Position );	
	for( unsigned i=0; i<Hits.size(); i++ )
	{
		BadCircleCenters.push_back( CircleCenters );
	}
	for( unsigned i=0; i<Hits.size(); i++ )
	{
		unsigned secondHitIterator = (i+1)%Hits.size();
		unsigned previousHitIterator = (i+Hits.size()-1)%Hits.size();
		TVector3 HitPosition( Hits[i].getPosX(), Hits[i].getPosY(), Hits[i].getPosZ() );
		TVector3 VersorToCircle( Hits[secondHitIterator].getPosX() - Hits[i].getPosX(), 
					 Hits[secondHitIterator].getPosY() - Hits[i].getPosY(), 
					 Hits[secondHitIterator].getPosZ() - Hits[i].getPosZ() );
		double DistanceBetweenCenters = VersorToCircle.Mag();
		VersorToCircle = VersorToCircle.Unit();
		if( !(fabs( Radiuses[i] - DistanceBetweenCenters ) < Radiuses[secondHitIterator]) || !(Radiuses[secondHitIterator] < Radiuses[i] + DistanceBetweenCenters) ) // Triangle inequality
			return BadCircleCenters;
		
		double DistanceFromCenterAndCircle = ( pow( Radiuses[i], 2 ) - pow( Radiuses[secondHitIterator], 2 ) + pow( DistanceBetweenCenters, 2 ) )/( 2 * DistanceBetweenCenters );
		Position = HitPosition + DistanceFromCenterAndCircle * VersorToCircle;
		double RadiusOfCircle = sqrt( pow( Radiuses[i], 2 ) - pow( DistanceFromCenterAndCircle, 2 ) );
		TVector3 SphereCenter1ToCircleCenter( Position(0) - Hits[i].getPosX(), Position(1) - Hits[i].getPosY(), Position(2) - Hits[i].getPosZ() );
		TVector3 SphereCenter2ToCircleCenter( Hits[previousHitIterator].getPosX() - Hits[i].getPosX(), 
						      Hits[previousHitIterator].getPosY() - Hits[i].getPosY(), 
						      Hits[previousHitIterator].getPosZ() - Hits[i].getPosZ() );
		TVector3 ZeroCrossProduct = SphereCenter1ToCircleCenter.Cross( SphereCenter2ToCircleCenter );
		if( ZeroCrossProduct(0) == 0 && ZeroCrossProduct(1) == 0 ZeroCrossProduct(2) == 0 )
		{
			ZeroCrossProduct(2) = 1
		}
		TVector3 VectorToMinPointOnCircle = SphereCenter1ToCircleCenter.Cross( ZeroCrossProduct );	
		VectorToMinPointOnCircle = VectorToMinPointOnCircle.Unit();	
		TVector3 MinPointCircle = Position + VectorToMinPointOnCircle * RadiusOfCircle;
		TVector3 MinPointCircle2 = Position - VectorToMinPointOnCircle * RadiusOfCircle;
		
		CircleCenters[0] = MinPointCircle;
		CircleCenters[1] = MinPointCircle2;
		ResultingPoints.push_back( CircleCenters );
	}
	return ResultingPoints;
}

double CalcPhiAngle( JPetHit Hit )
{
	TVector3 Position( Hit.getPosX(), Hit.getPosY(), Hit.getPosZ() );
        double X = Position(0);
        double Radius = sqrt( Position(0)*Position(0) + Position(1)*Position(1) );
        double Y = Position(1);
        double PhiAngle = 0;
        if( Y >=0 )
                PhiAngle = acos( X/Radius )*180/3.14159265;
        else
                PhiAngle = 360 - acos( X/Radius )*180/3.14159265;
        return PhiAngle;
}

std::vector < JPetHit > SortByAngle( std::vector < JPetHit > HitsToSort, std::vector<double> Angles ) //Bubble sort
{
	std::vector < JPetHit > SortedHits = HitsToSort;
	JPetHit tempHit;
	double tempAngle;
	double NumberOfSorts = 0;
	if( HitsToSort.size() == 0 )
	{
		return SortedHits;
	}
	for( unsigned i=0; i<HitsToSort.size()-1; i++ )
	{
		for( unsigned j=0; j<HitsToSort.size()-1; j++ )
		{
			if( Angles[j] > Angles[j+1] )
			{
				tempHit = SortedHits[j];
				SortedHits[j] = SortedHits[j+1];
				SortedHits[j+1] = tempHit;
				
				tempAngle = Angles[j];
				Angles[j] = Angles[j+1];
				Angles[j+1] = tempAngle;
				
				NumberOfSorts++;
			}
		}
		if( NumberOfSorts == 0 )
			break;
		NumberOfSorts = 0;
	}
	return SortedHits;
}

double CalcDistanceBetweenHitPositions( JPetHit Hit1, JPetHit Hit2 )
{
	double Distance = sqrt( pow( ( Hit2.getPosX() - Hit1.getPosX() ), 2 ) + pow( ( Hit2.getPosY() - Hit1.getPosY() ), 2 ) + pow( ( Hit2.getPosZ() - Hit1.getPosZ() ), 2 ) );
	return Distance;
}

double FindMinFromDerrivative( std::vector<double> Arg, std::vector<double> Val )
{
	double Derr1 = Val[1] - Val[0];
	unsigned StopInd = 0;
	for( unsigned i=1; i<Val.size()-1; i++ )
	{
		if( Derr1 < 0 )
		{
			Derr1 = Val[i+1] - Val[i];
			if(Derr1 > 0)
			{
				StopInd = i;
				break;
			}
		}
	}
	double a = ( Val[StopInd+1] - Val[StopInd] - (Val[StopInd] - Val[StopInd-1]) )/( (Arg[StopInd+1] + Arg[StopInd])/2 - (Arg[StopInd] + Arg[StopInd-1])/2 );
	double b = Val[StopInd+1] - Val[StopInd] - a*(Arg[StopInd+1] + Arg[StopInd])/2;
	if( a )
		return -b/a;
	else
	{
		std::cout << "Error, wtf, error, no idea?!?" << std::endl;
		return 0;
	}
	
}