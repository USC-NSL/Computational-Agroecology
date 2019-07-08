#include "q_learning.h"

namespace agent {

Qlearning::Qlearning(const std::string& name, environment::Environment* env):Agent(name,env){
  name_ = name;
  env_ = env;
  for(int  i = 0; i < ROW; i++){
    for(int j = 0; j < COLUMN; j++){
      q_table[ROW][COLUMN] = 0;
    }
  }
}

int Qlearning::Execute(int days){
  while(days > 0){


    days--;
  }
  return days;
}
  
void Qlearning::Update_agent_q_table(int current_state, int reward, int next_state, int action_taken){
  int current_q = *q_table[current_state, action_taken];
  int max_q_future = max_q();
  int new_q = (1 - LEARNING_RATE) * current_q + LEARNING_RATE * (reward + DISCOUNT * max_q_future);
  *q_table[current_state, action_taken] = new_q;
  return;
}

int Qlearning::max_q(){
  std::random_device ramdom_number; 
  // seed the generator
  std::mt19937 seed(ramdom_number()); 

  std::uniform_int_distribution<> action_Distr(0, ROW - 1);
  int random_state_index =  action_Distr(seed);
  return *std::max_element(q_table[random_state_index], q_table[random_state_index] + COLUMN);

}


}  // namespace agent