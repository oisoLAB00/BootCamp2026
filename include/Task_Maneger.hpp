#pragma once
#define LED1_PIN 14
#define LED2_PIN 13
#define LED3_PIN 15


namespace Task_Maneger{
    enum class task_flow{STOP, CALIB, WORK};
    enum class hand_state{OPEN, CATCH}; 
}


struct UICommand {
    bool start_calib = false;
    bool stop = false;
    bool work = false;
};
/*
class Task_Maneger{
  public :
    enum class task_flow{STOP, CALIB, WORK};
    enum class hand_state{OPEN, PRE_CATCH, CATCH};
    Task_Maneger(){

    }
    ~Task_Maneger(){
      
    }
    void init_calib();
    
    void set_task(task_flow task, hand_state hand);



  private :


};
*/

