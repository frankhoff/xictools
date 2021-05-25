
/*========================================================================*
 *                                                                        *
 *  Distributed by Whiteley Research Inc., Sunnyvale, California, USA     *
 *                       http://wrcad.com                                 *
 *  Copyright (C) 2017 Whiteley Research Inc., all rights reserved.       *
 *  Author: Stephen R. Whiteley, except as indicated.                     *
 *                                                                        *
 *  As fully as possible recognizing licensing terms and conditions       *
 *  imposed by earlier work from which this work was derived, if any,     *
 *  this work is released under the Apache License, Version 2.0 (the      *
 *  "License").  You may not use this file except in compliance with      *
 *  the License, and compliance with inherited licenses which are         *
 *  specified in a sub-header below this one if applicable.  A copy       *
 *  of the License is provided with this distribution, or you may         *
 *  obtain a copy of the License at                                       *
 *                                                                        *
 *        http://www.apache.org/licenses/LICENSE-2.0                      *
 *                                                                        *
 *  See the License for the specific language governing permissions       *
 *  and limitations under the License.                                    *
 *                                                                        *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,      *
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES      *
 *   OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-        *
 *   INFRINGEMENT.  IN NO EVENT SHALL WHITELEY RESEARCH INCORPORATED      *
 *   OR STEPHEN R. WHITELEY BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER     *
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,      *
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE       *
 *   USE OR OTHER DEALINGS IN THE SOFTWARE.                               *
 *                                                                        *
 *========================================================================*
 *               XicTools Integrated Circuit Design System                *
 *                                                                        *
 * WRspice Circuit Simulation and Analysis Tool:  Device Library          *
 *                                                                        *
 *========================================================================*
 $Id:$
 *========================================================================*/

/**** BSIM4.6.2 Released by Wenwei Yang 07/31/2008 ****/

/**********
Copyright 2006 Regents of the University of California.  All rights reserved.
File: bsim4def.h
Author: 2000 Weidong Liu.
Authors: 2001- Xuemei Xi, Mohan Dunga, Ali Niknejad, Chenming Hu.
Authors: 2006- Mohan Dunga, Ali Niknejad, Chenming Hu
Authors: 2007- Mohan Dunga, Wenwei Yang, Ali Niknejad, Chenming Hu
Authors: 2008- Wenwei Yang, Ali Niknejad, Chenming Hu 
Modified by Xuemei Xi, 11/15/2002.
Modified by Xuemei Xi, 05/09/2003.
Modified by Xuemei Xi, 03/04/2004.
Modified by Xuemei Xi, Mohan Dunga, 09/24/2004.
Modified by Xuemei Xi, 07/29/2005.
Modified by Mohan Dunga, 12/13/2006
Modified by Mohan Dunga, Wenwei Yang, 05/18/2007.
Modified by Wenwei Yang, 07/31/2008.
**********/

#ifndef B$DEFS_H
#define B$DEFS_H

#include "device.h"

// This is bsim-4.6.5, which is bsim-4.6.4 with some simple mods (see
// docs) applied.
#define B4VERSION "4.6.5"

#define NEWCONV

#define FABS            fabs
#define REFTEMP         wrsREFTEMP       
#define CHARGE          wrsCHARGE        
#define CONSTCtoK       wrsCONSTCtoK     
#define CONSTboltz      wrsCONSTboltz    
#define CONSTvt0        wrsCONSTvt0      
#define CONSTKoverQ     wrsCONSTKoverQ   
#define CONSTroot2      wrsCONSTroot2    
#define CONSTe          wrsCONSTe        

struct dvaMatrix;

namespace BSIM465 {

struct sBSIM4model;
struct sBSIM4instance;

struct BSIM4adj
{
    BSIM4adj();
    ~BSIM4adj();

    double *BSIM4DPdPtr;
    double *BSIM4DPdpPtr;
    double *BSIM4DPgpPtr;
    double *BSIM4DPgmPtr;
    double *BSIM4DPspPtr;
    double *BSIM4DPbpPtr;
    double *BSIM4DPdbPtr;

    double *BSIM4DdPtr;
    double *BSIM4DdpPtr;

    double *BSIM4GPdpPtr;
    double *BSIM4GPgpPtr;
    double *BSIM4GPgmPtr;
    double *BSIM4GPgePtr;
    double *BSIM4GPspPtr;
    double *BSIM4GPbpPtr;

    double *BSIM4GMdpPtr;
    double *BSIM4GMgpPtr;
    double *BSIM4GMgmPtr;
    double *BSIM4GMgePtr;
    double *BSIM4GMspPtr;
    double *BSIM4GMbpPtr;

    double *BSIM4GEdpPtr;
    double *BSIM4GEgpPtr;
    double *BSIM4GEgmPtr;
    double *BSIM4GEgePtr;
    double *BSIM4GEspPtr;
    double *BSIM4GEbpPtr;

    double *BSIM4SPdpPtr;
    double *BSIM4SPgpPtr;
    double *BSIM4SPgmPtr;
    double *BSIM4SPsPtr;
    double *BSIM4SPspPtr;
    double *BSIM4SPbpPtr;
    double *BSIM4SPsbPtr;

    double *BSIM4SspPtr;
    double *BSIM4SsPtr;

    double *BSIM4BPdpPtr;
    double *BSIM4BPgpPtr;
    double *BSIM4BPgmPtr;
    double *BSIM4BPspPtr;
    double *BSIM4BPdbPtr;
    double *BSIM4BPbPtr;
    double *BSIM4BPsbPtr;
    double *BSIM4BPbpPtr;

    double *BSIM4DBdpPtr;
    double *BSIM4DBdbPtr;
    double *BSIM4DBbpPtr;
    double *BSIM4DBbPtr;

    double *BSIM4SBspPtr;
    double *BSIM4SBbpPtr;
    double *BSIM4SBbPtr;
    double *BSIM4SBsbPtr;

    double *BSIM4BdbPtr;
    double *BSIM4BbpPtr;
    double *BSIM4BsbPtr;
    double *BSIM4BbPtr;

    double *BSIM4DgpPtr;
    double *BSIM4DspPtr;
    double *BSIM4DbpPtr;
    double *BSIM4SdpPtr;
    double *BSIM4SgpPtr;
    double *BSIM4SbpPtr;

    double *BSIM4QdpPtr;
    double *BSIM4QgpPtr;
    double *BSIM4QspPtr;
    double *BSIM4QbpPtr;
    double *BSIM4QqPtr;
    double *BSIM4DPqPtr;
    double *BSIM4GPqPtr;
    double *BSIM4SPqPtr;

    dvaMatrix *matrix;
};

struct BSIM4dev : public IFdevice
{
    BSIM4dev();
    sGENmodel *newModl();
    sGENinstance *newInst();
    int destroy(sGENmodel**);
    int delInst(sGENmodel*, IFuid, sGENinstance*);
    int delModl(sGENmodel**, IFuid, sGENmodel*);

    void parse(int, sCKT*, sLine*);
//    int loadTest(sGENinstance*, sCKT*);   
    int load(sGENinstance*, sCKT*);
    int setup(sGENmodel*, sCKT*, int*);
    int unsetup(sGENmodel*, sCKT*);
    int resetup(sGENmodel*, sCKT*);
    int temperature(sGENmodel*, sCKT*);
    int getic(sGENmodel*, sCKT*);
//    int accept(sCKT*, sGENmodel*);
    int trunc(sGENmodel*, sCKT*, double*);
    int convTest(sGENmodel*, sCKT*);

    void backup(sGENmodel*, DEV_BKMODE);

    int setInst(int, IFdata*, sGENinstance*);  
    int setModl(int, IFdata*, sGENmodel*);
    int askInst(const sCKT*, const sGENinstance*, int, IFdata*);
    int askModl(const sGENmodel*, int, IFdata*); 
//    int findBranch(sCKT*, sGENmodel*, IFuid);

    int acLoad(sGENmodel*, sCKT*);
    int pzLoad(sGENmodel*, sCKT*, IFcomplex*);
//    int disto(int, sGENmodel*, sCKT*);
    int noise(int, int, sGENmodel*, sCKT*, sNdata*, double*);
private:
    int checkModel(sBSIM4model*, sBSIM4instance*, sCKT*);
    int PAeffGeo(double, int, int, double, double, double, double,
        double*, double*, double*, double*);
    int RdseffGeo(double, int, int, int, double, double, double,
        double, double, int, double*);
};

struct sBSIM4instancePOD
{
    int BSIM4dNode;
    int BSIM4gNodeExt;
    int BSIM4sNode;
    int BSIM4bNode;

    int BSIM4dNodePrime;
    int BSIM4gNodePrime;
    int BSIM4gNodeMid;
    int BSIM4sNodePrime;
    int BSIM4bNodePrime;
    int BSIM4dbNode;
    int BSIM4sbNode;
    int BSIM4qNode;

//SRW
    // for measuring currents
    BSIM4adj *BSIM4adjoint;

    // This provides a means to back up and restore a known-good
    // state.
    void *BSIM4backing;

// SRW
    double BSIM4m;

    double BSIM4ueff;
    double BSIM4thetavth; 
    double BSIM4von;
    double BSIM4vdsat;
    double BSIM4cgdo;
    double BSIM4qgdo;
    double BSIM4cgso;
    double BSIM4qgso;
    double BSIM4grbsb;
    double BSIM4grbdb;
    double BSIM4grbpb;
    double BSIM4grbps;
    double BSIM4grbpd;

    double BSIM4vjsmFwd;
    double BSIM4vjsmRev;
    double BSIM4vjdmFwd;
    double BSIM4vjdmRev;
    double BSIM4XExpBVS;
    double BSIM4XExpBVD;
    double BSIM4SslpFwd;
    double BSIM4SslpRev;
    double BSIM4DslpFwd;
    double BSIM4DslpRev;
    double BSIM4IVjsmFwd;
    double BSIM4IVjsmRev;
    double BSIM4IVjdmFwd;
    double BSIM4IVjdmRev;

    double BSIM4grgeltd;
    double BSIM4Pseff;
    double BSIM4Pdeff;
    double BSIM4Aseff;
    double BSIM4Adeff;

    double BSIM4l;
    double BSIM4w;
// SRW
double BSIM4wf;
    double BSIM4drainArea;
    double BSIM4sourceArea;
    double BSIM4drainSquares;
    double BSIM4sourceSquares;
    double BSIM4drainPerimeter;
    double BSIM4sourcePerimeter;
    double BSIM4sourceConductance;
    double BSIM4drainConductance;
     /* stress effect instance param */
    double BSIM4sa;
    double BSIM4sb;
    double BSIM4sd;
    double BSIM4sca;
    double BSIM4scb;
    double BSIM4scc;
    double BSIM4sc;

    double BSIM4rbdb;
    double BSIM4rbsb;
    double BSIM4rbpb;
    double BSIM4rbps;
    double BSIM4rbpd;
    
    double BSIM4delvto;
    double BSIM4xgw;
    double BSIM4ngcon;

     /* added here to account stress effect instance dependence */
    double BSIM4u0temp;
    double BSIM4vsattemp;
    double BSIM4vth0;
    double BSIM4vfb;
    double BSIM4vfbzb;
    double BSIM4vtfbphi1;
    double BSIM4vtfbphi2;
    double BSIM4k2;
    double BSIM4vbsc;
    double BSIM4k2ox;
    double BSIM4eta0;

    double BSIM4icVDS;
    double BSIM4icVGS;
    double BSIM4icVBS;
    double BSIM4nf;
    int BSIM4off;
    int BSIM4mode;
    int BSIM4trnqsMod;
    int BSIM4acnqsMod;
    int BSIM4rbodyMod;
    int BSIM4rgateMod;
    int BSIM4geoMod;
    int BSIM4rgeoMod;
    int BSIM4min;

    /* OP point */
    double BSIM4Vgsteff;
    double BSIM4vgs_eff;
    double BSIM4vgd_eff;
    double BSIM4dvgs_eff_dvg;
    double BSIM4dvgd_eff_dvg;
    double BSIM4Vdseff;
    double BSIM4nstar;
    double BSIM4Abulk;
    double BSIM4EsatL;
    double BSIM4AbovVgst2Vtm;
    double BSIM4qinv;
    double BSIM4cd;
    double BSIM4cbs;
    double BSIM4cbd;
    double BSIM4csub;
    double BSIM4Igidl;
    double BSIM4Igisl;
    double BSIM4gm;
    double BSIM4gds;
    double BSIM4gmbs;
    double BSIM4gbd;
    double BSIM4gbs;

    double BSIM4gbbs;
    double BSIM4gbgs;
    double BSIM4gbds;
    double BSIM4ggidld;
    double BSIM4ggidlg;
    double BSIM4ggidls;
    double BSIM4ggidlb;
    double BSIM4ggisld;
    double BSIM4ggislg;
    double BSIM4ggisls;
    double BSIM4ggislb;

    double BSIM4Igcs;
    double BSIM4gIgcsg;
    double BSIM4gIgcsd;
    double BSIM4gIgcss;
    double BSIM4gIgcsb;
    double BSIM4Igcd;
    double BSIM4gIgcdg;
    double BSIM4gIgcdd;
    double BSIM4gIgcds;
    double BSIM4gIgcdb;

    double BSIM4Igs;
    double BSIM4gIgsg;
    double BSIM4gIgss;
    double BSIM4Igd;
    double BSIM4gIgdg;
    double BSIM4gIgdd;

    double BSIM4Igb;
    double BSIM4gIgbg;
    double BSIM4gIgbd;
    double BSIM4gIgbs;
    double BSIM4gIgbb;

    double BSIM4grdsw;
    double BSIM4IdovVds;
    double BSIM4gcrg;
    double BSIM4gcrgd;
    double BSIM4gcrgg;
    double BSIM4gcrgs;
    double BSIM4gcrgb;

    double BSIM4gstot;
    double BSIM4gstotd;
    double BSIM4gstotg;
    double BSIM4gstots;
    double BSIM4gstotb;

    double BSIM4gdtot;
    double BSIM4gdtotd;
    double BSIM4gdtotg;
    double BSIM4gdtots;
    double BSIM4gdtotb;

    double BSIM4cggb;
    double BSIM4cgdb;
    double BSIM4cgsb;
    double BSIM4cbgb;
    double BSIM4cbdb;
    double BSIM4cbsb;
    double BSIM4cdgb;
    double BSIM4cddb;
    double BSIM4cdsb;
    double BSIM4csgb;
    double BSIM4csdb;
    double BSIM4cssb;
    double BSIM4cgbb;
    double BSIM4cdbb;
    double BSIM4csbb;
    double BSIM4cbbb;
    double BSIM4capbd;
    double BSIM4capbs;

    double BSIM4cqgb;
    double BSIM4cqdb;
    double BSIM4cqsb;
    double BSIM4cqbb;

    double BSIM4qgate;
    double BSIM4qbulk;
    double BSIM4qdrn;
    double BSIM4qsrc;
    double BSIM4qdef;

    double BSIM4qchqs;
    double BSIM4taunet;
    double BSIM4gtau;
    double BSIM4gtg;
    double BSIM4gtd;
    double BSIM4gts;
    double BSIM4gtb;
    double BSIM4SjctTempRevSatCur;
    double BSIM4DjctTempRevSatCur;
    double BSIM4SswTempRevSatCur;
    double BSIM4DswTempRevSatCur;
    double BSIM4SswgTempRevSatCur;
    double BSIM4DswgTempRevSatCur;

    struct bsim4SizeDependParam  *pParam;

    unsigned BSIM4lGiven :1;
    unsigned BSIM4wGiven :1;
    unsigned BSIM4nfGiven :1;
    unsigned BSIM4minGiven :1;
    unsigned BSIM4drainAreaGiven :1;
    unsigned BSIM4sourceAreaGiven    :1;
    unsigned BSIM4drainSquaresGiven  :1;
    unsigned BSIM4sourceSquaresGiven :1;
    unsigned BSIM4drainPerimeterGiven    :1;
    unsigned BSIM4sourcePerimeterGiven   :1;
    unsigned BSIM4saGiven :1;
    unsigned BSIM4sbGiven :1;
    unsigned BSIM4sdGiven :1;
    unsigned BSIM4scaGiven :1;
    unsigned BSIM4scbGiven :1;
    unsigned BSIM4sccGiven :1;
    unsigned BSIM4scGiven :1;
    unsigned BSIM4rbdbGiven   :1;
    unsigned BSIM4rbsbGiven   :1;
    unsigned BSIM4rbpbGiven   :1;
    unsigned BSIM4rbpdGiven   :1;
    unsigned BSIM4rbpsGiven   :1;
    unsigned BSIM4delvtoGiven   :1;
    unsigned BSIM4xgwGiven   :1;
    unsigned BSIM4ngconGiven   :1;
    unsigned BSIM4icVDSGiven :1;
    unsigned BSIM4icVGSGiven :1;
    unsigned BSIM4icVBSGiven :1;
    unsigned BSIM4trnqsModGiven :1;
    unsigned BSIM4acnqsModGiven :1;
    unsigned BSIM4rbodyModGiven :1;
    unsigned BSIM4rgateModGiven :1;
    unsigned BSIM4geoModGiven :1;
    unsigned BSIM4rgeoModGiven :1;
// SRW
    unsigned BSIM4mGiven :1;
    unsigned BSIM4wfGiven :1;

    double *BSIM4DPdPtr;
    double *BSIM4DPdpPtr;
    double *BSIM4DPgpPtr;
    double *BSIM4DPgmPtr;
    double *BSIM4DPspPtr;
    double *BSIM4DPbpPtr;
    double *BSIM4DPdbPtr;

    double *BSIM4DdPtr;
    double *BSIM4DdpPtr;

    double *BSIM4GPdpPtr;
    double *BSIM4GPgpPtr;
    double *BSIM4GPgmPtr;
    double *BSIM4GPgePtr;
    double *BSIM4GPspPtr;
    double *BSIM4GPbpPtr;

    double *BSIM4GMdpPtr;
    double *BSIM4GMgpPtr;
    double *BSIM4GMgmPtr;
    double *BSIM4GMgePtr;
    double *BSIM4GMspPtr;
    double *BSIM4GMbpPtr;

    double *BSIM4GEdpPtr;
    double *BSIM4GEgpPtr;
    double *BSIM4GEgmPtr;
    double *BSIM4GEgePtr;
    double *BSIM4GEspPtr;
    double *BSIM4GEbpPtr;

    double *BSIM4SPdpPtr;
    double *BSIM4SPgpPtr;
    double *BSIM4SPgmPtr;
    double *BSIM4SPsPtr;
    double *BSIM4SPspPtr;
    double *BSIM4SPbpPtr;
    double *BSIM4SPsbPtr;

    double *BSIM4SspPtr;
    double *BSIM4SsPtr;

    double *BSIM4BPdpPtr;
    double *BSIM4BPgpPtr;
    double *BSIM4BPgmPtr;
    double *BSIM4BPspPtr;
    double *BSIM4BPdbPtr;
    double *BSIM4BPbPtr;
    double *BSIM4BPsbPtr;
    double *BSIM4BPbpPtr;

    double *BSIM4DBdpPtr;
    double *BSIM4DBdbPtr;
    double *BSIM4DBbpPtr;
    double *BSIM4DBbPtr;

    double *BSIM4SBspPtr;
    double *BSIM4SBbpPtr;
    double *BSIM4SBbPtr;
    double *BSIM4SBsbPtr;

    double *BSIM4BdbPtr;
    double *BSIM4BbpPtr;
    double *BSIM4BsbPtr;
    double *BSIM4BbPtr;

    double *BSIM4DgpPtr;
    double *BSIM4DspPtr;
    double *BSIM4DbpPtr;
    double *BSIM4SdpPtr;
    double *BSIM4SgpPtr;
    double *BSIM4SbpPtr;

    double *BSIM4QdpPtr;
    double *BSIM4QgpPtr;
    double *BSIM4QspPtr;
    double *BSIM4QbpPtr;
    double *BSIM4QqPtr;
    double *BSIM4DPqPtr;
    double *BSIM4GPqPtr;
    double *BSIM4SPqPtr;

/* indices to the array of BSIM4 NOISE SOURCES */

#define BSIM4RDNOIZ       0
#define BSIM4RSNOIZ       1
#define BSIM4RGNOIZ       2
#define BSIM4RBPSNOIZ     3
#define BSIM4RBPDNOIZ     4
#define BSIM4RBPBNOIZ     5
#define BSIM4RBSBNOIZ     6
#define BSIM4RBDBNOIZ     7
#define BSIM4IDNOIZ       8
#define BSIM4FLNOIZ       9
#define BSIM4IGSNOIZ      10
#define BSIM4IGDNOIZ      11
#define BSIM4IGBNOIZ      12
#define BSIM4TOTNOIZ      13

#define BSIM4NSRCS        14  /* Number of BSIM4 noise sources */

#ifndef NONOISE
    double BSIM4nVar[NSTATVARS][BSIM4NSRCS];
#else /* NONOISE */
        double **BSIM4nVar;
#endif /* NONOISE */

};

#define BSIM4vbd GENstate + 0
#define BSIM4vbs GENstate + 1
#define BSIM4vgs GENstate + 2
#define BSIM4vds GENstate + 3
#define BSIM4vdbs GENstate + 4
#define BSIM4vdbd GENstate + 5
#define BSIM4vsbs GENstate + 6
#define BSIM4vges GENstate + 7
#define BSIM4vgms GENstate + 8
#define BSIM4vses GENstate + 9
#define BSIM4vdes GENstate + 10

#define BSIM4qb GENstate + 11
#define BSIM4cqb GENstate + 12
#define BSIM4qg GENstate + 13
#define BSIM4cqg GENstate + 14
#define BSIM4qd GENstate + 15
#define BSIM4cqd GENstate + 16
#define BSIM4qgmid GENstate + 17
#define BSIM4cqgmid GENstate + 18

#define BSIM4qbs  GENstate + 19
#define BSIM4cqbs  GENstate + 20
#define BSIM4qbd  GENstate + 21
#define BSIM4cqbd  GENstate + 22

#define BSIM4qcheq GENstate + 23
#define BSIM4cqcheq GENstate + 24
#define BSIM4qcdump GENstate + 25
#define BSIM4cqcdump GENstate + 26
#define BSIM4qdef GENstate + 27
#define BSIM4qs GENstate + 28

// SRW - added for interpolation 
#define BSIM4a_cd      GENstate + 29
#define BSIM4a_cbs     GENstate + 30
#define BSIM4a_cbd     GENstate + 31
#define BSIM4a_gm      GENstate + 32
#define BSIM4a_gds     GENstate + 33
#define BSIM4a_gmbs    GENstate + 34
#define BSIM4a_gbd     GENstate + 35
#define BSIM4a_gbs     GENstate + 36
#define BSIM4a_cggb    GENstate + 37
#define BSIM4a_cgdb    GENstate + 38
#define BSIM4a_cgsb    GENstate + 39
#define BSIM4a_cdgb    GENstate + 40
#define BSIM4a_cddb    GENstate + 41
#define BSIM4a_cdsb    GENstate + 42
#define BSIM4a_cbgb    GENstate + 43
#define BSIM4a_cbdb    GENstate + 44
#define BSIM4a_cbsb    GENstate + 45
#define BSIM4a_capbd   GENstate + 46
#define BSIM4a_capbs   GENstate + 47
#define BSIM4a_von     GENstate + 48
#define BSIM4a_vdsat   GENstate + 49

#define BSIM4a_csub    GENstate + 50
#define BSIM4a_Igidl   GENstate + 51
#define BSIM4a_Igisl   GENstate + 52
#define BSIM4a_Igs     GENstate + 53
#define BSIM4a_Igd     GENstate + 54
#define BSIM4a_Igb     GENstate + 55
#define BSIM4a_Igcs    GENstate + 56
#define BSIM4a_Igcd    GENstate + 57
#define BSIM4a_csgb    GENstate + 58
#define BSIM4a_csdb    GENstate + 59
#define BSIM4a_cssb    GENstate + 60
#define BSIM4a_cgbb    GENstate + 61
#define BSIM4a_cdbb    GENstate + 62
#define BSIM4a_csbb    GENstate + 63
#define BSIM4a_cbbb    GENstate + 64
#define BSIM4a_qinv    GENstate + 65

#define BSIM4a_id      GENstate + 66
#define BSIM4a_is      GENstate + 67
#define BSIM4a_ig      GENstate + 68
#define BSIM4a_ib      GENstate + 69

#define BSIM4a_qbulk   GENstate + 70
#define BSIM4a_qgate   GENstate + 71
#define BSIM4a_qdrn    GENstate + 72
#define BSIM4a_qsrc    GENstate + 73
#define BSIM4a_gcrg    GENstate + 74
#define BSIM4a_gtau    GENstate + 75

#define BSIM4numStates 76

struct sBSIM4instance : sGENinstance, sBSIM4instancePOD
{
    sBSIM4instance() : sGENinstance(), sBSIM4instancePOD()
        { GENnumNodes = 4; }
    ~sBSIM4instance()
        {
            delete BSIM4adjoint;
            delete [] (char*)BSIM4backing;
        }

    sBSIM4instance *next()
        { return (static_cast<sBSIM4instance*>(GENnextInstance)); }

    void backup(DEV_BKMODE m)
        {
            if (m == DEV_SAVE) {
                if (!BSIM4backing)
                    BSIM4backing = new char[sizeof(sBSIM4instance)];
                memcpy(BSIM4backing, this, sizeof(sBSIM4instance));
            }
            else if (m == DEV_RESTORE) {
                if (BSIM4backing)
                    memcpy(this, BSIM4backing, sizeof(sBSIM4instance));
            }
            else {
                // DEV_CLEAR
                delete [] (char*)BSIM4backing;
                BSIM4backing = 0;
            }
        }
};

struct bsim4SizeDependParam
{
    double Width;
    double Length;
    double NFinger;

    double BSIM4cdsc;           
    double BSIM4cdscb;    
    double BSIM4cdscd;       
    double BSIM4cit;           
    double BSIM4nfactor;      
    double BSIM4xj;
    double BSIM4vsat;         
    double BSIM4at;         
    double BSIM4a0;   
    double BSIM4ags;      
    double BSIM4a1;         
    double BSIM4a2;         
    double BSIM4keta;     
    double BSIM4nsub;
    double BSIM4ndep;        
    double BSIM4nsd;
    double BSIM4phin;
    double BSIM4ngate;        
    double BSIM4gamma1;      
    double BSIM4gamma2;     
    double BSIM4vbx;      
    double BSIM4vbi;       
    double BSIM4vbm;       
    double BSIM4xt;       
    double BSIM4phi;
    double BSIM4litl;
    double BSIM4k1;
    double BSIM4kt1;
    double BSIM4kt1l;
    double BSIM4kt2;
    double BSIM4k2;
    double BSIM4k3;
    double BSIM4k3b;
    double BSIM4w0;
    double BSIM4dvtp0;
    double BSIM4dvtp1;
    double BSIM4lpe0;
    double BSIM4lpeb;
    double BSIM4dvt0;      
    double BSIM4dvt1;      
    double BSIM4dvt2;      
    double BSIM4dvt0w;      
    double BSIM4dvt1w;      
    double BSIM4dvt2w;      
    double BSIM4drout;      
    double BSIM4dsub;      
    double BSIM4vth0;
    double BSIM4ua;
    double BSIM4ua1;
    double BSIM4ub;
    double BSIM4ub1;
    double BSIM4uc;
    double BSIM4uc1;
    double BSIM4ud;
    double BSIM4ud1;
    double BSIM4up;
    double BSIM4lp;
    double BSIM4u0;
    double BSIM4eu;
    double BSIM4ucs;
    double BSIM4ute;
    double BSIM4ucste;
    double BSIM4voff;
    double BSIM4tvoff;
    double BSIM4minv;
    double BSIM4minvcv;
    double BSIM4vfb;
    double BSIM4delta;
    double BSIM4rdsw;       
    double BSIM4rds0;       
    double BSIM4rs0;
    double BSIM4rd0;
    double BSIM4rsw;
    double BSIM4rdw;
    double BSIM4prwg;       
    double BSIM4prwb;       
    double BSIM4prt;       
    double BSIM4eta0;         
    double BSIM4etab;         
    double BSIM4pclm;      
    double BSIM4pdibl1;      
    double BSIM4pdibl2;      
    double BSIM4pdiblb;      
    double BSIM4fprout;
    double BSIM4pdits;
    double BSIM4pditsd;
    double BSIM4pscbe1;       
    double BSIM4pscbe2;       
    double BSIM4pvag;       
    double BSIM4wr;
    double BSIM4dwg;
    double BSIM4dwb;
    double BSIM4b0;
    double BSIM4b1;
    double BSIM4alpha0;
    double BSIM4alpha1;
    double BSIM4beta0;
    double BSIM4agidl;
    double BSIM4bgidl;
    double BSIM4cgidl;
    double BSIM4egidl;
    double BSIM4agisl;
    double BSIM4bgisl;
    double BSIM4cgisl;
    double BSIM4egisl;
    double BSIM4aigc;
    double BSIM4bigc;
    double BSIM4cigc;
    double BSIM4aigs;
    double BSIM4bigs;
    double BSIM4cigs;
    double BSIM4aigd;
    double BSIM4bigd;
    double BSIM4cigd;
    double BSIM4aigbacc;
    double BSIM4bigbacc;
    double BSIM4cigbacc;
    double BSIM4aigbinv;
    double BSIM4bigbinv;
    double BSIM4cigbinv;
    double BSIM4nigc;
    double BSIM4nigbacc;
    double BSIM4nigbinv;
    double BSIM4ntox;
    double BSIM4eigbinv;
    double BSIM4pigcd;
    double BSIM4poxedge;
    double BSIM4xrcrg1;
    double BSIM4xrcrg2;
    double BSIM4lambda; /* overshoot */
    double BSIM4vtl; /* thermal velocity limit */
    double BSIM4xn; /* back scattering parameter */
    double BSIM4lc; /* back scattering parameter */
    double BSIM4tfactor;  /* ballistic transportation factor  */
    double BSIM4vfbsdoff;  /* S/D flatband offset voltage  */
    double BSIM4tvfbsdoff;  

/* added for stress effect */
    double BSIM4ku0;
    double BSIM4kvth0;
    double BSIM4ku0temp;
    double BSIM4rho_ref;
    double BSIM4inv_od_ref;
/* added for well proximity effect */
    double BSIM4kvth0we;
    double BSIM4k2we;
    double BSIM4ku0we;

    /* CV model */
    double BSIM4cgsl;
    double BSIM4cgdl;
    double BSIM4ckappas;
    double BSIM4ckappad;
    double BSIM4cf;
    double BSIM4clc;
    double BSIM4cle;
    double BSIM4vfbcv;
    double BSIM4noff;
    double BSIM4voffcv;
    double BSIM4acde;
    double BSIM4moin;

/* Pre-calculated constants */

    double BSIM4dw;
    double BSIM4dl;
    double BSIM4leff;
    double BSIM4weff;

    double BSIM4dwc;
    double BSIM4dlc;
    double BSIM4dwj;
    double BSIM4leffCV;
    double BSIM4weffCV;
    double BSIM4weffCJ;
    double BSIM4abulkCVfactor;
    double BSIM4cgso;
    double BSIM4cgdo;
    double BSIM4cgbo;

    double BSIM4u0temp;       
    double BSIM4vsattemp;   
    double BSIM4sqrtPhi;   
    double BSIM4phis3;   
    double BSIM4Xdep0;          
    double BSIM4sqrtXdep0;          
    double BSIM4theta0vb0;
    double BSIM4thetaRout; 
    double BSIM4mstar;
    double BSIM4VgsteffVth;
    double BSIM4mstarcv;
    double BSIM4voffcbn;
    double BSIM4voffcbncv;
    double BSIM4rdswmin;
    double BSIM4rdwmin;
    double BSIM4rswmin;
    double BSIM4vfbsd;

    double BSIM4cof1;
    double BSIM4cof2;
    double BSIM4cof3;
    double BSIM4cof4;
    double BSIM4cdep0;
    double BSIM4ToxRatio;
    double BSIM4Aechvb;
    double BSIM4Bechvb;
    double BSIM4ToxRatioEdge;
    double BSIM4AechvbEdgeS;
    double BSIM4AechvbEdgeD;
    double BSIM4BechvbEdge;
    double BSIM4ldeb;
    double BSIM4k1ox;
    double BSIM4k2ox;
    double BSIM4vfbzbfactor;

    struct bsim4SizeDependParam  *pNext;
};

struct sBSIM4modelPOD
{
    int BSIM4type;

    int    BSIM4mobMod;
    int    BSIM4cvchargeMod;
    int    BSIM4capMod;
    int    BSIM4dioMod;
    int    BSIM4trnqsMod;
    int    BSIM4acnqsMod;
    int    BSIM4fnoiMod;
    int    BSIM4tnoiMod;
    int    BSIM4rdsMod;
    int    BSIM4rbodyMod;
    int    BSIM4rgateMod;
    int    BSIM4perMod;
    int    BSIM4geoMod;
// SRW, added RGEOMODE as model param
    int    BSIM4rgeoMod;
    int    BSIM4mtrlMod;
    int    BSIM4igcMod;
    int    BSIM4igbMod;
    int    BSIM4tempMod;
    int    BSIM4binUnit;
    int    BSIM4paramChk;
/* SRW */
    const char   *BSIM4version;

    double BSIM4eot;
    double BSIM4vddeot;
    double BSIM4tempeot;
    double BSIM4leffeot;
    double BSIM4weffeot;
    double BSIM4ados;
    double BSIM4bdos;
    double BSIM4toxe;             
    double BSIM4toxp;
    double BSIM4toxm;
    double BSIM4dtox;
    double BSIM4epsrox;
    double BSIM4cdsc;           
    double BSIM4cdscb; 
    double BSIM4cdscd;          
    double BSIM4cit;           
    double BSIM4nfactor;      
    double BSIM4xj;
    double BSIM4vsat;         
    double BSIM4at;         
    double BSIM4a0;   
    double BSIM4ags;      
    double BSIM4a1;         
    double BSIM4a2;         
    double BSIM4keta;     
    double BSIM4nsub;
    double BSIM4phig;
    double BSIM4epsrgate;
    double BSIM4easub;
    double BSIM4epsrsub;
    double BSIM4ni0sub;
    double BSIM4bg0sub;
    double BSIM4tbgasub;
    double BSIM4tbgbsub;
    double BSIM4ndep;        
    double BSIM4nsd;
    double BSIM4phin;
    double BSIM4ngate;        
    double BSIM4gamma1;      
    double BSIM4gamma2;     
    double BSIM4vbx;      
    double BSIM4vbm;       
    double BSIM4xt;       
    double BSIM4k1;
    double BSIM4kt1;
    double BSIM4kt1l;
    double BSIM4kt2;
    double BSIM4k2;
    double BSIM4k3;
    double BSIM4k3b;
    double BSIM4w0;
    double BSIM4dvtp0;
    double BSIM4dvtp1;
    double BSIM4lpe0;
    double BSIM4lpeb;
    double BSIM4dvt0;      
    double BSIM4dvt1;      
    double BSIM4dvt2;      
    double BSIM4dvt0w;      
    double BSIM4dvt1w;      
    double BSIM4dvt2w;      
    double BSIM4drout;      
    double BSIM4dsub;      
    double BSIM4vth0;
    double BSIM4eu;
    double BSIM4ucs;
    double BSIM4ua;
    double BSIM4ua1;
    double BSIM4ub;
    double BSIM4ub1;
    double BSIM4uc;
    double BSIM4uc1;
    double BSIM4ud;
    double BSIM4ud1;
    double BSIM4up;
    double BSIM4lp;
    double BSIM4u0;
    double BSIM4ute;
    double BSIM4ucste;
    double BSIM4voff;
    double BSIM4tvoff;
    double BSIM4minv;
    double BSIM4minvcv;
    double BSIM4voffl;
    double BSIM4voffcvl;
    double BSIM4delta;
    double BSIM4rdsw;       
    double BSIM4rdswmin;
    double BSIM4rdwmin;
    double BSIM4rswmin;
    double BSIM4rsw;
    double BSIM4rdw;
    double BSIM4prwg;
    double BSIM4prwb;
    double BSIM4prt;       
    double BSIM4eta0;         
    double BSIM4etab;         
    double BSIM4pclm;      
    double BSIM4pdibl1;      
    double BSIM4pdibl2;      
    double BSIM4pdiblb;
    double BSIM4fprout;
    double BSIM4pdits;
    double BSIM4pditsd;
    double BSIM4pditsl;
    double BSIM4pscbe1;       
    double BSIM4pscbe2;       
    double BSIM4pvag;       
    double BSIM4wr;
    double BSIM4dwg;
    double BSIM4dwb;
    double BSIM4b0;
    double BSIM4b1;
    double BSIM4alpha0;
    double BSIM4alpha1;
    double BSIM4beta0;
    double BSIM4agidl;
    double BSIM4bgidl;
    double BSIM4cgidl;
    double BSIM4egidl;
    double BSIM4agisl;
    double BSIM4bgisl;
    double BSIM4cgisl;
    double BSIM4egisl;
    double BSIM4aigc;
    double BSIM4bigc;
    double BSIM4cigc;
    double BSIM4aigsd;
    double BSIM4bigsd;
    double BSIM4cigsd;
    double BSIM4aigs;
    double BSIM4bigs;
    double BSIM4cigs;
    double BSIM4aigd;
    double BSIM4bigd;
    double BSIM4cigd;
    double BSIM4aigbacc;
    double BSIM4bigbacc;
    double BSIM4cigbacc;
    double BSIM4aigbinv;
    double BSIM4bigbinv;
    double BSIM4cigbinv;
    double BSIM4nigc;
    double BSIM4nigbacc;
    double BSIM4nigbinv;
    double BSIM4ntox;
    double BSIM4eigbinv;
    double BSIM4pigcd;
    double BSIM4poxedge;
    double BSIM4toxref;
    double BSIM4ijthdfwd;
    double BSIM4ijthsfwd;
    double BSIM4ijthdrev;
    double BSIM4ijthsrev;
    double BSIM4xjbvd;
    double BSIM4xjbvs;
    double BSIM4bvd;
    double BSIM4bvs;

    double BSIM4jtss;
    double BSIM4jtsd;
    double BSIM4jtssws;
    double BSIM4jtsswd;
    double BSIM4jtsswgs;
    double BSIM4jtsswgd;
    double BSIM4jtweff;
    double BSIM4njts;
    double BSIM4njtssw;
    double BSIM4njtsswg;
    double BSIM4njtsd;
    double BSIM4njtsswd;
    double BSIM4njtsswgd;
    double BSIM4xtss;
    double BSIM4xtsd;
    double BSIM4xtssws;
    double BSIM4xtsswd;
    double BSIM4xtsswgs;
    double BSIM4xtsswgd;
    double BSIM4tnjts;
    double BSIM4tnjtssw;
    double BSIM4tnjtsswg;
    double BSIM4tnjtsd;
    double BSIM4tnjtsswd;
    double BSIM4tnjtsswgd;
    double BSIM4vtss;
    double BSIM4vtsd;
    double BSIM4vtssws;
    double BSIM4vtsswd;
    double BSIM4vtsswgs;
    double BSIM4vtsswgd;

    double BSIM4xrcrg1;
    double BSIM4xrcrg2;
    double BSIM4lambda;
    double BSIM4vtl; 
    double BSIM4lc; 
    double BSIM4xn; 
    double BSIM4vfbsdoff;  /* S/D flatband offset voltage  */
    double BSIM4lintnoi;  /* lint offset for noise calculation  */
    double BSIM4tvfbsdoff;  

    double BSIM4vfb;
    double BSIM4gbmin;
    double BSIM4rbdb;
    double BSIM4rbsb;
    double BSIM4rbpb;
    double BSIM4rbps;
    double BSIM4rbpd;

    double BSIM4rbps0;
    double BSIM4rbpsl;
    double BSIM4rbpsw;
    double BSIM4rbpsnf;

    double BSIM4rbpd0;
    double BSIM4rbpdl;
    double BSIM4rbpdw;
    double BSIM4rbpdnf;

    double BSIM4rbpbx0;
    double BSIM4rbpbxl;
    double BSIM4rbpbxw;
    double BSIM4rbpbxnf;
    double BSIM4rbpby0;
    double BSIM4rbpbyl;
    double BSIM4rbpbyw;
    double BSIM4rbpbynf;

    double BSIM4rbsbx0;
    double BSIM4rbsby0;
    double BSIM4rbdbx0;
    double BSIM4rbdby0;

    double BSIM4rbsdbxl;
    double BSIM4rbsdbxw;
    double BSIM4rbsdbxnf;
    double BSIM4rbsdbyl;
    double BSIM4rbsdbyw;
    double BSIM4rbsdbynf;

    double BSIM4tnoia;
    double BSIM4tnoib;
    double BSIM4rnoia;
    double BSIM4rnoib;
    double BSIM4ntnoi;

    /* CV model and Parasitics */
    double BSIM4cgsl;
    double BSIM4cgdl;
    double BSIM4ckappas;
    double BSIM4ckappad;
    double BSIM4cf;
    double BSIM4vfbcv;
    double BSIM4clc;
    double BSIM4cle;
    double BSIM4dwc;
    double BSIM4dlc;
    double BSIM4xw;
    double BSIM4xl;
    double BSIM4dlcig;
    double BSIM4dlcigd;
    double BSIM4dwj;
    double BSIM4noff;
    double BSIM4voffcv;
    double BSIM4acde;
    double BSIM4moin;
    double BSIM4tcj;
    double BSIM4tcjsw;
    double BSIM4tcjswg;
    double BSIM4tpb;
    double BSIM4tpbsw;
    double BSIM4tpbswg;
    double BSIM4dmcg;
    double BSIM4dmci;
    double BSIM4dmdg;
    double BSIM4dmcgt;
    double BSIM4xgw;
    double BSIM4xgl;
    double BSIM4rshg;
    double BSIM4ngcon;

    /* Length Dependence */
    double BSIM4lcdsc;           
    double BSIM4lcdscb; 
    double BSIM4lcdscd;          
    double BSIM4lcit;           
    double BSIM4lnfactor;      
    double BSIM4lxj;
    double BSIM4lvsat;         
    double BSIM4lat;         
    double BSIM4la0;   
    double BSIM4lags;      
    double BSIM4la1;         
    double BSIM4la2;         
    double BSIM4lketa;     
    double BSIM4lnsub;
    double BSIM4lndep;        
    double BSIM4lnsd;
    double BSIM4lphin;
    double BSIM4lngate;        
    double BSIM4lgamma1;      
    double BSIM4lgamma2;     
    double BSIM4lvbx;      
    double BSIM4lvbm;       
    double BSIM4lxt;       
    double BSIM4lk1;
    double BSIM4lkt1;
    double BSIM4lkt1l;
    double BSIM4lkt2;
    double BSIM4lk2;
    double BSIM4lk3;
    double BSIM4lk3b;
    double BSIM4lw0;
    double BSIM4ldvtp0;
    double BSIM4ldvtp1;
    double BSIM4llpe0;
    double BSIM4llpeb;
    double BSIM4ldvt0;      
    double BSIM4ldvt1;      
    double BSIM4ldvt2;      
    double BSIM4ldvt0w;      
    double BSIM4ldvt1w;      
    double BSIM4ldvt2w;      
    double BSIM4ldrout;      
    double BSIM4ldsub;      
    double BSIM4lvth0;
    double BSIM4lua;
    double BSIM4lua1;
    double BSIM4lub;
    double BSIM4lub1;
    double BSIM4luc;
    double BSIM4luc1;
    double BSIM4lud;
    double BSIM4lud1;
    double BSIM4lup;
    double BSIM4llp;
    double BSIM4lu0;
    double BSIM4leu;
    double BSIM4lucs;
    double BSIM4lute;
    double BSIM4lucste;
    double BSIM4lvoff;
    double BSIM4ltvoff;
    double BSIM4lminv;
    double BSIM4lminvcv;
    double BSIM4ldelta;
    double BSIM4lrdsw;       
    double BSIM4lrsw;
    double BSIM4lrdw;
    double BSIM4lprwg;
    double BSIM4lprwb;
    double BSIM4lprt;       
    double BSIM4leta0;         
    double BSIM4letab;         
    double BSIM4lpclm;      
    double BSIM4lpdibl1;      
    double BSIM4lpdibl2;      
    double BSIM4lpdiblb;
    double BSIM4lfprout;
    double BSIM4lpdits;
    double BSIM4lpditsd;
    double BSIM4lpscbe1;       
    double BSIM4lpscbe2;       
    double BSIM4lpvag;       
    double BSIM4lwr;
    double BSIM4ldwg;
    double BSIM4ldwb;
    double BSIM4lb0;
    double BSIM4lb1;
    double BSIM4lalpha0;
    double BSIM4lalpha1;
    double BSIM4lbeta0;
    double BSIM4lvfb;
    double BSIM4lagidl;
    double BSIM4lbgidl;
    double BSIM4lcgidl;
    double BSIM4legidl;
    double BSIM4lagisl;
    double BSIM4lbgisl;
    double BSIM4lcgisl;
    double BSIM4legisl;
    double BSIM4laigc;
    double BSIM4lbigc;
    double BSIM4lcigc;
    double BSIM4laigsd;
    double BSIM4lbigsd;
    double BSIM4lcigsd;
    double BSIM4laigs;
    double BSIM4lbigs;
    double BSIM4lcigs;
    double BSIM4laigd;
    double BSIM4lbigd;
    double BSIM4lcigd;
    double BSIM4laigbacc;
    double BSIM4lbigbacc;
    double BSIM4lcigbacc;
    double BSIM4laigbinv;
    double BSIM4lbigbinv;
    double BSIM4lcigbinv;
    double BSIM4lnigc;
    double BSIM4lnigbacc;
    double BSIM4lnigbinv;
    double BSIM4lntox;
    double BSIM4leigbinv;
    double BSIM4lpigcd;
    double BSIM4lpoxedge;
    double BSIM4lxrcrg1;
    double BSIM4lxrcrg2;
    double BSIM4llambda;
    double BSIM4lvtl; 
    double BSIM4lxn; 
    double BSIM4lvfbsdoff; 
    double BSIM4ltvfbsdoff; 

    /* CV model */
    double BSIM4lcgsl;
    double BSIM4lcgdl;
    double BSIM4lckappas;
    double BSIM4lckappad;
    double BSIM4lcf;
    double BSIM4lclc;
    double BSIM4lcle;
    double BSIM4lvfbcv;
    double BSIM4lnoff;
    double BSIM4lvoffcv;
    double BSIM4lacde;
    double BSIM4lmoin;

    /* Width Dependence */
    double BSIM4wcdsc;           
    double BSIM4wcdscb; 
    double BSIM4wcdscd;          
    double BSIM4wcit;           
    double BSIM4wnfactor;      
    double BSIM4wxj;
    double BSIM4wvsat;         
    double BSIM4wat;         
    double BSIM4wa0;   
    double BSIM4wags;      
    double BSIM4wa1;         
    double BSIM4wa2;         
    double BSIM4wketa;     
    double BSIM4wnsub;
    double BSIM4wndep;        
    double BSIM4wnsd;
    double BSIM4wphin;
    double BSIM4wngate;        
    double BSIM4wgamma1;      
    double BSIM4wgamma2;     
    double BSIM4wvbx;      
    double BSIM4wvbm;       
    double BSIM4wxt;       
    double BSIM4wk1;
    double BSIM4wkt1;
    double BSIM4wkt1l;
    double BSIM4wkt2;
    double BSIM4wk2;
    double BSIM4wk3;
    double BSIM4wk3b;
    double BSIM4ww0;
    double BSIM4wdvtp0;
    double BSIM4wdvtp1;
    double BSIM4wlpe0;
    double BSIM4wlpeb;
    double BSIM4wdvt0;      
    double BSIM4wdvt1;      
    double BSIM4wdvt2;      
    double BSIM4wdvt0w;      
    double BSIM4wdvt1w;      
    double BSIM4wdvt2w;      
    double BSIM4wdrout;      
    double BSIM4wdsub;      
    double BSIM4wvth0;
    double BSIM4wua;
    double BSIM4wua1;
    double BSIM4wub;
    double BSIM4wub1;
    double BSIM4wuc;
    double BSIM4wuc1;
    double BSIM4wud;
    double BSIM4wud1;
    double BSIM4wup;
    double BSIM4wlp;
    double BSIM4wu0;
    double BSIM4weu;
    double BSIM4wucs;
    double BSIM4wute;
    double BSIM4wucste;
    double BSIM4wvoff;
    double BSIM4wtvoff;
    double BSIM4wminv;
    double BSIM4wminvcv;
    double BSIM4wdelta;
    double BSIM4wrdsw;       
    double BSIM4wrsw;
    double BSIM4wrdw;
    double BSIM4wprwg;
    double BSIM4wprwb;
    double BSIM4wprt;       
    double BSIM4weta0;         
    double BSIM4wetab;         
    double BSIM4wpclm;      
    double BSIM4wpdibl1;      
    double BSIM4wpdibl2;      
    double BSIM4wpdiblb;
    double BSIM4wfprout;
    double BSIM4wpdits;
    double BSIM4wpditsd;
    double BSIM4wpscbe1;       
    double BSIM4wpscbe2;       
    double BSIM4wpvag;       
    double BSIM4wwr;
    double BSIM4wdwg;
    double BSIM4wdwb;
    double BSIM4wb0;
    double BSIM4wb1;
    double BSIM4walpha0;
    double BSIM4walpha1;
    double BSIM4wbeta0;
    double BSIM4wvfb;
    double BSIM4wagidl;
    double BSIM4wbgidl;
    double BSIM4wcgidl;
    double BSIM4wegidl;
    double BSIM4wagisl;
    double BSIM4wbgisl;
    double BSIM4wcgisl;
    double BSIM4wegisl;
    double BSIM4waigc;
    double BSIM4wbigc;
    double BSIM4wcigc;
    double BSIM4waigsd;
    double BSIM4wbigsd;
    double BSIM4wcigsd;
    double BSIM4waigs;
    double BSIM4wbigs;
    double BSIM4wcigs;
    double BSIM4waigd;
    double BSIM4wbigd;
    double BSIM4wcigd;
    double BSIM4waigbacc;
    double BSIM4wbigbacc;
    double BSIM4wcigbacc;
    double BSIM4waigbinv;
    double BSIM4wbigbinv;
    double BSIM4wcigbinv;
    double BSIM4wnigc;
    double BSIM4wnigbacc;
    double BSIM4wnigbinv;
    double BSIM4wntox;
    double BSIM4weigbinv;
    double BSIM4wpigcd;
    double BSIM4wpoxedge;
    double BSIM4wxrcrg1;
    double BSIM4wxrcrg2;
    double BSIM4wlambda;
    double BSIM4wvtl; 
    double BSIM4wxn; 
    double BSIM4wvfbsdoff;  
    double BSIM4wtvfbsdoff;  

    /* CV model */
    double BSIM4wcgsl;
    double BSIM4wcgdl;
    double BSIM4wckappas;
    double BSIM4wckappad;
    double BSIM4wcf;
    double BSIM4wclc;
    double BSIM4wcle;
    double BSIM4wvfbcv;
    double BSIM4wnoff;
    double BSIM4wvoffcv;
    double BSIM4wacde;
    double BSIM4wmoin;

    /* Cross-term Dependence */
    double BSIM4pcdsc;           
    double BSIM4pcdscb; 
    double BSIM4pcdscd;          
    double BSIM4pcit;           
    double BSIM4pnfactor;      
    double BSIM4pxj;
    double BSIM4pvsat;         
    double BSIM4pat;         
    double BSIM4pa0;   
    double BSIM4pags;      
    double BSIM4pa1;         
    double BSIM4pa2;         
    double BSIM4pketa;     
    double BSIM4pnsub;
    double BSIM4pndep;        
    double BSIM4pnsd;
    double BSIM4pphin;
    double BSIM4pngate;        
    double BSIM4pgamma1;      
    double BSIM4pgamma2;     
    double BSIM4pvbx;      
    double BSIM4pvbm;       
    double BSIM4pxt;       
    double BSIM4pk1;
    double BSIM4pkt1;
    double BSIM4pkt1l;
    double BSIM4pkt2;
    double BSIM4pk2;
    double BSIM4pk3;
    double BSIM4pk3b;
    double BSIM4pw0;
    double BSIM4pdvtp0;
    double BSIM4pdvtp1;
    double BSIM4plpe0;
    double BSIM4plpeb;
    double BSIM4pdvt0;      
    double BSIM4pdvt1;      
    double BSIM4pdvt2;      
    double BSIM4pdvt0w;      
    double BSIM4pdvt1w;      
    double BSIM4pdvt2w;      
    double BSIM4pdrout;      
    double BSIM4pdsub;      
    double BSIM4pvth0;
    double BSIM4pua;
    double BSIM4pua1;
    double BSIM4pub;
    double BSIM4pub1;
    double BSIM4puc;
    double BSIM4puc1;
    double BSIM4pud;
    double BSIM4pud1;
    double BSIM4pup;
    double BSIM4plp;
    double BSIM4pu0;
    double BSIM4peu;
    double BSIM4pucs;
    double BSIM4pute;
    double BSIM4pucste;
    double BSIM4pvoff;
    double BSIM4ptvoff;
    double BSIM4pminv;
    double BSIM4pminvcv;
    double BSIM4pdelta;
    double BSIM4prdsw;
    double BSIM4prsw;
    double BSIM4prdw;
    double BSIM4pprwg;
    double BSIM4pprwb;
    double BSIM4pprt;       
    double BSIM4peta0;         
    double BSIM4petab;         
    double BSIM4ppclm;      
    double BSIM4ppdibl1;      
    double BSIM4ppdibl2;      
    double BSIM4ppdiblb;
    double BSIM4pfprout;
    double BSIM4ppdits;
    double BSIM4ppditsd;
    double BSIM4ppscbe1;       
    double BSIM4ppscbe2;       
    double BSIM4ppvag;       
    double BSIM4pwr;
    double BSIM4pdwg;
    double BSIM4pdwb;
    double BSIM4pb0;
    double BSIM4pb1;
    double BSIM4palpha0;
    double BSIM4palpha1;
    double BSIM4pbeta0;
    double BSIM4pvfb;
    double BSIM4pagidl;
    double BSIM4pbgidl;
    double BSIM4pcgidl;
    double BSIM4pegidl;
    double BSIM4pagisl;
    double BSIM4pbgisl;
    double BSIM4pcgisl;
    double BSIM4pegisl;
    double BSIM4paigc;
    double BSIM4pbigc;
    double BSIM4pcigc;
    double BSIM4paigsd;
    double BSIM4pbigsd;
    double BSIM4pcigsd;
    double BSIM4paigs;
    double BSIM4pbigs;
    double BSIM4pcigs;
    double BSIM4paigd;
    double BSIM4pbigd;
    double BSIM4pcigd;
    double BSIM4paigbacc;
    double BSIM4pbigbacc;
    double BSIM4pcigbacc;
    double BSIM4paigbinv;
    double BSIM4pbigbinv;
    double BSIM4pcigbinv;
    double BSIM4pnigc;
    double BSIM4pnigbacc;
    double BSIM4pnigbinv;
    double BSIM4pntox;
    double BSIM4peigbinv;
    double BSIM4ppigcd;
    double BSIM4ppoxedge;
    double BSIM4pxrcrg1;
    double BSIM4pxrcrg2;
    double BSIM4plambda;
    double BSIM4pvtl;
    double BSIM4pxn; 
    double BSIM4pvfbsdoff;  
    double BSIM4ptvfbsdoff;  

    /* CV model */
    double BSIM4pcgsl;
    double BSIM4pcgdl;
    double BSIM4pckappas;
    double BSIM4pckappad;
    double BSIM4pcf;
    double BSIM4pclc;
    double BSIM4pcle;
    double BSIM4pvfbcv;
    double BSIM4pnoff;
    double BSIM4pvoffcv;
    double BSIM4pacde;
    double BSIM4pmoin;

    double BSIM4tnom;
    double BSIM4cgso;
    double BSIM4cgdo;
    double BSIM4cgbo;
    double BSIM4xpart;
    double BSIM4cFringOut;
    double BSIM4cFringMax;

    double BSIM4sheetResistance;
    double BSIM4SjctSatCurDensity;
    double BSIM4DjctSatCurDensity;
    double BSIM4SjctSidewallSatCurDensity;
    double BSIM4DjctSidewallSatCurDensity;
    double BSIM4SjctGateSidewallSatCurDensity;
    double BSIM4DjctGateSidewallSatCurDensity;
    double BSIM4SbulkJctPotential;
    double BSIM4DbulkJctPotential;
    double BSIM4SbulkJctBotGradingCoeff;
    double BSIM4DbulkJctBotGradingCoeff;
    double BSIM4SbulkJctSideGradingCoeff;
    double BSIM4DbulkJctSideGradingCoeff;
    double BSIM4SbulkJctGateSideGradingCoeff;
    double BSIM4DbulkJctGateSideGradingCoeff;
    double BSIM4SsidewallJctPotential;
    double BSIM4DsidewallJctPotential;
    double BSIM4SGatesidewallJctPotential;
    double BSIM4DGatesidewallJctPotential;
    double BSIM4SunitAreaJctCap;
    double BSIM4DunitAreaJctCap;
    double BSIM4SunitLengthSidewallJctCap;
    double BSIM4DunitLengthSidewallJctCap;
    double BSIM4SunitLengthGateSidewallJctCap;
    double BSIM4DunitLengthGateSidewallJctCap;
    double BSIM4SjctEmissionCoeff;
    double BSIM4DjctEmissionCoeff;
    double BSIM4SjctTempExponent;
    double BSIM4DjctTempExponent;
    double BSIM4njtsstemp;
    double BSIM4njtsswstemp;
    double BSIM4njtsswgstemp;
    double BSIM4njtsdtemp;
    double BSIM4njtsswdtemp;
    double BSIM4njtsswgdtemp;

    double BSIM4Lint;
    double BSIM4Ll;
    double BSIM4Llc;
    double BSIM4Lln;
    double BSIM4Lw;
    double BSIM4Lwc;
    double BSIM4Lwn;
    double BSIM4Lwl;
    double BSIM4Lwlc;
    double BSIM4Lmin;
    double BSIM4Lmax;

    double BSIM4Wint;
    double BSIM4Wl;
    double BSIM4Wlc;
    double BSIM4Wln;
    double BSIM4Ww;
    double BSIM4Wwc;
    double BSIM4Wwn;
    double BSIM4Wwl;
    double BSIM4Wwlc;
    double BSIM4Wmin;
    double BSIM4Wmax;

    /* added for stress effect */
    double BSIM4saref;
    double BSIM4sbref;
    double BSIM4wlod;
    double BSIM4ku0;
    double BSIM4kvsat;
    double BSIM4kvth0;
    double BSIM4tku0;
    double BSIM4llodku0;
    double BSIM4wlodku0;
    double BSIM4llodvth;
    double BSIM4wlodvth;
    double BSIM4lku0;
    double BSIM4wku0;
    double BSIM4pku0;
    double BSIM4lkvth0;
    double BSIM4wkvth0;
    double BSIM4pkvth0;
    double BSIM4stk2;
    double BSIM4lodk2;
    double BSIM4steta0;
    double BSIM4lodeta0;

    double BSIM4web; 
    double BSIM4wec;
    double BSIM4kvth0we; 
    double BSIM4k2we; 
    double BSIM4ku0we; 
    double BSIM4scref; 
    double BSIM4wpemod; 
    double BSIM4lkvth0we;
    double BSIM4lk2we;
    double BSIM4lku0we;
    double BSIM4wkvth0we;
    double BSIM4wk2we;
    double BSIM4wku0we;
    double BSIM4pkvth0we;
    double BSIM4pk2we;
    double BSIM4pku0we;

/* Pre-calculated constants
 * move to size-dependent param */
    double BSIM4Eg0;
    double BSIM4vtm;   
    double BSIM4vtm0;   
    double BSIM4coxe;
    double BSIM4coxp;
    double BSIM4cof1;
    double BSIM4cof2;
    double BSIM4cof3;
    double BSIM4cof4;
    double BSIM4vcrit;
    double BSIM4factor1;
    double BSIM4PhiBS;
    double BSIM4PhiBSWS;
    double BSIM4PhiBSWGS;
    double BSIM4SjctTempSatCurDensity;
    double BSIM4SjctSidewallTempSatCurDensity;
    double BSIM4SjctGateSidewallTempSatCurDensity;
    double BSIM4PhiBD;
    double BSIM4PhiBSWD;
    double BSIM4PhiBSWGD;
    double BSIM4DjctTempSatCurDensity;
    double BSIM4DjctSidewallTempSatCurDensity;
    double BSIM4DjctGateSidewallTempSatCurDensity;
    double BSIM4SunitAreaTempJctCap;
    double BSIM4DunitAreaTempJctCap;
    double BSIM4SunitLengthSidewallTempJctCap;
    double BSIM4DunitLengthSidewallTempJctCap;
    double BSIM4SunitLengthGateSidewallTempJctCap;
    double BSIM4DunitLengthGateSidewallTempJctCap;

    double BSIM4oxideTrapDensityA;      
    double BSIM4oxideTrapDensityB;     
    double BSIM4oxideTrapDensityC;  
    double BSIM4em;  
    double BSIM4ef;  
    double BSIM4af;  
    double BSIM4kf;  

    struct bsim4SizeDependParam *pSizeDependParamKnot;

    /* Flags */
    unsigned  BSIM4mobModGiven :1;
    unsigned  BSIM4binUnitGiven :1;
    unsigned  BSIM4cvchargeModGiven :1;
    unsigned  BSIM4capModGiven :1;
    unsigned  BSIM4dioModGiven :1;
    unsigned  BSIM4rdsModGiven :1;
    unsigned  BSIM4rbodyModGiven :1;
    unsigned  BSIM4rgateModGiven :1;
    unsigned  BSIM4perModGiven :1;
    unsigned  BSIM4geoModGiven :1;
// SRW, added RGEOMODE as model param
    unsigned  BSIM4rgeoModGiven :1;
    unsigned  BSIM4paramChkGiven :1;
    unsigned  BSIM4trnqsModGiven :1;
    unsigned  BSIM4acnqsModGiven :1;
    unsigned  BSIM4fnoiModGiven :1;
    unsigned  BSIM4tnoiModGiven :1;
    unsigned  BSIM4mtrlModGiven :1;
    unsigned  BSIM4igcModGiven :1;
    unsigned  BSIM4igbModGiven :1;
    unsigned  BSIM4tempModGiven :1;
    unsigned  BSIM4typeGiven   :1;
    unsigned  BSIM4toxrefGiven   :1;
    unsigned  BSIM4eotGiven   :1;
    unsigned  BSIM4vddeotGiven   :1;
    unsigned  BSIM4tempeotGiven  :1;
    unsigned  BSIM4leffeotGiven  :1;
    unsigned  BSIM4weffeotGiven  :1;
    unsigned  BSIM4adosGiven   :1;
    unsigned  BSIM4bdosGiven   :1;
    unsigned  BSIM4toxeGiven   :1;
    unsigned  BSIM4toxpGiven   :1;
    unsigned  BSIM4toxmGiven   :1;
    unsigned  BSIM4dtoxGiven   :1;
    unsigned  BSIM4epsroxGiven   :1;
    unsigned  BSIM4versionGiven   :1;
    unsigned  BSIM4cdscGiven   :1;
    unsigned  BSIM4cdscbGiven   :1;
    unsigned  BSIM4cdscdGiven   :1;
    unsigned  BSIM4citGiven   :1;
    unsigned  BSIM4nfactorGiven   :1;
    unsigned  BSIM4xjGiven   :1;
    unsigned  BSIM4vsatGiven   :1;
    unsigned  BSIM4atGiven   :1;
    unsigned  BSIM4a0Given   :1;
    unsigned  BSIM4agsGiven   :1;
    unsigned  BSIM4a1Given   :1;
    unsigned  BSIM4a2Given   :1;
    unsigned  BSIM4ketaGiven   :1;    
    unsigned  BSIM4nsubGiven   :1;
    unsigned  BSIM4phigGiven   :1;
    unsigned  BSIM4epsrgateGiven   :1;
    unsigned  BSIM4easubGiven   :1;
    unsigned  BSIM4epsrsubGiven   :1;
    unsigned  BSIM4ni0subGiven   :1;
    unsigned  BSIM4bg0subGiven   :1;
    unsigned  BSIM4tbgasubGiven   :1;
    unsigned  BSIM4tbgbsubGiven   :1;
    unsigned  BSIM4ndepGiven   :1;
    unsigned  BSIM4nsdGiven    :1;
    unsigned  BSIM4phinGiven   :1;
    unsigned  BSIM4ngateGiven   :1;
    unsigned  BSIM4gamma1Given   :1;
    unsigned  BSIM4gamma2Given   :1;
    unsigned  BSIM4vbxGiven   :1;
    unsigned  BSIM4vbmGiven   :1;
    unsigned  BSIM4xtGiven   :1;
    unsigned  BSIM4k1Given   :1;
    unsigned  BSIM4kt1Given   :1;
    unsigned  BSIM4kt1lGiven   :1;
    unsigned  BSIM4kt2Given   :1;
    unsigned  BSIM4k2Given   :1;
    unsigned  BSIM4k3Given   :1;
    unsigned  BSIM4k3bGiven   :1;
    unsigned  BSIM4w0Given   :1;
    unsigned  BSIM4dvtp0Given :1;
    unsigned  BSIM4dvtp1Given :1;
    unsigned  BSIM4lpe0Given   :1;
    unsigned  BSIM4lpebGiven   :1;
    unsigned  BSIM4dvt0Given   :1;   
    unsigned  BSIM4dvt1Given   :1;     
    unsigned  BSIM4dvt2Given   :1;     
    unsigned  BSIM4dvt0wGiven   :1;   
    unsigned  BSIM4dvt1wGiven   :1;     
    unsigned  BSIM4dvt2wGiven   :1;     
    unsigned  BSIM4droutGiven   :1;     
    unsigned  BSIM4dsubGiven   :1;    
    unsigned  BSIM4vth0Given   :1;
    unsigned  BSIM4euGiven   :1;
    unsigned  BSIM4ucsGiven  :1;
    unsigned  BSIM4uaGiven   :1;
    unsigned  BSIM4ua1Given   :1;
    unsigned  BSIM4ubGiven   :1;
    unsigned  BSIM4ub1Given   :1;
    unsigned  BSIM4ucGiven   :1;
    unsigned  BSIM4uc1Given   :1;
    unsigned  BSIM4udGiven     :1;
    unsigned  BSIM4ud1Given     :1;
    unsigned  BSIM4upGiven     :1;
    unsigned  BSIM4lpGiven     :1;
    unsigned  BSIM4u0Given   :1;
    unsigned  BSIM4uteGiven   :1;
    unsigned  BSIM4ucsteGiven :1;
    unsigned  BSIM4voffGiven   :1;
    unsigned  BSIM4tvoffGiven   :1;
    unsigned  BSIM4vofflGiven  :1;
    unsigned  BSIM4voffcvlGiven  :1;
    unsigned  BSIM4minvGiven   :1;
    unsigned  BSIM4minvcvGiven   :1;
    unsigned  BSIM4rdswGiven   :1;      
    unsigned  BSIM4rdswminGiven :1;
    unsigned  BSIM4rdwminGiven :1;
    unsigned  BSIM4rswminGiven :1;
    unsigned  BSIM4rswGiven   :1;
    unsigned  BSIM4rdwGiven   :1;
    unsigned  BSIM4prwgGiven   :1;      
    unsigned  BSIM4prwbGiven   :1;      
    unsigned  BSIM4prtGiven   :1;      
    unsigned  BSIM4eta0Given   :1;    
    unsigned  BSIM4etabGiven   :1;    
    unsigned  BSIM4pclmGiven   :1;   
    unsigned  BSIM4pdibl1Given   :1;   
    unsigned  BSIM4pdibl2Given   :1;  
    unsigned  BSIM4pdiblbGiven   :1;  
    unsigned  BSIM4fproutGiven   :1;
    unsigned  BSIM4pditsGiven    :1;
    unsigned  BSIM4pditsdGiven    :1;
    unsigned  BSIM4pditslGiven    :1;
    unsigned  BSIM4pscbe1Given   :1;    
    unsigned  BSIM4pscbe2Given   :1;    
    unsigned  BSIM4pvagGiven   :1;    
    unsigned  BSIM4deltaGiven  :1;     
    unsigned  BSIM4wrGiven   :1;
    unsigned  BSIM4dwgGiven   :1;
    unsigned  BSIM4dwbGiven   :1;
    unsigned  BSIM4b0Given   :1;
    unsigned  BSIM4b1Given   :1;
    unsigned  BSIM4alpha0Given   :1;
    unsigned  BSIM4alpha1Given   :1;
    unsigned  BSIM4beta0Given   :1;
    unsigned  BSIM4agidlGiven   :1;
    unsigned  BSIM4bgidlGiven   :1;
    unsigned  BSIM4cgidlGiven   :1;
    unsigned  BSIM4egidlGiven   :1;
    unsigned  BSIM4agislGiven   :1;
    unsigned  BSIM4bgislGiven   :1;
    unsigned  BSIM4cgislGiven   :1;
    unsigned  BSIM4egislGiven   :1;
    unsigned  BSIM4aigcGiven   :1;
    unsigned  BSIM4bigcGiven   :1;
    unsigned  BSIM4cigcGiven   :1;
    unsigned  BSIM4aigsdGiven   :1;
    unsigned  BSIM4bigsdGiven   :1;
    unsigned  BSIM4cigsdGiven   :1;
    unsigned  BSIM4aigsGiven   :1;
    unsigned  BSIM4bigsGiven   :1;
    unsigned  BSIM4cigsGiven   :1;
    unsigned  BSIM4aigdGiven   :1;
    unsigned  BSIM4bigdGiven   :1;
    unsigned  BSIM4cigdGiven   :1;
    unsigned  BSIM4aigbaccGiven   :1;
    unsigned  BSIM4bigbaccGiven   :1;
    unsigned  BSIM4cigbaccGiven   :1;
    unsigned  BSIM4aigbinvGiven   :1;
    unsigned  BSIM4bigbinvGiven   :1;
    unsigned  BSIM4cigbinvGiven   :1;
    unsigned  BSIM4nigcGiven   :1;
    unsigned  BSIM4nigbinvGiven   :1;
    unsigned  BSIM4nigbaccGiven   :1;
    unsigned  BSIM4ntoxGiven   :1;
    unsigned  BSIM4eigbinvGiven   :1;
    unsigned  BSIM4pigcdGiven   :1;
    unsigned  BSIM4poxedgeGiven   :1;
    unsigned  BSIM4ijthdfwdGiven  :1;
    unsigned  BSIM4ijthsfwdGiven  :1;
    unsigned  BSIM4ijthdrevGiven  :1;
    unsigned  BSIM4ijthsrevGiven  :1;
    unsigned  BSIM4xjbvdGiven   :1;
    unsigned  BSIM4xjbvsGiven   :1;
    unsigned  BSIM4bvdGiven   :1;
    unsigned  BSIM4bvsGiven   :1;

    unsigned  BSIM4jtssGiven   :1;
    unsigned  BSIM4jtsdGiven   :1;
    unsigned  BSIM4jtsswsGiven   :1;
    unsigned  BSIM4jtsswdGiven   :1;
    unsigned  BSIM4jtsswgsGiven   :1;
    unsigned  BSIM4jtsswgdGiven   :1;
    unsigned  BSIM4jtweffGiven    :1;
    unsigned  BSIM4njtsGiven   :1;
    unsigned  BSIM4njtsswGiven   :1;
    unsigned  BSIM4njtsswgGiven   :1;
    unsigned  BSIM4njtsdGiven   :1;
    unsigned  BSIM4njtsswdGiven   :1;
    unsigned  BSIM4njtsswgdGiven   :1;
    unsigned  BSIM4xtssGiven   :1;
    unsigned  BSIM4xtsdGiven   :1;
    unsigned  BSIM4xtsswsGiven   :1;
    unsigned  BSIM4xtsswdGiven   :1;
    unsigned  BSIM4xtsswgsGiven   :1;
    unsigned  BSIM4xtsswgdGiven   :1;
    unsigned  BSIM4tnjtsGiven   :1;
    unsigned  BSIM4tnjtsswGiven   :1;
    unsigned  BSIM4tnjtsswgGiven   :1;
    unsigned  BSIM4tnjtsdGiven   :1;
    unsigned  BSIM4tnjtsswdGiven   :1;
    unsigned  BSIM4tnjtsswgdGiven   :1;
    unsigned  BSIM4vtssGiven   :1;
    unsigned  BSIM4vtsdGiven   :1;
    unsigned  BSIM4vtsswsGiven   :1;
    unsigned  BSIM4vtsswdGiven   :1;
    unsigned  BSIM4vtsswgsGiven   :1;
    unsigned  BSIM4vtsswgdGiven   :1;

    unsigned  BSIM4vfbGiven   :1;
    unsigned  BSIM4gbminGiven :1;
    unsigned  BSIM4rbdbGiven :1;
    unsigned  BSIM4rbsbGiven :1;
    unsigned  BSIM4rbpsGiven :1;
    unsigned  BSIM4rbpdGiven :1;
    unsigned  BSIM4rbpbGiven :1;

    unsigned BSIM4rbps0Given :1;
    unsigned BSIM4rbpslGiven :1;
    unsigned BSIM4rbpswGiven :1;
    unsigned BSIM4rbpsnfGiven :1;

    unsigned BSIM4rbpd0Given :1;
    unsigned BSIM4rbpdlGiven :1;
    unsigned BSIM4rbpdwGiven :1;
    unsigned BSIM4rbpdnfGiven :1;

    unsigned BSIM4rbpbx0Given :1;
    unsigned BSIM4rbpbxlGiven :1;
    unsigned BSIM4rbpbxwGiven :1;
    unsigned BSIM4rbpbxnfGiven :1;
    unsigned BSIM4rbpby0Given :1;
    unsigned BSIM4rbpbylGiven :1;
    unsigned BSIM4rbpbywGiven :1;
    unsigned BSIM4rbpbynfGiven :1;

    unsigned BSIM4rbsbx0Given :1;
    unsigned BSIM4rbsby0Given :1;
    unsigned BSIM4rbdbx0Given :1;
    unsigned BSIM4rbdby0Given :1;

    unsigned BSIM4rbsdbxlGiven :1;
    unsigned BSIM4rbsdbxwGiven :1;
    unsigned BSIM4rbsdbxnfGiven :1;
    unsigned BSIM4rbsdbylGiven :1;
    unsigned BSIM4rbsdbywGiven :1;
    unsigned BSIM4rbsdbynfGiven :1;

    unsigned  BSIM4xrcrg1Given   :1;
    unsigned  BSIM4xrcrg2Given   :1;
    unsigned  BSIM4tnoiaGiven    :1;
    unsigned  BSIM4tnoibGiven    :1;
    unsigned  BSIM4rnoiaGiven    :1;
    unsigned  BSIM4rnoibGiven    :1;
    unsigned  BSIM4ntnoiGiven    :1;

    unsigned  BSIM4lambdaGiven    :1;
    unsigned  BSIM4vtlGiven    :1;
    unsigned  BSIM4lcGiven    :1;
    unsigned  BSIM4xnGiven    :1;
    unsigned  BSIM4vfbsdoffGiven    :1;
    unsigned  BSIM4lintnoiGiven    :1;
    unsigned  BSIM4tvfbsdoffGiven    :1;

    /* CV model and parasitics */
    unsigned  BSIM4cgslGiven   :1;
    unsigned  BSIM4cgdlGiven   :1;
    unsigned  BSIM4ckappasGiven   :1;
    unsigned  BSIM4ckappadGiven   :1;
    unsigned  BSIM4cfGiven   :1;
    unsigned  BSIM4vfbcvGiven   :1;
    unsigned  BSIM4clcGiven   :1;
    unsigned  BSIM4cleGiven   :1;
    unsigned  BSIM4dwcGiven   :1;
    unsigned  BSIM4dlcGiven   :1;
    unsigned  BSIM4xwGiven    :1;
    unsigned  BSIM4xlGiven    :1;
    unsigned  BSIM4dlcigGiven   :1;
    unsigned  BSIM4dlcigdGiven   :1;
    unsigned  BSIM4dwjGiven   :1;
    unsigned  BSIM4noffGiven  :1;
    unsigned  BSIM4voffcvGiven :1;
    unsigned  BSIM4acdeGiven  :1;
    unsigned  BSIM4moinGiven  :1;
    unsigned  BSIM4tcjGiven   :1;
    unsigned  BSIM4tcjswGiven :1;
    unsigned  BSIM4tcjswgGiven :1;
    unsigned  BSIM4tpbGiven    :1;
    unsigned  BSIM4tpbswGiven  :1;
    unsigned  BSIM4tpbswgGiven :1;
    unsigned  BSIM4dmcgGiven :1;
    unsigned  BSIM4dmciGiven :1;
    unsigned  BSIM4dmdgGiven :1;
    unsigned  BSIM4dmcgtGiven :1;
    unsigned  BSIM4xgwGiven :1;
    unsigned  BSIM4xglGiven :1;
    unsigned  BSIM4rshgGiven :1;
    unsigned  BSIM4ngconGiven :1;

    /* Length dependence */
    unsigned  BSIM4lcdscGiven   :1;
    unsigned  BSIM4lcdscbGiven   :1;
    unsigned  BSIM4lcdscdGiven   :1;
    unsigned  BSIM4lcitGiven   :1;
    unsigned  BSIM4lnfactorGiven   :1;
    unsigned  BSIM4lxjGiven   :1;
    unsigned  BSIM4lvsatGiven   :1;
    unsigned  BSIM4latGiven   :1;
    unsigned  BSIM4la0Given   :1;
    unsigned  BSIM4lagsGiven   :1;
    unsigned  BSIM4la1Given   :1;
    unsigned  BSIM4la2Given   :1;
    unsigned  BSIM4lketaGiven   :1;    
    unsigned  BSIM4lnsubGiven   :1;
    unsigned  BSIM4lndepGiven   :1;
    unsigned  BSIM4lnsdGiven    :1;
    unsigned  BSIM4lphinGiven   :1;
    unsigned  BSIM4lngateGiven   :1;
    unsigned  BSIM4lgamma1Given   :1;
    unsigned  BSIM4lgamma2Given   :1;
    unsigned  BSIM4lvbxGiven   :1;
    unsigned  BSIM4lvbmGiven   :1;
    unsigned  BSIM4lxtGiven   :1;
    unsigned  BSIM4lk1Given   :1;
    unsigned  BSIM4lkt1Given   :1;
    unsigned  BSIM4lkt1lGiven   :1;
    unsigned  BSIM4lkt2Given   :1;
    unsigned  BSIM4lk2Given   :1;
    unsigned  BSIM4lk3Given   :1;
    unsigned  BSIM4lk3bGiven   :1;
    unsigned  BSIM4lw0Given   :1;
    unsigned  BSIM4ldvtp0Given :1;
    unsigned  BSIM4ldvtp1Given :1;
    unsigned  BSIM4llpe0Given   :1;
    unsigned  BSIM4llpebGiven   :1;
    unsigned  BSIM4ldvt0Given   :1;   
    unsigned  BSIM4ldvt1Given   :1;     
    unsigned  BSIM4ldvt2Given   :1;     
    unsigned  BSIM4ldvt0wGiven   :1;   
    unsigned  BSIM4ldvt1wGiven   :1;     
    unsigned  BSIM4ldvt2wGiven   :1;     
    unsigned  BSIM4ldroutGiven   :1;     
    unsigned  BSIM4ldsubGiven   :1;     
    unsigned  BSIM4lvth0Given   :1;
    unsigned  BSIM4luaGiven   :1;
    unsigned  BSIM4lua1Given   :1;
    unsigned  BSIM4lubGiven   :1;
    unsigned  BSIM4lub1Given   :1;
    unsigned  BSIM4lucGiven   :1;
    unsigned  BSIM4luc1Given   :1;
    unsigned  BSIM4ludGiven     :1;
    unsigned  BSIM4lud1Given     :1;
    unsigned  BSIM4lupGiven     :1;
    unsigned  BSIM4llpGiven     :1;
    unsigned  BSIM4lu0Given   :1;
    unsigned  BSIM4leuGiven   :1;
    unsigned  BSIM4lucsGiven   :1;
    unsigned  BSIM4luteGiven   :1;
    unsigned  BSIM4lucsteGiven  :1;
    unsigned  BSIM4lvoffGiven   :1;
    unsigned  BSIM4ltvoffGiven   :1;
    unsigned  BSIM4lminvGiven   :1;
    unsigned  BSIM4lminvcvGiven   :1;
    unsigned  BSIM4lrdswGiven   :1;      
    unsigned  BSIM4lrswGiven   :1;
    unsigned  BSIM4lrdwGiven   :1;
    unsigned  BSIM4lprwgGiven   :1;      
    unsigned  BSIM4lprwbGiven   :1;      
    unsigned  BSIM4lprtGiven   :1;      
    unsigned  BSIM4leta0Given   :1;    
    unsigned  BSIM4letabGiven   :1;    
    unsigned  BSIM4lpclmGiven   :1;   
    unsigned  BSIM4lpdibl1Given   :1;   
    unsigned  BSIM4lpdibl2Given   :1;  
    unsigned  BSIM4lpdiblbGiven   :1;  
    unsigned  BSIM4lfproutGiven   :1;
    unsigned  BSIM4lpditsGiven    :1;
    unsigned  BSIM4lpditsdGiven    :1;
    unsigned  BSIM4lpscbe1Given   :1;    
    unsigned  BSIM4lpscbe2Given   :1;    
    unsigned  BSIM4lpvagGiven   :1;    
    unsigned  BSIM4ldeltaGiven  :1;     
    unsigned  BSIM4lwrGiven   :1;
    unsigned  BSIM4ldwgGiven   :1;
    unsigned  BSIM4ldwbGiven   :1;
    unsigned  BSIM4lb0Given   :1;
    unsigned  BSIM4lb1Given   :1;
    unsigned  BSIM4lalpha0Given   :1;
    unsigned  BSIM4lalpha1Given   :1;
    unsigned  BSIM4lbeta0Given   :1;
    unsigned  BSIM4lvfbGiven   :1;
    unsigned  BSIM4lagidlGiven   :1;
    unsigned  BSIM4lbgidlGiven   :1;
    unsigned  BSIM4lcgidlGiven   :1;
    unsigned  BSIM4legidlGiven   :1;
    unsigned  BSIM4lagislGiven   :1;
    unsigned  BSIM4lbgislGiven   :1;
    unsigned  BSIM4lcgislGiven   :1;
    unsigned  BSIM4legislGiven   :1;
    unsigned  BSIM4laigcGiven   :1;
    unsigned  BSIM4lbigcGiven   :1;
    unsigned  BSIM4lcigcGiven   :1;
    unsigned  BSIM4laigsdGiven   :1;
    unsigned  BSIM4lbigsdGiven   :1;
    unsigned  BSIM4lcigsdGiven   :1;
    unsigned  BSIM4laigsGiven   :1;
    unsigned  BSIM4lbigsGiven   :1;
    unsigned  BSIM4lcigsGiven   :1;
    unsigned  BSIM4laigdGiven   :1;
    unsigned  BSIM4lbigdGiven   :1;
    unsigned  BSIM4lcigdGiven   :1;
    unsigned  BSIM4laigbaccGiven   :1;
    unsigned  BSIM4lbigbaccGiven   :1;
    unsigned  BSIM4lcigbaccGiven   :1;
    unsigned  BSIM4laigbinvGiven   :1;
    unsigned  BSIM4lbigbinvGiven   :1;
    unsigned  BSIM4lcigbinvGiven   :1;
    unsigned  BSIM4lnigcGiven   :1;
    unsigned  BSIM4lnigbinvGiven   :1;
    unsigned  BSIM4lnigbaccGiven   :1;
    unsigned  BSIM4lntoxGiven   :1;
    unsigned  BSIM4leigbinvGiven   :1;
    unsigned  BSIM4lpigcdGiven   :1;
    unsigned  BSIM4lpoxedgeGiven   :1;
    unsigned  BSIM4lxrcrg1Given   :1;
    unsigned  BSIM4lxrcrg2Given   :1;
    unsigned  BSIM4llambdaGiven    :1;
    unsigned  BSIM4lvtlGiven    :1;
    unsigned  BSIM4lxnGiven    :1;
    unsigned  BSIM4lvfbsdoffGiven    :1;
    unsigned  BSIM4ltvfbsdoffGiven    :1;

    /* CV model */
    unsigned  BSIM4lcgslGiven   :1;
    unsigned  BSIM4lcgdlGiven   :1;
    unsigned  BSIM4lckappasGiven   :1;
    unsigned  BSIM4lckappadGiven   :1;
    unsigned  BSIM4lcfGiven   :1;
    unsigned  BSIM4lclcGiven   :1;
    unsigned  BSIM4lcleGiven   :1;
    unsigned  BSIM4lvfbcvGiven   :1;
    unsigned  BSIM4lnoffGiven   :1;
    unsigned  BSIM4lvoffcvGiven :1;
    unsigned  BSIM4lacdeGiven   :1;
    unsigned  BSIM4lmoinGiven   :1;

    /* Width dependence */
    unsigned  BSIM4wcdscGiven   :1;
    unsigned  BSIM4wcdscbGiven   :1;
    unsigned  BSIM4wcdscdGiven   :1;
    unsigned  BSIM4wcitGiven   :1;
    unsigned  BSIM4wnfactorGiven   :1;
    unsigned  BSIM4wxjGiven   :1;
    unsigned  BSIM4wvsatGiven   :1;
    unsigned  BSIM4watGiven   :1;
    unsigned  BSIM4wa0Given   :1;
    unsigned  BSIM4wagsGiven   :1;
    unsigned  BSIM4wa1Given   :1;
    unsigned  BSIM4wa2Given   :1;
    unsigned  BSIM4wketaGiven   :1;    
    unsigned  BSIM4wnsubGiven   :1;
    unsigned  BSIM4wndepGiven   :1;
    unsigned  BSIM4wnsdGiven    :1;
    unsigned  BSIM4wphinGiven   :1;
    unsigned  BSIM4wngateGiven   :1;
    unsigned  BSIM4wgamma1Given   :1;
    unsigned  BSIM4wgamma2Given   :1;
    unsigned  BSIM4wvbxGiven   :1;
    unsigned  BSIM4wvbmGiven   :1;
    unsigned  BSIM4wxtGiven   :1;
    unsigned  BSIM4wk1Given   :1;
    unsigned  BSIM4wkt1Given   :1;
    unsigned  BSIM4wkt1lGiven   :1;
    unsigned  BSIM4wkt2Given   :1;
    unsigned  BSIM4wk2Given   :1;
    unsigned  BSIM4wk3Given   :1;
    unsigned  BSIM4wk3bGiven   :1;
    unsigned  BSIM4ww0Given   :1;
    unsigned  BSIM4wdvtp0Given :1;
    unsigned  BSIM4wdvtp1Given :1;
    unsigned  BSIM4wlpe0Given   :1;
    unsigned  BSIM4wlpebGiven   :1;
    unsigned  BSIM4wdvt0Given   :1;   
    unsigned  BSIM4wdvt1Given   :1;     
    unsigned  BSIM4wdvt2Given   :1;     
    unsigned  BSIM4wdvt0wGiven   :1;   
    unsigned  BSIM4wdvt1wGiven   :1;     
    unsigned  BSIM4wdvt2wGiven   :1;     
    unsigned  BSIM4wdroutGiven   :1;     
    unsigned  BSIM4wdsubGiven   :1;     
    unsigned  BSIM4wvth0Given   :1;
    unsigned  BSIM4wuaGiven   :1;
    unsigned  BSIM4wua1Given   :1;
    unsigned  BSIM4wubGiven   :1;
    unsigned  BSIM4wub1Given   :1;
    unsigned  BSIM4wucGiven   :1;
    unsigned  BSIM4wuc1Given   :1;
    unsigned  BSIM4wudGiven     :1;
    unsigned  BSIM4wud1Given     :1;
    unsigned  BSIM4wupGiven     :1;
    unsigned  BSIM4wlpGiven     :1;
    unsigned  BSIM4wu0Given   :1;
    unsigned  BSIM4weuGiven   :1;
    unsigned  BSIM4wucsGiven  :1;
    unsigned  BSIM4wuteGiven   :1;
    unsigned  BSIM4wucsteGiven  :1;
    unsigned  BSIM4wvoffGiven   :1;
    unsigned  BSIM4wtvoffGiven   :1;
    unsigned  BSIM4wminvGiven   :1;
    unsigned  BSIM4wminvcvGiven   :1;
    unsigned  BSIM4wrdswGiven   :1;      
    unsigned  BSIM4wrswGiven   :1;
    unsigned  BSIM4wrdwGiven   :1;
    unsigned  BSIM4wprwgGiven   :1;      
    unsigned  BSIM4wprwbGiven   :1;      
    unsigned  BSIM4wprtGiven   :1;      
    unsigned  BSIM4weta0Given   :1;    
    unsigned  BSIM4wetabGiven   :1;    
    unsigned  BSIM4wpclmGiven   :1;   
    unsigned  BSIM4wpdibl1Given   :1;   
    unsigned  BSIM4wpdibl2Given   :1;  
    unsigned  BSIM4wpdiblbGiven   :1;  
    unsigned  BSIM4wfproutGiven   :1;
    unsigned  BSIM4wpditsGiven    :1;
    unsigned  BSIM4wpditsdGiven    :1;
    unsigned  BSIM4wpscbe1Given   :1;    
    unsigned  BSIM4wpscbe2Given   :1;    
    unsigned  BSIM4wpvagGiven   :1;    
    unsigned  BSIM4wdeltaGiven  :1;     
    unsigned  BSIM4wwrGiven   :1;
    unsigned  BSIM4wdwgGiven   :1;
    unsigned  BSIM4wdwbGiven   :1;
    unsigned  BSIM4wb0Given   :1;
    unsigned  BSIM4wb1Given   :1;
    unsigned  BSIM4walpha0Given   :1;
    unsigned  BSIM4walpha1Given   :1;
    unsigned  BSIM4wbeta0Given   :1;
    unsigned  BSIM4wvfbGiven   :1;
    unsigned  BSIM4wagidlGiven   :1;
    unsigned  BSIM4wbgidlGiven   :1;
    unsigned  BSIM4wcgidlGiven   :1;
    unsigned  BSIM4wegidlGiven   :1;
    unsigned  BSIM4wagislGiven   :1;
    unsigned  BSIM4wbgislGiven   :1;
    unsigned  BSIM4wcgislGiven   :1;
    unsigned  BSIM4wegislGiven   :1;
    unsigned  BSIM4waigcGiven   :1;
    unsigned  BSIM4wbigcGiven   :1;
    unsigned  BSIM4wcigcGiven   :1;
    unsigned  BSIM4waigsdGiven   :1;
    unsigned  BSIM4wbigsdGiven   :1;
    unsigned  BSIM4wcigsdGiven   :1;
    unsigned  BSIM4waigsGiven   :1;
    unsigned  BSIM4wbigsGiven   :1;
    unsigned  BSIM4wcigsGiven   :1;
    unsigned  BSIM4waigdGiven   :1;
    unsigned  BSIM4wbigdGiven   :1;
    unsigned  BSIM4wcigdGiven   :1;
    unsigned  BSIM4waigbaccGiven   :1;
    unsigned  BSIM4wbigbaccGiven   :1;
    unsigned  BSIM4wcigbaccGiven   :1;
    unsigned  BSIM4waigbinvGiven   :1;
    unsigned  BSIM4wbigbinvGiven   :1;
    unsigned  BSIM4wcigbinvGiven   :1;
    unsigned  BSIM4wnigcGiven   :1;
    unsigned  BSIM4wnigbinvGiven   :1;
    unsigned  BSIM4wnigbaccGiven   :1;
    unsigned  BSIM4wntoxGiven   :1;
    unsigned  BSIM4weigbinvGiven   :1;
    unsigned  BSIM4wpigcdGiven   :1;
    unsigned  BSIM4wpoxedgeGiven   :1;
    unsigned  BSIM4wxrcrg1Given   :1;
    unsigned  BSIM4wxrcrg2Given   :1;
    unsigned  BSIM4wlambdaGiven    :1;
    unsigned  BSIM4wvtlGiven    :1;
    unsigned  BSIM4wxnGiven    :1;
    unsigned  BSIM4wvfbsdoffGiven    :1;
    unsigned  BSIM4wtvfbsdoffGiven    :1;

    /* CV model */
    unsigned  BSIM4wcgslGiven   :1;
    unsigned  BSIM4wcgdlGiven   :1;
    unsigned  BSIM4wckappasGiven   :1;
    unsigned  BSIM4wckappadGiven   :1;
    unsigned  BSIM4wcfGiven   :1;
    unsigned  BSIM4wclcGiven   :1;
    unsigned  BSIM4wcleGiven   :1;
    unsigned  BSIM4wvfbcvGiven   :1;
    unsigned  BSIM4wnoffGiven   :1;
    unsigned  BSIM4wvoffcvGiven :1;
    unsigned  BSIM4wacdeGiven   :1;
    unsigned  BSIM4wmoinGiven   :1;

    /* Cross-term dependence */
    unsigned  BSIM4pcdscGiven   :1;
    unsigned  BSIM4pcdscbGiven   :1;
    unsigned  BSIM4pcdscdGiven   :1;
    unsigned  BSIM4pcitGiven   :1;
    unsigned  BSIM4pnfactorGiven   :1;
    unsigned  BSIM4pxjGiven   :1;
    unsigned  BSIM4pvsatGiven   :1;
    unsigned  BSIM4patGiven   :1;
    unsigned  BSIM4pa0Given   :1;
    unsigned  BSIM4pagsGiven   :1;
    unsigned  BSIM4pa1Given   :1;
    unsigned  BSIM4pa2Given   :1;
    unsigned  BSIM4pketaGiven   :1;    
    unsigned  BSIM4pnsubGiven   :1;
    unsigned  BSIM4pndepGiven   :1;
    unsigned  BSIM4pnsdGiven    :1;
    unsigned  BSIM4pphinGiven   :1;
    unsigned  BSIM4pngateGiven   :1;
    unsigned  BSIM4pgamma1Given   :1;
    unsigned  BSIM4pgamma2Given   :1;
    unsigned  BSIM4pvbxGiven   :1;
    unsigned  BSIM4pvbmGiven   :1;
    unsigned  BSIM4pxtGiven   :1;
    unsigned  BSIM4pk1Given   :1;
    unsigned  BSIM4pkt1Given   :1;
    unsigned  BSIM4pkt1lGiven   :1;
    unsigned  BSIM4pkt2Given   :1;
    unsigned  BSIM4pk2Given   :1;
    unsigned  BSIM4pk3Given   :1;
    unsigned  BSIM4pk3bGiven   :1;
    unsigned  BSIM4pw0Given   :1;
    unsigned  BSIM4pdvtp0Given :1;
    unsigned  BSIM4pdvtp1Given :1;
    unsigned  BSIM4plpe0Given   :1;
    unsigned  BSIM4plpebGiven   :1;
    unsigned  BSIM4pdvt0Given   :1;   
    unsigned  BSIM4pdvt1Given   :1;     
    unsigned  BSIM4pdvt2Given   :1;     
    unsigned  BSIM4pdvt0wGiven   :1;   
    unsigned  BSIM4pdvt1wGiven   :1;     
    unsigned  BSIM4pdvt2wGiven   :1;     
    unsigned  BSIM4pdroutGiven   :1;     
    unsigned  BSIM4pdsubGiven   :1;     
    unsigned  BSIM4pvth0Given   :1;
    unsigned  BSIM4puaGiven   :1;
    unsigned  BSIM4pua1Given   :1;
    unsigned  BSIM4pubGiven   :1;
    unsigned  BSIM4pub1Given   :1;
    unsigned  BSIM4pucGiven   :1;
    unsigned  BSIM4puc1Given   :1;
    unsigned  BSIM4pudGiven     :1;
    unsigned  BSIM4pud1Given     :1;
    unsigned  BSIM4pupGiven     :1;
    unsigned  BSIM4plpGiven     :1;
    unsigned  BSIM4pu0Given   :1;
    unsigned  BSIM4peuGiven   :1;
    unsigned  BSIM4pucsGiven   :1;
    unsigned  BSIM4puteGiven   :1;
    unsigned  BSIM4pucsteGiven  :1;
    unsigned  BSIM4pvoffGiven   :1;
    unsigned  BSIM4ptvoffGiven   :1;
    unsigned  BSIM4pminvGiven   :1;
    unsigned  BSIM4pminvcvGiven   :1;
    unsigned  BSIM4prdswGiven   :1;      
    unsigned  BSIM4prswGiven   :1;
    unsigned  BSIM4prdwGiven   :1;
    unsigned  BSIM4pprwgGiven   :1;      
    unsigned  BSIM4pprwbGiven   :1;      
    unsigned  BSIM4pprtGiven   :1;      
    unsigned  BSIM4peta0Given   :1;    
    unsigned  BSIM4petabGiven   :1;    
    unsigned  BSIM4ppclmGiven   :1;   
    unsigned  BSIM4ppdibl1Given   :1;   
    unsigned  BSIM4ppdibl2Given   :1;  
    unsigned  BSIM4ppdiblbGiven   :1;  
    unsigned  BSIM4pfproutGiven   :1;
    unsigned  BSIM4ppditsGiven    :1;
    unsigned  BSIM4ppditsdGiven    :1;
    unsigned  BSIM4ppscbe1Given   :1;    
    unsigned  BSIM4ppscbe2Given   :1;    
    unsigned  BSIM4ppvagGiven   :1;    
    unsigned  BSIM4pdeltaGiven  :1;     
    unsigned  BSIM4pwrGiven   :1;
    unsigned  BSIM4pdwgGiven   :1;
    unsigned  BSIM4pdwbGiven   :1;
    unsigned  BSIM4pb0Given   :1;
    unsigned  BSIM4pb1Given   :1;
    unsigned  BSIM4palpha0Given   :1;
    unsigned  BSIM4palpha1Given   :1;
    unsigned  BSIM4pbeta0Given   :1;
    unsigned  BSIM4pvfbGiven   :1;
    unsigned  BSIM4pagidlGiven   :1;
    unsigned  BSIM4pbgidlGiven   :1;
    unsigned  BSIM4pcgidlGiven   :1;
    unsigned  BSIM4pegidlGiven   :1;
    unsigned  BSIM4pagislGiven   :1;
    unsigned  BSIM4pbgislGiven   :1;
    unsigned  BSIM4pcgislGiven   :1;
    unsigned  BSIM4pegislGiven   :1;
    unsigned  BSIM4paigcGiven   :1;
    unsigned  BSIM4pbigcGiven   :1;
    unsigned  BSIM4pcigcGiven   :1;
    unsigned  BSIM4paigsdGiven   :1;
    unsigned  BSIM4pbigsdGiven   :1;
    unsigned  BSIM4pcigsdGiven   :1;
    unsigned  BSIM4paigsGiven   :1;
    unsigned  BSIM4pbigsGiven   :1;
    unsigned  BSIM4pcigsGiven   :1;
    unsigned  BSIM4paigdGiven   :1;
    unsigned  BSIM4pbigdGiven   :1;
    unsigned  BSIM4pcigdGiven   :1;
    unsigned  BSIM4paigbaccGiven   :1;
    unsigned  BSIM4pbigbaccGiven   :1;
    unsigned  BSIM4pcigbaccGiven   :1;
    unsigned  BSIM4paigbinvGiven   :1;
    unsigned  BSIM4pbigbinvGiven   :1;
    unsigned  BSIM4pcigbinvGiven   :1;
    unsigned  BSIM4pnigcGiven   :1;
    unsigned  BSIM4pnigbinvGiven   :1;
    unsigned  BSIM4pnigbaccGiven   :1;
    unsigned  BSIM4pntoxGiven   :1;
    unsigned  BSIM4peigbinvGiven   :1;
    unsigned  BSIM4ppigcdGiven   :1;
    unsigned  BSIM4ppoxedgeGiven   :1;
    unsigned  BSIM4pxrcrg1Given   :1;
    unsigned  BSIM4pxrcrg2Given   :1;
    unsigned  BSIM4plambdaGiven    :1;
    unsigned  BSIM4pvtlGiven    :1;
    unsigned  BSIM4pxnGiven    :1;
    unsigned  BSIM4pvfbsdoffGiven    :1;
    unsigned  BSIM4ptvfbsdoffGiven    :1;

    /* CV model */
    unsigned  BSIM4pcgslGiven   :1;
    unsigned  BSIM4pcgdlGiven   :1;
    unsigned  BSIM4pckappasGiven   :1;
    unsigned  BSIM4pckappadGiven   :1;
    unsigned  BSIM4pcfGiven   :1;
    unsigned  BSIM4pclcGiven   :1;
    unsigned  BSIM4pcleGiven   :1;
    unsigned  BSIM4pvfbcvGiven   :1;
    unsigned  BSIM4pnoffGiven   :1;
    unsigned  BSIM4pvoffcvGiven :1;
    unsigned  BSIM4pacdeGiven   :1;
    unsigned  BSIM4pmoinGiven   :1;

    unsigned  BSIM4useFringeGiven   :1;

    unsigned  BSIM4tnomGiven   :1;
    unsigned  BSIM4cgsoGiven   :1;
    unsigned  BSIM4cgdoGiven   :1;
    unsigned  BSIM4cgboGiven   :1;
    unsigned  BSIM4xpartGiven   :1;
    unsigned  BSIM4sheetResistanceGiven   :1;

    unsigned  BSIM4SjctSatCurDensityGiven   :1;
    unsigned  BSIM4SjctSidewallSatCurDensityGiven   :1;
    unsigned  BSIM4SjctGateSidewallSatCurDensityGiven   :1;
    unsigned  BSIM4SbulkJctPotentialGiven   :1;
    unsigned  BSIM4SbulkJctBotGradingCoeffGiven   :1;
    unsigned  BSIM4SsidewallJctPotentialGiven   :1;
    unsigned  BSIM4SGatesidewallJctPotentialGiven   :1;
    unsigned  BSIM4SbulkJctSideGradingCoeffGiven   :1;
    unsigned  BSIM4SunitAreaJctCapGiven   :1;
    unsigned  BSIM4SunitLengthSidewallJctCapGiven   :1;
    unsigned  BSIM4SbulkJctGateSideGradingCoeffGiven   :1;
    unsigned  BSIM4SunitLengthGateSidewallJctCapGiven   :1;
    unsigned  BSIM4SjctEmissionCoeffGiven :1;
    unsigned  BSIM4SjctTempExponentGiven    :1;

    unsigned  BSIM4DjctSatCurDensityGiven   :1;
    unsigned  BSIM4DjctSidewallSatCurDensityGiven   :1;
    unsigned  BSIM4DjctGateSidewallSatCurDensityGiven   :1;
    unsigned  BSIM4DbulkJctPotentialGiven   :1;
    unsigned  BSIM4DbulkJctBotGradingCoeffGiven   :1;
    unsigned  BSIM4DsidewallJctPotentialGiven   :1;
    unsigned  BSIM4DGatesidewallJctPotentialGiven   :1;
    unsigned  BSIM4DbulkJctSideGradingCoeffGiven   :1;
    unsigned  BSIM4DunitAreaJctCapGiven   :1;
    unsigned  BSIM4DunitLengthSidewallJctCapGiven   :1;
    unsigned  BSIM4DbulkJctGateSideGradingCoeffGiven   :1;
    unsigned  BSIM4DunitLengthGateSidewallJctCapGiven   :1;
    unsigned  BSIM4DjctEmissionCoeffGiven :1;
    unsigned  BSIM4DjctTempExponentGiven :1;

    unsigned  BSIM4oxideTrapDensityAGiven  :1;         
    unsigned  BSIM4oxideTrapDensityBGiven  :1;        
    unsigned  BSIM4oxideTrapDensityCGiven  :1;     
    unsigned  BSIM4emGiven  :1;     
    unsigned  BSIM4efGiven  :1;     
    unsigned  BSIM4afGiven  :1;     
    unsigned  BSIM4kfGiven  :1;     

    unsigned  BSIM4LintGiven   :1;
    unsigned  BSIM4LlGiven   :1;
    unsigned  BSIM4LlcGiven   :1;
    unsigned  BSIM4LlnGiven   :1;
    unsigned  BSIM4LwGiven   :1;
    unsigned  BSIM4LwcGiven   :1;
    unsigned  BSIM4LwnGiven   :1;
    unsigned  BSIM4LwlGiven   :1;
    unsigned  BSIM4LwlcGiven   :1;
    unsigned  BSIM4LminGiven   :1;
    unsigned  BSIM4LmaxGiven   :1;

    unsigned  BSIM4WintGiven   :1;
    unsigned  BSIM4WlGiven   :1;
    unsigned  BSIM4WlcGiven   :1;
    unsigned  BSIM4WlnGiven   :1;
    unsigned  BSIM4WwGiven   :1;
    unsigned  BSIM4WwcGiven   :1;
    unsigned  BSIM4WwnGiven   :1;
    unsigned  BSIM4WwlGiven   :1;
    unsigned  BSIM4WwlcGiven   :1;
    unsigned  BSIM4WminGiven   :1;
    unsigned  BSIM4WmaxGiven   :1;

    /* added for stress effect */
    unsigned  BSIM4sarefGiven   :1;
    unsigned  BSIM4sbrefGiven   :1;
    unsigned  BSIM4wlodGiven  :1;
    unsigned  BSIM4ku0Given   :1;
    unsigned  BSIM4kvsatGiven  :1;
    unsigned  BSIM4kvth0Given  :1;
    unsigned  BSIM4tku0Given   :1;
    unsigned  BSIM4llodku0Given   :1;
    unsigned  BSIM4wlodku0Given   :1;
    unsigned  BSIM4llodvthGiven   :1;
    unsigned  BSIM4wlodvthGiven   :1;
    unsigned  BSIM4lku0Given   :1;
    unsigned  BSIM4wku0Given   :1;
    unsigned  BSIM4pku0Given   :1;
    unsigned  BSIM4lkvth0Given   :1;
    unsigned  BSIM4wkvth0Given   :1;
    unsigned  BSIM4pkvth0Given   :1;
    unsigned  BSIM4stk2Given   :1;
    unsigned  BSIM4lodk2Given  :1;
    unsigned  BSIM4steta0Given :1;
    unsigned  BSIM4lodeta0Given :1;

    unsigned  BSIM4webGiven   :1;
    unsigned  BSIM4wecGiven   :1;
    unsigned  BSIM4kvth0weGiven   :1;
    unsigned  BSIM4k2weGiven   :1;
    unsigned  BSIM4ku0weGiven   :1;
    unsigned  BSIM4screfGiven   :1;
    unsigned  BSIM4wpemodGiven   :1;
    unsigned  BSIM4lkvth0weGiven   :1;
    unsigned  BSIM4lk2weGiven   :1;
    unsigned  BSIM4lku0weGiven   :1;
    unsigned  BSIM4wkvth0weGiven   :1;
    unsigned  BSIM4wk2weGiven   :1;
    unsigned  BSIM4wku0weGiven   :1;
    unsigned  BSIM4pkvth0weGiven   :1;
    unsigned  BSIM4pk2weGiven   :1;
    unsigned  BSIM4pku0weGiven   :1;
};

struct sBSIM4model : sGENmodel, sBSIM4modelPOD
{
    sBSIM4model() : sGENmodel(), sBSIM4modelPOD() { }
    ~sBSIM4model()
        {
            while (pSizeDependParamKnot) {
                bsim4SizeDependParam *px = pSizeDependParamKnot;
                pSizeDependParamKnot = pSizeDependParamKnot->pNext;
                delete px;
            }
            delete [] BSIM4version;
        }

    sBSIM4model *next()     { return ((sBSIM4model*)GENnextModel); }
    sBSIM4instance *inst()  { return ((sBSIM4instance*)GENinstances); }
};
} // namespace BSIM465
using namespace BSIM465;


#ifndef NMOS
#define NMOS 1
#define PMOS -1
#endif

// Instance parameters
enum {
    BSIM4_W = 1,
    BSIM4_L,
    BSIM4_AS,
    BSIM4_AD,
    BSIM4_PS,
    BSIM4_PD,
    BSIM4_NRS,
    BSIM4_NRD,
    BSIM4_OFF,
    BSIM4_IC,
    BSIM4_IC_VDS,
    BSIM4_IC_VGS,
    BSIM4_IC_VBS,
    BSIM4_TRNQSMOD,
    BSIM4_RBODYMOD,
    BSIM4_RGATEMOD,
    BSIM4_GEOMOD,
    BSIM4_RGEOMOD,
    BSIM4_NF,
    BSIM4_MIN,
    BSIM4_ACNQSMOD,
    BSIM4_RBDB,
    BSIM4_RBSB,
    BSIM4_RBPB,
    BSIM4_RBPS,
    BSIM4_RBPD,
    BSIM4_SA,
    BSIM4_SB,
    BSIM4_SD,
    BSIM4_DELVTO,
    BSIM4_XGW,
    BSIM4_NGCON,
    BSIM4_SCA,
    BSIM4_SCB,
    BSIM4_SCC,
    BSIM4_SC,

    BSIM4_DNODE,
    BSIM4_GNODEEXT,
    BSIM4_SNODE,
    BSIM4_BNODE,
    BSIM4_DNODEPRIME,
    BSIM4_GNODEPRIME,
    BSIM4_GNODEMIDE,
    BSIM4_GNODEMID,
    BSIM4_SNODEPRIME,
    BSIM4_BNODEPRIME,
    BSIM4_DBNODE,
    BSIM4_SBNODE,
    BSIM4_VBD,
    BSIM4_VBS,
    BSIM4_VGS,
    BSIM4_VDS,
    BSIM4_CD,
    BSIM4_CBS,
    BSIM4_CBD,
    BSIM4_GM,
    BSIM4_GDS,
    BSIM4_GMBS,
    BSIM4_GBD,
    BSIM4_GBS,
    BSIM4_QB,
    BSIM4_CQB,
    BSIM4_QG,
    BSIM4_CQG,
    BSIM4_QD,
    BSIM4_CQD,
    BSIM4_CGGB,
    BSIM4_CGDB,
    BSIM4_CGSB,
    BSIM4_CBGB,
    BSIM4_CAPBD,
    BSIM4_CQBD,
    BSIM4_CAPBS,
    BSIM4_CQBS,
    BSIM4_CDGB,
    BSIM4_CDDB,
    BSIM4_CDSB,
    BSIM4_VON,
    BSIM4_VDSAT,
    BSIM4_QBS,
    BSIM4_QBD,
    BSIM4_SOURCECONDUCT,
    BSIM4_DRAINCONDUCT,
    BSIM4_CBDB,
    BSIM4_CBSB,
    BSIM4_CSUB,
    BSIM4_QINV,
    BSIM4_IGIDL,
    BSIM4_CSGB,
    BSIM4_CSDB,
    BSIM4_CSSB,
    BSIM4_CGBB,
    BSIM4_CDBB,
    BSIM4_CSBB,
    BSIM4_CBBB,
    BSIM4_QS,
    BSIM4_IGISL,
    BSIM4_IGS,
    BSIM4_IGD,
    BSIM4_IGB,
    BSIM4_IGCS,
    BSIM4_IGCD,
    BSIM4_QDEF,
    BSIM4_DELVT0,
    BSIM4_GCRG,
    BSIM4_GTAU,

    // SRW - added
    BSIM4_ID,
    BSIM4_IS,
    BSIM4_IG,
    BSIM4_IB,
    BSIM4_M,
    BSIM4_WF
};

// Model parameters
enum {
    BSIM4_MOD_TEMPEOT = 1000,
    BSIM4_MOD_LEFFEOT,
    BSIM4_MOD_WEFFEOT,
    BSIM4_MOD_UCSTE,
    BSIM4_MOD_LUCSTE,
    BSIM4_MOD_WUCSTE,
    BSIM4_MOD_PUCSTE,
    BSIM4_MOD_UCS,
    BSIM4_MOD_LUCS,
    BSIM4_MOD_WUCS,
    BSIM4_MOD_PUCS,
    BSIM4_MOD_CVCHARGEMOD,
    BSIM4_MOD_ADOS,
    BSIM4_MOD_BDOS,
    BSIM4_MOD_TEMPMOD,
    BSIM4_MOD_MTRLMOD,
    BSIM4_MOD_IGCMOD,
    BSIM4_MOD_IGBMOD,
    BSIM4_MOD_ACNQSMOD,
    BSIM4_MOD_FNOIMOD,
    BSIM4_MOD_RDSMOD,
    BSIM4_MOD_DIOMOD,
    BSIM4_MOD_PERMOD,
    BSIM4_MOD_GEOMOD,
    // SRW, added RGEOMODE as model param
    BSIM4_MOD_RGEOMOD,
    BSIM4_MOD_RGATEMOD,
    BSIM4_MOD_RBODYMOD,
    BSIM4_MOD_CAPMOD,
    BSIM4_MOD_TRNQSMOD,
    BSIM4_MOD_MOBMOD,
    BSIM4_MOD_TNOIMOD,
    BSIM4_MOD_EOT,
    BSIM4_MOD_VDDEOT,
    BSIM4_MOD_TOXE,
    BSIM4_MOD_CDSC,
    BSIM4_MOD_CDSCB,
    BSIM4_MOD_CIT,
    BSIM4_MOD_NFACTOR,
    BSIM4_MOD_XJ,
    BSIM4_MOD_VSAT,
    BSIM4_MOD_AT,
    BSIM4_MOD_A0,
    BSIM4_MOD_A1,
    BSIM4_MOD_A2,
    BSIM4_MOD_KETA,
    BSIM4_MOD_NSUB,
    BSIM4_MOD_PHIG,
    BSIM4_MOD_EPSRGATE,
    BSIM4_MOD_EASUB,
    BSIM4_MOD_EPSRSUB,
    BSIM4_MOD_NI0SUB,
    BSIM4_MOD_BG0SUB,
    BSIM4_MOD_TBGASUB,
    BSIM4_MOD_TBGBSUB,
    BSIM4_MOD_NDEP,
    BSIM4_MOD_NGATE,
    BSIM4_MOD_GAMMA1,
    BSIM4_MOD_GAMMA2,
    BSIM4_MOD_VBX,
    BSIM4_MOD_BINUNIT,
    BSIM4_MOD_VBM,
    BSIM4_MOD_XT,
    BSIM4_MOD_K1,
    BSIM4_MOD_KT1,
    BSIM4_MOD_KT1L,
    BSIM4_MOD_K2,
    BSIM4_MOD_KT2,
    BSIM4_MOD_K3,
    BSIM4_MOD_K3B,
    BSIM4_MOD_W0,
    BSIM4_MOD_LPE0,
    BSIM4_MOD_DVT0,
    BSIM4_MOD_DVT1,
    BSIM4_MOD_DVT2,
    BSIM4_MOD_DVT0W,
    BSIM4_MOD_DVT1W,
    BSIM4_MOD_DVT2W,
    BSIM4_MOD_DROUT,
    BSIM4_MOD_DSUB,
    BSIM4_MOD_VTH0,
    BSIM4_MOD_UA,
    BSIM4_MOD_UA1,
    BSIM4_MOD_UB,
    BSIM4_MOD_UB1,
    BSIM4_MOD_UC,
    BSIM4_MOD_UC1,
    BSIM4_MOD_U0,
    BSIM4_MOD_UTE,
    BSIM4_MOD_VOFF,
    BSIM4_MOD_DELTA,
    BSIM4_MOD_RDSW,
    BSIM4_MOD_PRT,
    BSIM4_MOD_LDD,
    BSIM4_MOD_ETA,
    BSIM4_MOD_ETA0,
    BSIM4_MOD_ETAB,
    BSIM4_MOD_PCLM,
    BSIM4_MOD_PDIBL1,
    BSIM4_MOD_PDIBL2,
    BSIM4_MOD_PSCBE1,
    BSIM4_MOD_PSCBE2,
    BSIM4_MOD_PVAG,
    BSIM4_MOD_WR,
    BSIM4_MOD_DWG,
    BSIM4_MOD_DWB,
    BSIM4_MOD_B0,
    BSIM4_MOD_B1,
    BSIM4_MOD_ALPHA0,
    BSIM4_MOD_BETA0,
    BSIM4_MOD_PDIBLB,
    BSIM4_MOD_PRWG,
    BSIM4_MOD_PRWB,
    BSIM4_MOD_CDSCD,
    BSIM4_MOD_AGS,
    BSIM4_MOD_FRINGE,
    BSIM4_MOD_CGSL,
    BSIM4_MOD_CGDL,
    BSIM4_MOD_CKAPPAS,
    BSIM4_MOD_CF,
    BSIM4_MOD_CLC,
    BSIM4_MOD_CLE,
    BSIM4_MOD_PARAMCHK,
    BSIM4_MOD_VERSION,
    BSIM4_MOD_VFBCV,
    BSIM4_MOD_ACDE,
    BSIM4_MOD_MOIN,
    BSIM4_MOD_NOFF,
    BSIM4_MOD_IJTHDFWD,
    BSIM4_MOD_ALPHA1,
    BSIM4_MOD_VFB,
    BSIM4_MOD_TOXM,
    BSIM4_MOD_TCJ,
    BSIM4_MOD_TCJSW,
    BSIM4_MOD_TCJSWG,
    BSIM4_MOD_TPB,
    BSIM4_MOD_TPBSW,
    BSIM4_MOD_TPBSWG,
    BSIM4_MOD_VOFFCV,
    BSIM4_MOD_GBMIN,
    BSIM4_MOD_RBDB,
    BSIM4_MOD_RBSB,
    BSIM4_MOD_RBPB,
    BSIM4_MOD_RBPS,
    BSIM4_MOD_RBPD,
    BSIM4_MOD_DMCG,
    BSIM4_MOD_DMCI,
    BSIM4_MOD_DMDG,
    BSIM4_MOD_XGW,
    BSIM4_MOD_XGL,
    BSIM4_MOD_RSHG,
    BSIM4_MOD_NGCON,
    BSIM4_MOD_AGIDL,
    BSIM4_MOD_BGIDL,
    BSIM4_MOD_EGIDL,
    BSIM4_MOD_IJTHSFWD,
    BSIM4_MOD_XJBVD,
    BSIM4_MOD_XJBVS,
    BSIM4_MOD_BVD,
    BSIM4_MOD_BVS,
    BSIM4_MOD_TOXP,
    BSIM4_MOD_DTOX,
    BSIM4_MOD_XRCRG1,
    BSIM4_MOD_XRCRG2,
    BSIM4_MOD_EU,
    BSIM4_MOD_IJTHSREV,
    BSIM4_MOD_IJTHDREV,
    BSIM4_MOD_MINV,
    BSIM4_MOD_VOFFL,
    BSIM4_MOD_PDITS,
    BSIM4_MOD_PDITSD,
    BSIM4_MOD_PDITSL,
    BSIM4_MOD_TNOIA,
    BSIM4_MOD_TNOIB,
    BSIM4_MOD_NTNOI,
    BSIM4_MOD_FPROUT,
    BSIM4_MOD_LPEB,
    BSIM4_MOD_DVTP0,
    BSIM4_MOD_DVTP1,
    BSIM4_MOD_CGIDL,
    BSIM4_MOD_PHIN,
    BSIM4_MOD_RDSWMIN,
    BSIM4_MOD_RSW,
    BSIM4_MOD_RDW,
    BSIM4_MOD_RDWMIN,
    BSIM4_MOD_RSWMIN,
    BSIM4_MOD_NSD,
    BSIM4_MOD_CKAPPAD,
    BSIM4_MOD_DMCGT,
    BSIM4_MOD_AIGC,
    BSIM4_MOD_BIGC,
    BSIM4_MOD_CIGC,
    BSIM4_MOD_AIGBACC,
    BSIM4_MOD_BIGBACC,
    BSIM4_MOD_CIGBACC,
    BSIM4_MOD_AIGBINV,
    BSIM4_MOD_BIGBINV,
    BSIM4_MOD_CIGBINV,
    BSIM4_MOD_NIGC,
    BSIM4_MOD_NIGBACC,
    BSIM4_MOD_NIGBINV,
    BSIM4_MOD_NTOX,
    BSIM4_MOD_TOXREF,
    BSIM4_MOD_EIGBINV,
    BSIM4_MOD_PIGCD,
    BSIM4_MOD_POXEDGE,
    BSIM4_MOD_EPSROX,
    BSIM4_MOD_AIGSD,
    BSIM4_MOD_BIGSD,
    BSIM4_MOD_CIGSD,
    BSIM4_MOD_JSWGS,
    BSIM4_MOD_JSWGD,
    BSIM4_MOD_LAMBDA,
    BSIM4_MOD_VTL,
    BSIM4_MOD_LC,
    BSIM4_MOD_XN,
    BSIM4_MOD_RNOIA,
    BSIM4_MOD_RNOIB,
    BSIM4_MOD_VFBSDOFF,
    BSIM4_MOD_LINTNOI,
    BSIM4_MOD_UD,
    BSIM4_MOD_UD1,
    BSIM4_MOD_UP,
    BSIM4_MOD_LP,
    BSIM4_MOD_TVOFF,
    BSIM4_MOD_TVFBSDOFF,
    BSIM4_MOD_MINVCV,
    BSIM4_MOD_VOFFCVL,

    // Length dependence
    BSIM4_MOD_LCDSC,
    BSIM4_MOD_LCDSCB,
    BSIM4_MOD_LCIT,
    BSIM4_MOD_LNFACTOR,
    BSIM4_MOD_LXJ,
    BSIM4_MOD_LVSAT,
    BSIM4_MOD_LAT,
    BSIM4_MOD_LA0,
    BSIM4_MOD_LA1,
    BSIM4_MOD_LA2,
    BSIM4_MOD_LKETA,
    BSIM4_MOD_LNSUB,
    BSIM4_MOD_LNDEP,
    BSIM4_MOD_LNGATE,
    BSIM4_MOD_LGAMMA1,
    BSIM4_MOD_LGAMMA2,
    BSIM4_MOD_LVBX,
    BSIM4_MOD_LVBM,
    BSIM4_MOD_LXT,
    BSIM4_MOD_LK1,
    BSIM4_MOD_LKT1,
    BSIM4_MOD_LKT1L,
    BSIM4_MOD_LK2,
    BSIM4_MOD_LKT2,
    BSIM4_MOD_LK3,
    BSIM4_MOD_LK3B,
    BSIM4_MOD_LW0,
    BSIM4_MOD_LLPE0,
    BSIM4_MOD_LDVT0,
    BSIM4_MOD_LDVT1,
    BSIM4_MOD_LDVT2,
    BSIM4_MOD_LDVT0W,
    BSIM4_MOD_LDVT1W,
    BSIM4_MOD_LDVT2W,
    BSIM4_MOD_LDROUT,
    BSIM4_MOD_LDSUB,
    BSIM4_MOD_LVTH0,
    BSIM4_MOD_LUA,
    BSIM4_MOD_LUA1,
    BSIM4_MOD_LUB,
    BSIM4_MOD_LUB1,
    BSIM4_MOD_LUC,
    BSIM4_MOD_LUC1,
    BSIM4_MOD_LU0,
    BSIM4_MOD_LUTE,
    BSIM4_MOD_LVOFF,
    BSIM4_MOD_LDELTA,
    BSIM4_MOD_LRDSW,
    BSIM4_MOD_LPRT,
    BSIM4_MOD_LLDD,
    BSIM4_MOD_LETA,
    BSIM4_MOD_LETA0,
    BSIM4_MOD_LETAB,
    BSIM4_MOD_LPCLM,
    BSIM4_MOD_LPDIBL1,
    BSIM4_MOD_LPDIBL2,
    BSIM4_MOD_LPSCBE1,
    BSIM4_MOD_LPSCBE2,
    BSIM4_MOD_LPVAG,
    BSIM4_MOD_LWR,
    BSIM4_MOD_LDWG,
    BSIM4_MOD_LDWB,
    BSIM4_MOD_LB0,
    BSIM4_MOD_LB1,
    BSIM4_MOD_LALPHA0,
    BSIM4_MOD_LBETA0,
    BSIM4_MOD_LPDIBLB,
    BSIM4_MOD_LPRWG,
    BSIM4_MOD_LPRWB,
    BSIM4_MOD_LCDSCD,
    BSIM4_MOD_LAGS,

    BSIM4_MOD_LFRINGE,
    BSIM4_MOD_LCGSL,
    BSIM4_MOD_LCGDL,
    BSIM4_MOD_LCKAPPAS,
    BSIM4_MOD_LCF,
    BSIM4_MOD_LCLC,
    BSIM4_MOD_LCLE,
    BSIM4_MOD_LVFBCV,
    BSIM4_MOD_LACDE,
    BSIM4_MOD_LMOIN,
    BSIM4_MOD_LNOFF,
    BSIM4_MOD_LALPHA1,
    BSIM4_MOD_LVFB,
    BSIM4_MOD_LVOFFCV,
    BSIM4_MOD_LAGIDL,
    BSIM4_MOD_LBGIDL,
    BSIM4_MOD_LEGIDL,
    BSIM4_MOD_LXRCRG1,
    BSIM4_MOD_LXRCRG2,
    BSIM4_MOD_LEU,
    BSIM4_MOD_LMINV,
    BSIM4_MOD_LPDITS,
    BSIM4_MOD_LPDITSD,
    BSIM4_MOD_LFPROUT,
    BSIM4_MOD_LLPEB,
    BSIM4_MOD_LDVTP0,
    BSIM4_MOD_LDVTP1,
    BSIM4_MOD_LCGIDL,
    BSIM4_MOD_LPHIN,
    BSIM4_MOD_LRSW,
    BSIM4_MOD_LRDW,
    BSIM4_MOD_LNSD,
    BSIM4_MOD_LCKAPPAD,
    BSIM4_MOD_LAIGC,
    BSIM4_MOD_LBIGC,
    BSIM4_MOD_LCIGC,
    BSIM4_MOD_LAIGBACC,
    BSIM4_MOD_LBIGBACC,
    BSIM4_MOD_LCIGBACC,
    BSIM4_MOD_LAIGBINV,
    BSIM4_MOD_LBIGBINV,
    BSIM4_MOD_LCIGBINV,
    BSIM4_MOD_LNIGC,
    BSIM4_MOD_LNIGBACC,
    BSIM4_MOD_LNIGBINV,
    BSIM4_MOD_LNTOX,
    BSIM4_MOD_LEIGBINV,
    BSIM4_MOD_LPIGCD,
    BSIM4_MOD_LPOXEDGE,
    BSIM4_MOD_LAIGSD,
    BSIM4_MOD_LBIGSD,
    BSIM4_MOD_LCIGSD,

    BSIM4_MOD_LLAMBDA,
    BSIM4_MOD_LVTL,
    BSIM4_MOD_LXN,
    BSIM4_MOD_LVFBSDOFF,
    BSIM4_MOD_LUD,
    BSIM4_MOD_LUD1,
    BSIM4_MOD_LUP,
    BSIM4_MOD_LLP,
    BSIM4_MOD_LMINVCV,

    // Width dependence
    BSIM4_MOD_WCDSC,
    BSIM4_MOD_WCDSCB,
    BSIM4_MOD_WCIT,
    BSIM4_MOD_WNFACTOR,
    BSIM4_MOD_WXJ,
    BSIM4_MOD_WVSAT,
    BSIM4_MOD_WAT,
    BSIM4_MOD_WA0,
    BSIM4_MOD_WA1,
    BSIM4_MOD_WA2,
    BSIM4_MOD_WKETA,
    BSIM4_MOD_WNSUB,
    BSIM4_MOD_WNDEP,
    BSIM4_MOD_WNGATE,
    BSIM4_MOD_WGAMMA1,
    BSIM4_MOD_WGAMMA2,
    BSIM4_MOD_WVBX,
    BSIM4_MOD_WVBM,
    BSIM4_MOD_WXT,
    BSIM4_MOD_WK1,
    BSIM4_MOD_WKT1,
    BSIM4_MOD_WKT1L,
    BSIM4_MOD_WK2,
    BSIM4_MOD_WKT2,
    BSIM4_MOD_WK3,
    BSIM4_MOD_WK3B,
    BSIM4_MOD_WW0,
    BSIM4_MOD_WLPE0,
    BSIM4_MOD_WDVT0,
    BSIM4_MOD_WDVT1,
    BSIM4_MOD_WDVT2,
    BSIM4_MOD_WDVT0W,
    BSIM4_MOD_WDVT1W,
    BSIM4_MOD_WDVT2W,
    BSIM4_MOD_WDROUT,
    BSIM4_MOD_WDSUB,
    BSIM4_MOD_WVTH0,
    BSIM4_MOD_WUA,
    BSIM4_MOD_WUA1,
    BSIM4_MOD_WUB,
    BSIM4_MOD_WUB1,
    BSIM4_MOD_WUC,
    BSIM4_MOD_WUC1,
    BSIM4_MOD_WU0,
    BSIM4_MOD_WUTE,
    BSIM4_MOD_WVOFF,
    BSIM4_MOD_WDELTA,
    BSIM4_MOD_WRDSW,
    BSIM4_MOD_WPRT,
    BSIM4_MOD_WLDD,
    BSIM4_MOD_WETA,
    BSIM4_MOD_WETA0,
    BSIM4_MOD_WETAB,
    BSIM4_MOD_WPCLM,
    BSIM4_MOD_WPDIBL1,
    BSIM4_MOD_WPDIBL2,
    BSIM4_MOD_WPSCBE1,
    BSIM4_MOD_WPSCBE2,
    BSIM4_MOD_WPVAG,
    BSIM4_MOD_WWR,
    BSIM4_MOD_WDWG,
    BSIM4_MOD_WDWB,
    BSIM4_MOD_WB0,
    BSIM4_MOD_WB1,
    BSIM4_MOD_WALPHA0,
    BSIM4_MOD_WBETA0,
    BSIM4_MOD_WPDIBLB,
    BSIM4_MOD_WPRWG,
    BSIM4_MOD_WPRWB,
    BSIM4_MOD_WCDSCD,
    BSIM4_MOD_WAGS,

    BSIM4_MOD_WFRINGE,
    BSIM4_MOD_WCGSL,
    BSIM4_MOD_WCGDL,
    BSIM4_MOD_WCKAPPAS,
    BSIM4_MOD_WCF,
    BSIM4_MOD_WCLC,
    BSIM4_MOD_WCLE,
    BSIM4_MOD_WVFBCV,
    BSIM4_MOD_WACDE,
    BSIM4_MOD_WMOIN,
    BSIM4_MOD_WNOFF,
    BSIM4_MOD_WALPHA1,
    BSIM4_MOD_WVFB,
    BSIM4_MOD_WVOFFCV,
    BSIM4_MOD_WAGIDL,
    BSIM4_MOD_WBGIDL,
    BSIM4_MOD_WEGIDL,
    BSIM4_MOD_WXRCRG1,
    BSIM4_MOD_WXRCRG2,
    BSIM4_MOD_WEU,
    BSIM4_MOD_WMINV,
    BSIM4_MOD_WPDITS,
    BSIM4_MOD_WPDITSD,
    BSIM4_MOD_WFPROUT,
    BSIM4_MOD_WLPEB,
    BSIM4_MOD_WDVTP0,
    BSIM4_MOD_WDVTP1,
    BSIM4_MOD_WCGIDL,
    BSIM4_MOD_WPHIN,
    BSIM4_MOD_WRSW,
    BSIM4_MOD_WRDW,
    BSIM4_MOD_WNSD,
    BSIM4_MOD_WCKAPPAD,
    BSIM4_MOD_WAIGC,
    BSIM4_MOD_WBIGC,
    BSIM4_MOD_WCIGC,
    BSIM4_MOD_WAIGBACC,
    BSIM4_MOD_WBIGBACC,
    BSIM4_MOD_WCIGBACC,
    BSIM4_MOD_WAIGBINV,
    BSIM4_MOD_WBIGBINV,
    BSIM4_MOD_WCIGBINV,
    BSIM4_MOD_WNIGC,
    BSIM4_MOD_WNIGBACC,
    BSIM4_MOD_WNIGBINV,
    BSIM4_MOD_WNTOX,
    BSIM4_MOD_WEIGBINV,
    BSIM4_MOD_WPIGCD,
    BSIM4_MOD_WPOXEDGE,
    BSIM4_MOD_WAIGSD,
    BSIM4_MOD_WBIGSD,
    BSIM4_MOD_WCIGSD,
    BSIM4_MOD_WLAMBDA,
    BSIM4_MOD_WVTL,
    BSIM4_MOD_WXN,
    BSIM4_MOD_WVFBSDOFF,
    BSIM4_MOD_WUD,
    BSIM4_MOD_WUD1,
    BSIM4_MOD_WUP,
    BSIM4_MOD_WLP,
    BSIM4_MOD_WMINVCV,

    // Cross-term dependence
    BSIM4_MOD_PCDSC,
    BSIM4_MOD_PCDSCB,
    BSIM4_MOD_PCIT,
    BSIM4_MOD_PNFACTOR,
    BSIM4_MOD_PXJ,
    BSIM4_MOD_PVSAT,
    BSIM4_MOD_PAT,
    BSIM4_MOD_PA0,
    BSIM4_MOD_PA1,
    BSIM4_MOD_PA2,
    BSIM4_MOD_PKETA,
    BSIM4_MOD_PNSUB,
    BSIM4_MOD_PNDEP,
    BSIM4_MOD_PNGATE,
    BSIM4_MOD_PGAMMA1,
    BSIM4_MOD_PGAMMA2,
    BSIM4_MOD_PVBX,

    BSIM4_MOD_PVBM,

    BSIM4_MOD_PXT,
    BSIM4_MOD_PK1,
    BSIM4_MOD_PKT1,
    BSIM4_MOD_PKT1L,
    BSIM4_MOD_PK2,
    BSIM4_MOD_PKT2,
    BSIM4_MOD_PK3,
    BSIM4_MOD_PK3B,
    BSIM4_MOD_PW0,
    BSIM4_MOD_PLPE0,

    BSIM4_MOD_PDVT0,
    BSIM4_MOD_PDVT1,
    BSIM4_MOD_PDVT2,

    BSIM4_MOD_PDVT0W,
    BSIM4_MOD_PDVT1W,
    BSIM4_MOD_PDVT2W,

    BSIM4_MOD_PDROUT,
    BSIM4_MOD_PDSUB,
    BSIM4_MOD_PVTH0,
    BSIM4_MOD_PUA,
    BSIM4_MOD_PUA1,
    BSIM4_MOD_PUB,
    BSIM4_MOD_PUB1,
    BSIM4_MOD_PUC,
    BSIM4_MOD_PUC1,
    BSIM4_MOD_PU0,
    BSIM4_MOD_PUTE,
    BSIM4_MOD_PVOFF,
    BSIM4_MOD_PDELTA,
    BSIM4_MOD_PRDSW,
    BSIM4_MOD_PPRT,
    BSIM4_MOD_PLDD,
    BSIM4_MOD_PETA,
    BSIM4_MOD_PETA0,
    BSIM4_MOD_PETAB,
    BSIM4_MOD_PPCLM,
    BSIM4_MOD_PPDIBL1,
    BSIM4_MOD_PPDIBL2,
    BSIM4_MOD_PPSCBE1,
    BSIM4_MOD_PPSCBE2,
    BSIM4_MOD_PPVAG,
    BSIM4_MOD_PWR,
    BSIM4_MOD_PDWG,
    BSIM4_MOD_PDWB,
    BSIM4_MOD_PB0,
    BSIM4_MOD_PB1,
    BSIM4_MOD_PALPHA0,
    BSIM4_MOD_PBETA0,
    BSIM4_MOD_PPDIBLB,

    BSIM4_MOD_PPRWG,
    BSIM4_MOD_PPRWB,

    BSIM4_MOD_PCDSCD,
    BSIM4_MOD_PAGS,

    BSIM4_MOD_PFRINGE,
    BSIM4_MOD_PCGSL,
    BSIM4_MOD_PCGDL,
    BSIM4_MOD_PCKAPPAS,
    BSIM4_MOD_PCF,
    BSIM4_MOD_PCLC,
    BSIM4_MOD_PCLE,
    BSIM4_MOD_PVFBCV,
    BSIM4_MOD_PACDE,
    BSIM4_MOD_PMOIN,
    BSIM4_MOD_PNOFF,
    BSIM4_MOD_PALPHA1,
    BSIM4_MOD_PVFB,
    BSIM4_MOD_PVOFFCV,
    BSIM4_MOD_PAGIDL,
    BSIM4_MOD_PBGIDL,
    BSIM4_MOD_PEGIDL,
    BSIM4_MOD_PXRCRG1,
    BSIM4_MOD_PXRCRG2,
    BSIM4_MOD_PEU,
    BSIM4_MOD_PMINV,
    BSIM4_MOD_PPDITS,
    BSIM4_MOD_PPDITSD,
    BSIM4_MOD_PFPROUT,
    BSIM4_MOD_PLPEB,
    BSIM4_MOD_PDVTP0,
    BSIM4_MOD_PDVTP1,
    BSIM4_MOD_PCGIDL,
    BSIM4_MOD_PPHIN,
    BSIM4_MOD_PRSW,
    BSIM4_MOD_PRDW,
    BSIM4_MOD_PNSD,
    BSIM4_MOD_PCKAPPAD,
    BSIM4_MOD_PAIGC,
    BSIM4_MOD_PBIGC,
    BSIM4_MOD_PCIGC,
    BSIM4_MOD_PAIGBACC,
    BSIM4_MOD_PBIGBACC,
    BSIM4_MOD_PCIGBACC,
    BSIM4_MOD_PAIGBINV,
    BSIM4_MOD_PBIGBINV,
    BSIM4_MOD_PCIGBINV,
    BSIM4_MOD_PNIGC,
    BSIM4_MOD_PNIGBACC,
    BSIM4_MOD_PNIGBINV,
    BSIM4_MOD_PNTOX,
    BSIM4_MOD_PEIGBINV,
    BSIM4_MOD_PPIGCD,
    BSIM4_MOD_PPOXEDGE,
    BSIM4_MOD_PAIGSD,
    BSIM4_MOD_PBIGSD,
    BSIM4_MOD_PCIGSD,

    BSIM4_MOD_SAREF,
    BSIM4_MOD_SBREF,
    BSIM4_MOD_KU0,
    BSIM4_MOD_KVSAT,
    BSIM4_MOD_TKU0,
    BSIM4_MOD_LLODKU0,
    BSIM4_MOD_WLODKU0,
    BSIM4_MOD_LLODVTH,
    BSIM4_MOD_WLODVTH,
    BSIM4_MOD_LKU0,
    BSIM4_MOD_WKU0,
    BSIM4_MOD_PKU0,
    BSIM4_MOD_KVTH0,
    BSIM4_MOD_LKVTH0,
    BSIM4_MOD_WKVTH0,
    BSIM4_MOD_PKVTH0,
    BSIM4_MOD_WLOD,
    BSIM4_MOD_STK2,
    BSIM4_MOD_LODK2,
    BSIM4_MOD_STETA0,
    BSIM4_MOD_LODETA0,

    BSIM4_MOD_WEB,
    BSIM4_MOD_WEC,
    BSIM4_MOD_KVTH0WE,
    BSIM4_MOD_K2WE,
    BSIM4_MOD_KU0WE,
    BSIM4_MOD_SCREF,
    BSIM4_MOD_WPEMOD,
    BSIM4_MOD_PMINVCV,

    BSIM4_MOD_PLAMBDA,
    BSIM4_MOD_PVTL,
    BSIM4_MOD_PXN,
    BSIM4_MOD_PVFBSDOFF,

    BSIM4_MOD_TNOM,
    BSIM4_MOD_CGSO,
    BSIM4_MOD_CGDO,
    BSIM4_MOD_CGBO,
    BSIM4_MOD_XPART,
    BSIM4_MOD_RSH,
    BSIM4_MOD_JSS,
    BSIM4_MOD_PBS,
    BSIM4_MOD_MJS,
    BSIM4_MOD_PBSWS,
    BSIM4_MOD_MJSWS,
    BSIM4_MOD_CJS,
    BSIM4_MOD_CJSWS,
    BSIM4_MOD_NMOS,
    BSIM4_MOD_PMOS,
    BSIM4_MOD_NOIA,
    BSIM4_MOD_NOIB,
    BSIM4_MOD_NOIC,
    BSIM4_MOD_LINT,
    BSIM4_MOD_LL,
    BSIM4_MOD_LLN,
    BSIM4_MOD_LW,
    BSIM4_MOD_LWN,
    BSIM4_MOD_LWL,
    BSIM4_MOD_LMIN,
    BSIM4_MOD_LMAX,
    BSIM4_MOD_WINT,
    BSIM4_MOD_WL,
    BSIM4_MOD_WLN,
    BSIM4_MOD_WW,
    BSIM4_MOD_WWN,
    BSIM4_MOD_WWL,
    BSIM4_MOD_WMIN,
    BSIM4_MOD_WMAX,
    BSIM4_MOD_DWC,
    BSIM4_MOD_DLC,
    BSIM4_MOD_XL,
    BSIM4_MOD_XW,
    BSIM4_MOD_EM,
    BSIM4_MOD_EF,
    BSIM4_MOD_AF,
    BSIM4_MOD_KF,
    BSIM4_MOD_NJS,
    BSIM4_MOD_XTIS,
    BSIM4_MOD_PBSWGS,
    BSIM4_MOD_MJSWGS,
    BSIM4_MOD_CJSWGS,
    BSIM4_MOD_JSWS,
    BSIM4_MOD_LLC,
    BSIM4_MOD_LWC,
    BSIM4_MOD_LWLC,
    BSIM4_MOD_WLC,
    BSIM4_MOD_WWC,
    BSIM4_MOD_WWLC,
    BSIM4_MOD_DWJ,
    BSIM4_MOD_JSD,
    BSIM4_MOD_PBD,
    BSIM4_MOD_MJD,
    BSIM4_MOD_PBSWD,
    BSIM4_MOD_MJSWD,
    BSIM4_MOD_CJD,
    BSIM4_MOD_CJSWD,
    BSIM4_MOD_NJD,
    BSIM4_MOD_XTID,
    BSIM4_MOD_PBSWGD,
    BSIM4_MOD_MJSWGD,
    BSIM4_MOD_CJSWGD,
    BSIM4_MOD_JSWD,
    BSIM4_MOD_DLCIG,

    // trap-assisted tunneling
    BSIM4_MOD_JTSS,
    BSIM4_MOD_JTSD,
    BSIM4_MOD_JTSSWS,
    BSIM4_MOD_JTSSWD,
    BSIM4_MOD_JTSSWGS,
    BSIM4_MOD_JTSSWGD,
    BSIM4_MOD_NJTS,
    BSIM4_MOD_NJTSSW,
    BSIM4_MOD_NJTSSWG,
    BSIM4_MOD_XTSS,
    BSIM4_MOD_XTSD,
    BSIM4_MOD_XTSSWS,
    BSIM4_MOD_XTSSWD,
    BSIM4_MOD_XTSSWGS,
    BSIM4_MOD_XTSSWGD,
    BSIM4_MOD_TNJTS,
    BSIM4_MOD_TNJTSSW,
    BSIM4_MOD_TNJTSSWG,
    BSIM4_MOD_VTSS,
    BSIM4_MOD_VTSD,
    BSIM4_MOD_VTSSWS,
    BSIM4_MOD_VTSSWD,
    BSIM4_MOD_VTSSWGS,
    BSIM4_MOD_VTSSWGD,
    BSIM4_MOD_PUD,
    BSIM4_MOD_PUD1,
    BSIM4_MOD_PUP,
    BSIM4_MOD_PLP,
    BSIM4_MOD_JTWEFF,

    BSIM4_MOD_LTVOFF,
    BSIM4_MOD_LTVFBSDOFF,
    BSIM4_MOD_WTVOFF,
    BSIM4_MOD_WTVFBSDOFF,
    BSIM4_MOD_PTVOFF,
    BSIM4_MOD_PTVFBSDOFF,

    BSIM4_MOD_LKVTH0WE,
    BSIM4_MOD_LK2WE,
    BSIM4_MOD_LKU0WE,
    BSIM4_MOD_WKVTH0WE,
    BSIM4_MOD_WK2WE,
    BSIM4_MOD_WKU0WE,
    BSIM4_MOD_PKVTH0WE,
    BSIM4_MOD_PK2WE,
    BSIM4_MOD_PKU0WE,

    BSIM4_MOD_RBPS0,
    BSIM4_MOD_RBPSL,
    BSIM4_MOD_RBPSW,
    BSIM4_MOD_RBPSNF,
    BSIM4_MOD_RBPD0,
    BSIM4_MOD_RBPDL,
    BSIM4_MOD_RBPDW,
    BSIM4_MOD_RBPDNF,

    BSIM4_MOD_RBPBX0,
    BSIM4_MOD_RBPBXL,
    BSIM4_MOD_RBPBXW,
    BSIM4_MOD_RBPBXNF,
    BSIM4_MOD_RBPBY0,
    BSIM4_MOD_RBPBYL,
    BSIM4_MOD_RBPBYW,
    BSIM4_MOD_RBPBYNF,

    BSIM4_MOD_RBSBX0,
    BSIM4_MOD_RBSBY0,
    BSIM4_MOD_RBDBX0,
    BSIM4_MOD_RBDBY0,

    BSIM4_MOD_RBSDBXL,
    BSIM4_MOD_RBSDBXW,
    BSIM4_MOD_RBSDBXNF,
    BSIM4_MOD_RBSDBYL,
    BSIM4_MOD_RBSDBYW,
    BSIM4_MOD_RBSDBYNF,

    BSIM4_MOD_AGISL,
    BSIM4_MOD_BGISL,
    BSIM4_MOD_EGISL,
    BSIM4_MOD_CGISL,
    BSIM4_MOD_LAGISL,
    BSIM4_MOD_LBGISL,
    BSIM4_MOD_LEGISL,
    BSIM4_MOD_LCGISL,
    BSIM4_MOD_WAGISL,
    BSIM4_MOD_WBGISL,
    BSIM4_MOD_WEGISL,
    BSIM4_MOD_WCGISL,
    BSIM4_MOD_PAGISL,
    BSIM4_MOD_PBGISL,
    BSIM4_MOD_PEGISL,
    BSIM4_MOD_PCGISL,

    BSIM4_MOD_AIGS,
    BSIM4_MOD_BIGS,
    BSIM4_MOD_CIGS,
    BSIM4_MOD_LAIGS,
    BSIM4_MOD_LBIGS,
    BSIM4_MOD_LCIGS,
    BSIM4_MOD_WAIGS,
    BSIM4_MOD_WBIGS,
    BSIM4_MOD_WCIGS,
    BSIM4_MOD_PAIGS,
    BSIM4_MOD_PBIGS,
    BSIM4_MOD_PCIGS,
    BSIM4_MOD_AIGD,
    BSIM4_MOD_BIGD,
    BSIM4_MOD_CIGD,
    BSIM4_MOD_LAIGD,
    BSIM4_MOD_LBIGD,
    BSIM4_MOD_LCIGD,
    BSIM4_MOD_WAIGD,
    BSIM4_MOD_WBIGD,
    BSIM4_MOD_WCIGD,
    BSIM4_MOD_PAIGD,
    BSIM4_MOD_PBIGD,
    BSIM4_MOD_PCIGD,
    BSIM4_MOD_DLCIGD,

    BSIM4_MOD_NJTSD,
    BSIM4_MOD_NJTSSWD,
    BSIM4_MOD_NJTSSWGD,
    BSIM4_MOD_TNJTSD,
    BSIM4_MOD_TNJTSSWD,
    BSIM4_MOD_TNJTSSWGD
};

#endif // B4DEFS_H

