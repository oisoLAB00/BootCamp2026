

class Task_Maneger{
  public :
    enum class task_flow{STOP, CALIB, WORK};
    enum class hand_state{CATCH, PRE_CATCH, OPEN};
    Task_Maneger(){

    }
    ~Task_Maneger(){
      
    }
    void init_calib();
    
    void set_task(task_flow task, hand_state hand);



  private :


};
