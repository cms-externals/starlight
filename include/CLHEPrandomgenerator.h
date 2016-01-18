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


#ifndef CLHEPRANDOMGENERATOR_H
#define CLHEPRANDOMGENERATOR_H
#include "singleton.h"

namespace CLHEP
{
	class HepRandomEngine;
}

class CLHEPrandomGenerator
{
	public:
	void SetSeed(unsigned int seed);
	double Rndom(int i=0);
	CLHEPrandomGenerator(){}
	CLHEPrandomGenerator(const CLHEPrandomGenerator &);
	
	private:
	friend class Singleton<CLHEPrandomGenerator>;

	//virtual void generateEvent(CLHEP::HepRandomEngine*) = 0;
	
};

#define randyInstance Singleton<CLHEPrandomGenerator>::instance()

#endif  // RANDOMGENERATOR_H
	
