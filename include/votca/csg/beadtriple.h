/* 
 * Copyright 2009-2011 The VOTCA Development Team (http://www.votca.org)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef BEADTRIPLE_H
#define	BEADTRIPLE_H

#include <vector>

namespace votca { namespace csg {
using namespace votca::tools;

/**
   \brief A particle pair
 
   This class defines a particle pair. The future plan is, that the Pair class
   can be overloaded and Particle list creates these inherited pairs.
 
 */

class BeadTriple    
{
public:
    BeadTriple() {_triple.resize(3);}
    BeadTriple(Bead *bead1, Bead *bead2, Bead *bead3, vec r12, vec r13, vec r23)
        : _r12(r12), _r13(r13), _r23(r23), _dist12(abs(r12)), _dist13(abs(r13)), _dist23(abs(r23)) 
    {
        _triple.resize(3);
        _triple[0]=bead1;
        _triple[1]=bead2;
        _triple[2]=bead3;
    }
        
    virtual ~BeadTriple() 
    {   
        for(std::vector<Bead *>::iterator iter = _triple.begin(); iter!=_triple.end(); ++iter)
            delete *iter;
        _triple.clear();
    }

    /// \brief the vector connecting two beads
    vec &r12() { return _r12; }
    vec &r13() { return _r13; }
    vec &r23() { return _r23; }
    /// \brief the distance of the beads
    double &dist12() { return _dist12; }
    double &dist13() { return _dist13; }
    double &dist23() { return _dist23; }    

protected:
        vec _r12;
        vec _r13;
        vec _r23;
        double _dist12;
        double _dist13;
        double _dist23;
        std::vector<Bead *> _triple;
};

}}

#endif	/* BEADTRIPLE_H */

