/*
 * Copyright 2009-2013 The VOTCA Development Team (http://www.votca.org)
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

#ifndef _DLPTOPOLOGYREADER_H
#define	_DLPTOPOLOGYREADER_H

#include <string>
#include <votca/csg/topology.h>
#include <votca/csg/topologyreader.h>

namespace votca { namespace csg {
using namespace votca::tools;

using namespace std;

/**
    \brief class for reading dlpoly topology files

    This class encapsulates the dlpoly topology reading functions and provides an interface to fill a topolgy class

*/

class DLPOLYTopologyReader
   : public TopologyReader
{
public:
    DLPOLYTopologyReader() {}

    /// read a topology file
    bool ReadTopology(string file, Topology &top);
    
    /// set the topology file name: <name>.dlpf (convention: ".dlpf"="FIELD")
    void   setFname(string name) { _fname = name; return; }
    /// get the topology file name: <name>.dlpf (convention: ".dlpf"="FIELD")
    string getFname()            { return _fname; }

private:
    string _fname;
    /// function to find and read the next line starting with a keyword/directive (skipping comments starting with "#" or ";")
    string _NextKeyline(ifstream &fs, const char* wsp);
    /// function to read the next line containing only a given keyword and an integer value after it (only skipping comments!)
    string _NextKeyInt(ifstream &fs, const char* wsp, const string &word, int &ival);
    /// function to check if the given (last read) directive line starts with a given keyword and has an integer value at the end
    bool   _isKeyInt(const string &line, const char* wsp, const string &word, int &ival);
};

}}

#endif	/* _DLPTOPOLOGYREADER_H */
