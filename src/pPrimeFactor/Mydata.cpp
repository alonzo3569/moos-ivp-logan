/***********************************************************/
/*    NAME: ual                                              */
/*    ORGN: MIT                                             */
/*    FILE: Mydata.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include <cmath>
#include "MBUtils.h"
#include "Mydata.h"
//#include <cstdint>
#include <cstdlib>

using namespace std;

//---------------------------------------------------------
// Constructor

Mydata::Mydata()
{
   // x =0;
  // m_buff.clear();
    m_primebuff.clear();
    m_unsolvebuff.clear();
}

//---------------------------------------------------------
// Destructor

Mydata::~Mydata()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

void  Mydata::setori(uint64_t input)
{
    m_origin = input;   
    //cout << m_origin << endl;    
    //cout << input << endl;    
}

//---------------------------------------------------------
void  Mydata::primefactor()
{
    // if (m_origin%2 == 1){ cout << m_origin << " is odd" <<  endl;}
          // else { cout << m_origin << " is even" << endl;}
    uint64_t b = 2;
    c = m_origin;
    int counter = 1;

    // process:
    for( b ; b <= sqrt(c);b++)
    {
        if (m_origin % b == 0)
               {
                     m_primebuff.push_back(b); //
                     m_origin = m_origin /b;  // 
                     b = b-1;
                     counter++;  
                     //Notify("ODOMETRY_DIST",counter);
                     //cout << b << endl;
               }
        //if ( counter == 100000){break;}
    }
    if (m_origin != 1)
    {
       m_primebuff.push_back(m_origin);
    }
   // if (counter == 100000)
   // {
   //     m_origin = unsolveprime;
   //     c = unsolveorigin;
   //     while (!m_primebuff.empty())
   //     {
   //         uint64_t x;
   //         x = m_primebuff.front();
   //         m_primebuff.pop_front();
   //         m_unsolvebuff.push_back(x);
   //     }
   // }
   // if (counter < 100000)
   // {
   //     if (!m_unsolvebuff.empty())
   //         {
   //              d = unsolveprime;
   //              for( b ; b <= sqrt(d);b++)
   //              {
   //                  if (unsolveprime % b == 0)
   //                  {
   //                      m_unsolvebuff.push_back(b);
   //                      unsolveprime = unsolveprime /b;
   //                      b = b-1;
   //                  }
   //                  else
   //                  {
   //                      if(unsolveprime != 1)
   //                      {
   //                          m_unsolvebuff.push_back(unsolveprime);
   //                      }
   //                  }
   //              }
   //         }
   //     if (m_origin != 1)
   //     {
   //         m_primebuff.push_back(m_origin);
   //     }    

   // }
}
//-------------------------------------------------------------
void  Mydata::printpfactorstring()
{    
    // print out the result:
    // stringstream ss_output;
    //ss_output << "orig = " << c << "received= calculated= ,solve_time= " << "primes" <<  
    cout << c << " primefactor";
    deque<uint64_t>::iterator p;
    for(p = m_primebuff.begin(); p != m_primebuff.end(); p++)
    {
         cout << ":" << *p;
         //int  x;
         //x = m_primebuff.front();
         //cout << x << endl;
         //m_primebuff.pop_front();
    }
    //<< "username = alonzo" << endl;
    // terminate the program:
    cout << endl;
}

//---------------------------------------------------------
void  Mydata::gettimespend(float time)
{
    m_timeuse = time;
}
//---------------------------------------------------------
string  Mydata::buildreport()
{

    stringstream ss;
    ss << "orig = " << c << ",calculate time = " << m_timeuse << ",primefactor";

    while(!m_primebuff.empty())
    {
        uint64_t x;
        x = m_primebuff.front();
        ss << ":" << x;
        m_primebuff.pop_front();
    }
    
    //if (!m_unsolvebuff.empty())
    //{
    //    cout << "orig = " << unsolveorigin << ",calculate time = " << m_timeuse << ",primefactor";
    //    while(!m_unsolvebuff.empty())
    //    {
    //        
    //        uint64_t x;
    //        x = m_unsolvebuff.front();
    //        cout << ":" << x;
    //        m_unsolvebuff.pop_front();
    //    }
    //cout << endl;
    //}
    return(ss.str());

}
//---------------------------------------------------------
//void Mydata::printpfactorstring()
//{
//    //deque<uint64_t>::iterator p;
//    //for(p = m_primebuff.begin(); p != m_primebuff.end(); p++)
//    if(!m_primebuff.empty())
//    {  
//      string type;
//      output_str;
//      stringstream ss_output;
//      
//      uint64_t x;
//      x = m_primebuff.front();
//      ss_output << x;
//      
//    } 
//
//} 



