///////////////////////////////////////////////////////////////////////////
//
//    Copyright 2010
//
//    This file is part of starlight.
//
//    starlight is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    starlight is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with starlight. If not, see <http://www.gnu.org/licenses/>.
//
///////////////////////////////////////////////////////////////////////////
//
// File and Version Information:
// $Rev::                             $: revision of last commit
// $Author::                          $: author of last commit
// $Date::                            $: date of last commit
//
// Description:
//
//
//
///////////////////////////////////////////////////////////////////////////


#include <iostream>
#include <fstream>

#include "reportingUtils.h"
#include "starlightconstants.h"
#include "inputParameters.h"
#include "inputParser.h"
#include "starlightconfig.h"
#include <cmath>
#include <cstring>
//#include "randomgenerator.h"
#include "CLHEPrandomgenerator.h"

using namespace std;
using namespace starlightConstants;

parameterlist parameterbase::_parameters;

#define REQUIRED true
#define NOT_REQUIRED false

//______________________________________________________________________________
inputParameters::inputParameters()
	: _configFileName        ("slight.in"),
 	  _beam1Z                ("BEAM_1_Z",0),
	  _beam1A                ("BEAM_1_A",0),
	  _beam2Z                ("BEAM_2_Z",0),
	  _beam2A                ("BEAM_2_A",0),
	  _beam1LorentzGamma     ("BEAM_1_GAMMA",0),
	  _beam2LorentzGamma     ("BEAM_2_GAMMA",0),
	  _maxW                  ("W_MAX",0),
	  _minW                  ("W_MIN",0),
	  _nmbWBins              ("W_N_BINS",0),
	  _maxRapidity           ("RAP_MAX",0),
	  _nmbRapidityBins       ("RAP_N_BINS",0),
	  _ptCutEnabled          ("CUT_PT",false),
	  _ptCutMin              ("PT_MIN",0),
	  _ptCutMax              ("PT_MAX",0),
	  _etaCutEnabled         ("CUT_ETA",false),
	  _etaCutMin             ("ETA_MIN",0),
	  _etaCutMax             ("ETA_MAX",0),
	  _productionMode        ("PROD_MODE",0),
	  _nmbEventsTot          ("N_EVENTS",0),
	  _prodParticleId        ("PROD_PID",0),
	  _randomSeed            ("RND_SEED",0),
	  _beamBreakupMode       ("BREAKUP_MODE",0),
	  _interferenceEnabled   ("INTERFERENCE",false),
	  _interferenceStrength  ("IF_STRENGTH",0),
	  _maxPtInterference     ("INT_PT_MAX",0),
	  _nmbPtBinsInterference ("INT_PT_N_BINS",0),
	  _ptBinWidthInterference("INT_PT_WIDTH",0),
	  _coherentProduction    ("COHERENT",false),
	  _incoherentFactor      ("INCO_FACTOR",0),
	  _protonEnergy          ("PROTON_ENERGY",0, NOT_REQUIRED),
	  _minGammaEnergy	 ("MIN_GAMMA_ENERGY",0, NOT_REQUIRED),
	  _maxGammaEnergy	 ("MAX_GAMMA_ENERGY",0, NOT_REQUIRED),
	  _pythiaParams          ("PYTHIA_PARAMS","", NOT_REQUIRED),
	  _pythiaFullEventRecord ("PYTHIA_FULL_EVENTRECORD",false, NOT_REQUIRED),
	  _xsecCalcMethod	 ("XSEC_METHOD",0, NOT_REQUIRED)
{
  // All parameters must be initialised in initialisation list! 
  // If not: error: 'parameter<T, validate>::parameter() [with T = unsigned int, bool validate = true]' is private
  // or similar
	
	_ip.addParameter(_beam1Z);
	_ip.addParameter(_beam2Z);
	_ip.addParameter(_beam1A);
	_ip.addParameter(_beam2A);

	_ip.addParameter(_beam1LorentzGamma);
	_ip.addParameter(_beam2LorentzGamma);
	
	_ip.addParameter(_maxW);
	_ip.addParameter(_minW);
	
	_ip.addParameter(_nmbWBins);

	_ip.addParameter(_maxRapidity);
	_ip.addParameter(_nmbRapidityBins);
	
	_ip.addParameter(_ptCutEnabled);
	_ip.addParameter(_ptCutMin);
	_ip.addParameter(_ptCutMax);
	
	_ip.addParameter(_etaCutEnabled);
	_ip.addParameter(_etaCutMax);
	_ip.addParameter(_etaCutMin);
	
	_ip.addParameter(_productionMode);
	_ip.addParameter(_nmbEventsTot);
	_ip.addParameter(_prodParticleId);
	_ip.addParameter(_randomSeed);
	_ip.addParameter(_beamBreakupMode);
	_ip.addParameter(_interferenceEnabled);
	_ip.addParameter(_interferenceStrength);
	_ip.addParameter(_maxPtInterference);
	_ip.addParameter(_nmbPtBinsInterference);
	_ip.addParameter(_coherentProduction);
	_ip.addParameter(_incoherentFactor);
	_ip.addParameter(_minGammaEnergy);
	_ip.addParameter(_maxGammaEnergy);
	_ip.addParameter(_pythiaParams);
	_ip.addParameter(_pythiaFullEventRecord);
	_ip.addParameter(_xsecCalcMethod);
}


//______________________________________________________________________________
inputParameters::~inputParameters()
{ }


//______________________________________________________________________________
bool
inputParameters::configureFromFile(const std::string &configFileName)
{
	// open config file
	_configFileName = configFileName;
	
	int nParameters = _ip.parseFile(_configFileName);
	
	if(nParameters == -1) 
	{
		printWarn << "could not open file '" << _configFileName << "'" << endl;
	  return false;
	}
	
	//ip.printParameterInfo(cout);
	
	if(_ip.validateParameters(cerr))
		printInfo << "successfully read input parameters from '" << _configFileName << "'" << endl;
	else {
 		printWarn << "problems reading input parameters from '" << _configFileName << "'" << endl
 		          << *this;
 		return false;
 	}
 	return true;
}
 bool inputParameters::init()
 {
	// Set the seed for the random generator
	randyInstance.SetSeed(_randomSeed.value());
	
 	// Calculate beam gamma in CMS frame
 	double rap1 = acosh(beam1LorentzGamma());
	double rap2 = -acosh(beam2LorentzGamma());
	_beamLorentzGamma = cosh((rap1-rap2)/2);
	
	std::cout << "Rapidity beam 1: " << rap1 << ", rapidity beam 2: " << rap2 << ", rapidity CMS system: " << (rap1+rap2)/2 << ", beam gamma in CMS: " << _beamLorentzGamma<< std::endl;
	_ptBinWidthInterference = maxPtInterference() / nmbPtBinsInterference();
	_protonEnergy           = _beamLorentzGamma * protonMass;

	// define interaction type
	switch (productionMode()) {
	case 1:
		_interactionType = PHOTONPHOTON;
		break;
	case 2:
		_interactionType = PHOTONPOMERONNARROW;
		break;
	case 3:
		_interactionType = PHOTONPOMERONWIDE;
		break;
        case 4:
                _interactionType = PHOTONPOMERONINCOHERENT;
                break;
	case 5:
		_interactionType = PHOTONUCLEARSINGLE;
		break;
	case 6:
		_interactionType = PHOTONUCLEARDOUBLE;
		break;
	case 7:
		_interactionType = PHOTONUCLEARSINGLEPA;
		break;
	case 8:
		_interactionType = PHOTONUCLEARSINGLEPAPY;
		break;
// 	case 9:
// 		_interactionType = PHOTONPHOTONKNIEHL;
// 		break;
//         case 10:
//                 _interactionType = PHOTONPHOTONKNIEHLMODIFIED;
//                 break;
	default:
		printWarn << "unknown production mode '" << _productionMode << "'" << endl;
		return false;
	}
  
	//Trying to define the proper Wmins and Wmaxs. a TEMPORARY fix....Better solution=??
	double mass        = 0;
	double width       = 0;
	double defaultMinW = 0;  // default for _minW, unless it is defined later [GeV/c^2]
	switch (prodParticleId()) {
	case 11:  // e+e- pair
		_particleType = ELECTRON;
		_decayType    = LEPTONPAIR;
		defaultMinW   = 0.01; // default is 0.01; up to 0.15 is safe for Summer 2000 triggering for e+e- pairs
		break;
	case 13:  // mu+mu- pair
		_particleType = MUON;
		_decayType    = LEPTONPAIR;
		defaultMinW   = 2 * muonMass;
		break;
	case 15:  // tau+tau- pair
		_particleType = TAUON;
		_decayType    = LEPTONPAIR;
		defaultMinW   = 2 * tauMass;
		break;
	case 10015:  // tau+tau- pair
		_particleType = TAUONDECAY;
		_decayType    = LEPTONPAIR;
		defaultMinW   = 2 * tauMass;
		break;
// 	case 24:  // W+W- pair
// 		_particleType = W;
// 		_decayType    = WW;
// 		defaultMinW   = 2 * muonMass;
// 		break;
	case 115:  // a_2(1320)
		_particleType = A2;
		_decayType    = SINGLEMESON;
		break;
	case 221:  // eta
		_particleType = ETA;
		_decayType    = SINGLEMESON;
		break;
	case 225:  // f_2(1270)
		_particleType = F2;
                defaultMinW   = 2*pionChargedMass;
		_decayType    = SINGLEMESON;
		break;
	case 331:  // eta'(958)
		_particleType = ETAPRIME;
		_decayType    = SINGLEMESON;
		break;
	case 335:  // f_2'(1525)
		_particleType = F2PRIME;
		_decayType    = SINGLEMESON;
		break;
	case 441:  // eta_c(1s)
		_particleType = ETAC;
		_decayType    = SINGLEMESON;
                defaultMinW   = etaCMass - 5 * 0.0267;
		break;
	case 9010221:  // f_0(980), was orginally called 10221? updated to standard number
		_particleType = F0;
		_decayType    = SINGLEMESON;
                defaultMinW   = 2*pionNeutralMass;
		break;
	case 33:  // Z"/Z03
		_particleType = ZOVERZ03;
		_decayType    = SINGLEMESON;
		break;
// 	case 25: // Higgs
// 		_particleType = HIGGS;
// 		_decayType    = SINGLEMESON;
// 		break;
	case 113:  // rho(770)
		_particleType = RHO;
		_decayType    = WIDEVMDEFAULT;
		mass          = 0.7685;
		width         = 0.1507;
		defaultMinW   = 2 * pionChargedMass;
		_maxW         = mass + 5 * width;
		break;
	case 913:  // rho(770) with direct pi+pi- decay, interference given by ZEUS data
		_particleType = RHOZEUS;
		_decayType    = WIDEVMDEFAULT;
		mass          = 0.7685;                    
		width         = 0.1507;
		defaultMinW   = 2 * pionChargedMass;
		_maxW         = mass + 5 * width;  // use the same 1.5GeV max mass as ZEUS
		break;
	case 999:  // pi+pi-pi+pi- phase space decay
		_particleType = FOURPRONG;
		_decayType    = WIDEVMDEFAULT;
		mass          = 1.350;
		width         = 0.360;
		defaultMinW   = 4 * pionChargedMass;
		_maxW         = 3;
		break;
	case 223:  // omega(782)
		_particleType = OMEGA;
		_decayType    = NARROWVMDEFAULT;  // will probably be moved to 3-body decay
		mass          = 0.78194;
		width         = 0.00843;
		defaultMinW   = mass - 5 * width;
		_maxW         = mass + 5 * width;
		break;
	case 333:  // phi(1020)
		_particleType = PHI;
		_decayType    = NARROWVMDEFAULT;
		mass          = 1.019413;
		width         = 0.00443;
		defaultMinW   = 2 * kaonChargedMass;
		_maxW         = mass + 5 * width;
		break;
	case 443:  // J/psi
		_particleType = JPSI;
		_decayType    = NARROWVMDEFAULT;
		mass          = 3.09692;   // JN  3.09688;
		width         = 0.000091;  // JN  0.000087;
		defaultMinW   = mass - 5 * width;
		_maxW         = mass + 5 * width;
		break;
	case 443011:  // J/psi
		_particleType = JPSI_ee;
		_decayType    = NARROWVMDEFAULT;
		mass          = 3.09692;   // JN  3.09688;
		width         = 0.000091;  // JN  0.000087;
		defaultMinW   = mass - 5 * width;
		_maxW         = mass + 5 * width;
		break;
	case 443013:  // J/psi
		_particleType = JPSI_mumu;
		_decayType    = NARROWVMDEFAULT;
		mass          = 3.09692;   // JN  3.09688;
		width         = 0.000091;  // JN  0.000087;
		defaultMinW   = mass - 5 * width;
		_maxW         = mass + 5 * width;
		break;
	case 444:  // J/psi
		_particleType = JPSI2S;
		_decayType    = NARROWVMDEFAULT;
		mass          = 3.686093;
		width         = 0.000337;
		defaultMinW   = mass - 5 * width;
		_maxW         = mass + 5 * width;
		break;
	case 444011: // J/psi
		_particleType = JPSI2S_ee;
		_decayType    = NARROWVMDEFAULT;
		mass          = 3.686093;     
		width         = 0.000337;
		defaultMinW   = mass - 5 * width;
		_maxW         = mass + 5 * width;
		break;
	case 444013:  // J/psi
		_particleType = JPSI2S_mumu;
		_decayType    = NARROWVMDEFAULT;
		mass          = 3.686093;
		width         = 0.000337;
		defaultMinW   = mass - 5 * width;
		_maxW         = mass + 5 * width;
		break;
	case 553:  // Upsilon
		_particleType = UPSILON;
		_decayType    = NARROWVMDEFAULT;
		mass          = 9.46030;
		width         = 0.00005402;
		defaultMinW   = mass - 5 * width;
		_maxW         = mass + 5 * width;
		break;
	case 553011:  // Upsilon
		_particleType = UPSILON_ee;
		_decayType    = NARROWVMDEFAULT;
		mass          = 9.46030;
		width         = 0.00005402;
		defaultMinW   = mass - 5 * width;
		_maxW         = mass + 5 * width;
		break;
	case 553013:  // Upsilon
		_particleType = UPSILON_mumu;
		_decayType    = NARROWVMDEFAULT;
		mass          = 9.46030;
		width         = 0.00005402;
		defaultMinW   = mass - 5 * width;
		_maxW         = mass + 5 * width;
		break;
	case 554:  // Upsilon(2S)
		_particleType = UPSILON2S;
		_decayType    = NARROWVMDEFAULT;
		mass          = 10.02326;
		width         = 0.00003198;
		defaultMinW   = mass - 5 * width;
		_maxW         = mass + 5 * width;
		break;
	case 554011:  // Upsilon(2S)
		_particleType = UPSILON2S_ee;
		_decayType    = NARROWVMDEFAULT;
		mass          = 10.02326;
		width         = 0.00003198;
		defaultMinW   = mass - 5 * width;
		_maxW         = mass + 5 * width;
		break;
	case 554013:  // Upsilon(2S)
		_particleType = UPSILON2S_mumu;
		_decayType    = NARROWVMDEFAULT;
		mass          = 10.02326;
		width         = 0.00003198;
		defaultMinW   = mass - 5 * width;
		_maxW         = mass + 5 * width;
		break;
	case 555:  // Upsilon(3S)
		mass          = 10.3552;
		width         = 0.00002032;
		defaultMinW   = mass - 5 * width;
		_maxW         = mass + 5 * width;
		_particleType = UPSILON3S;
		_decayType    = NARROWVMDEFAULT;
		break;
	case 555011:  // Upsilon(3S)
		_particleType = UPSILON3S_ee;
		_decayType    = NARROWVMDEFAULT;
		mass          = 10.3552;
		width         = 0.00002032;
		defaultMinW   = mass - 5 * width;
		_maxW         = mass + 5 * width;
		break;
	case 555013:  // Upsilon(3S)
		_particleType = UPSILON3S_mumu;
		_decayType    = NARROWVMDEFAULT;
		mass          = 10.3552;
		width         = 0.00002032;
		defaultMinW   = mass - 5 * width;
		_maxW         = mass + 5 * width;
		break;
	default:
		printWarn << "unknown particle ID " << _prodParticleId << endl;
		return false;
	}  // _prodParticleId

	if (_minW.value() == -1)
		_minW = defaultMinW;

	printInfo << "using the following " << *this;
	
	return true;
}


//______________________________________________________________________________
ostream&
inputParameters::print(ostream& out) const
{
	out << "starlight parameters:" << endl
	    << "    config file name  ...................... '" << _configFileName << "'" << endl
	    << "    beam 1 atomic number ................... " << _beam1Z.value() << endl
	    << "    beam 1 atomic mass number .............. " << _beam1A.value() << endl
	    << "    beam 2 atomic number ................... " << _beam2Z.value() << endl
	    << "    beam 2 atomic mass number .............. " << _beam2A.value() << endl
	    << "    Lorentz gamma of beams in CM frame ..... " << _beamLorentzGamma << endl
	    << "    mass W of produced hadronic system ..... " << _minW.value() << " < W < " << _maxW.value() << " GeV/c^2" << endl
	    << "    # of W bins ............................ " << _nmbWBins.value() << endl
	    << "    maximum absolute value for rapidity .... " << _maxRapidity.value() << endl
	    << "    # of rapidity bins ..................... " << _nmbRapidityBins.value() << endl
	    << "    cut in pT............................... " << yesNo(_ptCutEnabled.value()) << endl;
    if (_ptCutEnabled.value()) {
	out << "        minumum pT.......................... " << _ptCutMin.value() << " GeV/c" << endl
	    << "        maximum pT.......................... " << _ptCutMax.value() << " GeV/c" << endl;}
	out << "    cut in eta.............................. " << yesNo(_etaCutEnabled.value()) << endl;
    if (_etaCutEnabled.value()) {
	out << "        minumum eta......................... " << _etaCutMin.value() << endl
	    << "        maximum eta......................... " << _etaCutMax.value() << endl;}
        out << "    production mode ........................ " << _productionMode.value() << endl
	    << "    number of events to generate ........... " << _nmbEventsTot.value() << endl
	    << "    PDG ID of produced particle ............ " << _prodParticleId.value() << endl
	    << "    seed for random generator .............. " << _randomSeed.value() << endl
	    << "    breakup mode for beam particles ........ " << _beamBreakupMode.value() << endl
	    << "    interference enabled ................... " << yesNo(_interferenceEnabled.value()) << endl;
    if (_interferenceEnabled.value()) {
	out << "    interference strength .................. " << _interferenceStrength.value() << endl
	    << "    maximum p_T for interference calc. ..... " << _maxPtInterference.value() << " GeV/c" << endl
	    << "    # of p_T bins for interference calc. ... " << _nmbPtBinsInterference.value() << endl;}
    if (_productionMode.value()!=1) {
	out  << "    coherent scattering off nucleus ........ " << yesNo(_coherentProduction.value()) << endl;
    	if (!_coherentProduction.value()) {
	 out << "    scaling factor for incoh. VM prod. ..... " << _incoherentFactor.value() << endl;}
	}
	return out;
}


//______________________________________________________________________________
ostream&
inputParameters::write(ostream& out) const
{
  
	out << "BEAM_1_Z"      << beam1Z               () <<endl
	    << "BEAM_2_Z"      << beam1A               () <<endl
	    << "BEAM_1_A"      << beam2Z               () <<endl
	    << "BEAM_2_A"      << beam2A               () <<endl
	    << "BEAM_GAMMA"    << beamLorentzGamma     () <<endl
	    << "W_MAX"         << maxW                 () <<endl
	    << "W_MIN"         << minW                 () <<endl
	    << "W_N_BINS"      << nmbWBins             () <<endl
	    << "RAP_MAX"       << maxRapidity          () <<endl
	    << "RAP_N_BINS"    << nmbRapidityBins      () <<endl
	    << "CUT_PT"        << ptCutEnabled         () <<endl
	    << "PT_MIN"        << ptCutMin             () <<endl
	    << "PT_MAX"        << ptCutMax             () <<endl
	    << "CUT_ETA"       << etaCutEnabled        () <<endl
	    << "ETA_MIN"       << etaCutMin            () <<endl
	    << "ETA_MAX"       << etaCutMax            () <<endl
	    << "PROD_MODE"     << productionMode       () <<endl
	    << "N_EVENTS"      << nmbEvents            () <<endl
	    << "PROD_PID"      << prodParticleId       () <<endl
	    << "RND_SEED"      << randomSeed           () <<endl
	    << "BREAKUP_MODE"  << beamBreakupMode      () <<endl
	    << "INTERFERENCE"  << interferenceEnabled  () <<endl
	    << "IF_STRENGTH"   << interferenceStrength () <<endl
	    << "INT_PT_MAX"    << maxPtInterference    () <<endl
	    << "INT_PT_N_BINS" << nmbPtBinsInterference() <<endl
	    << "COHERENT"      << coherentProduction   () <<endl
	    << "INCO_FACTOR"   << incoherentFactor     () <<endl;

	return out;
}

std::string
inputParameters::parameterValueKey() const 
{
 
 // std::stringstream s;
  
//   s <<_beam1A<<_beam1Z<<_beam2A<<_beam1LorentzGamma<<_beam2LorentzGamma<<_maxW<<_minW;
//     <<_nmbWBins<<_maxRapidity<<_nmbRapidityBins<<_
    
  
  //return s;
  
  return parameterbase::_parameters.validationKey()
  ;
}
