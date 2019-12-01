Hope this file can help you understand the changes I made in uSimMarine and save your precious time!

There are three modes in uSimMarine now,

Mode:
(1) dual_state          : on/off   (bool m_dual_state          ,default:false)
(2) drift_state         : on/off   (bool m_drift_state         ,default:false)
(3) wrong_heading_state : on/off   (bool m_wrong_heading_state ,default:false)

So there are 2*2*2 = 8 possibilities.

 dual | drift | heading |  (0:off / 1:on)
-------------------------
  0   |   0   |    0    |  ==> Case 1: dual_state off, drift_state off, wrong_heading_state off
-------------------------
  0   |   0   |    1    |  ==> Case 2: dual_state off, drift_state off, wrong_heading_state on (and so on)
-------------------------
  0   |   1   |    0    |  ==> Case 3
-------------------------
  0   |   1   |    1    |  ==> Case 4
-------------------------
  1   |   0   |    0    |  ==> Case 5
-------------------------
  1   |   0   |    1    |  ==> Case 6
-------------------------
  1   |   1   |    0    |  ==> Case 7
-------------------------
  1   |   1   |    1    |  ==> Case 8
-------------------------

Calculate propagation:(Input)
=================================================================================================================================================
In USM_MOOSAPP.cpp:Iterate()
                                                                                                        bool apply_wrong_heading
                                                                                                                   |           
                                                                                                                   v           
                                                                propagateNodeRecord(m_record, delta_time, false, false) ---
                                                               /                                                          |
                                                              /                                                           | 
                                    if(m_dual_state == ture)--                                                            |--- for Case 5,6,7,8
                                   /                          \                                                           |
                                  /                            \                                                          |
                                 /                              propagateNodeRecord(m_record_gt, delta_time, true, true)---
m_model.propagate(m_curr_time) --
                                 \   
                                  \    
                                   \                
                                    else -- propagateNodeRecord(m_record, delta_time, true, true) --------------------------- for Case 1,2,3,4

=================================================================================================================================================


Notify:(Output)
=================================================================================================================================================
In USM_MOOSAPP.cpp:Iterate()


       if(m_dual_state = false)
      /   postNodeRecordUpdate(m_sim_prefix, record) --- for Case 1,2,3,4
     /
    /
   /
  /
-------else if(m_dual_state = false && m_wring_heading_mode == ture && m_drift == false)  ---- for Case 6
  \       postNodeRecordUpdate(m_sim_prefix+"_GT", record_gt)
   \      postNodeRecordUpdate_wrong_heading_state(m_sim_prefix, record, record_gt)
    \
     \
      \
       else                                               
          postNodeRecordUpdate(m_sim_prefix, record);   ------------- for Case 5,7,8
          postNodeRecordUpdate(m_sim_prefix+"_GT", record_gt)


=================================================================================================================================================


