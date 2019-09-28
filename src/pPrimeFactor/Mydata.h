/************************************************************/
/*    NAME: ual                                              */
/*    ORGN: MIT                                             */
/*    FILE: Mydata.h                                          */
/*    DATE:                                                 */
/************************************************************/

#ifndef Mydata_HEADER
#define Mydata_HEADER

#include "MOOS/libMOOS/MOOSLib.h"
#include<iostream>
#include<deque>
class Mydata 
{
 public:
   Mydata();
   ~Mydata();
   void setori(uint64_t);
   void primefactor();
   void gettimespend(float);
   void printpfactorstring();
   std::string buildreport();
   std::deque<uint64_t>m_primebuff;
   std::deque<uint64_t>m_unsolvebuff;
   uint64_t c;
   uint64_t d;
   uint64_t unsolveprime;
   uint64_t unsolveorigin;
   float m_timeuse;
   float m_timeuse2;
   //pushbackm_origin(); 
 protected:

 private: // Configuration variables

 private: // State variables
 //int x;   
 //std::string  x ;
    uint64_t m_origin;  

};

#endif 
