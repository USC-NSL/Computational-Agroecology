#include "q_learning.h"

namespace agent {

Qlearning::Qlearning(const std::string& name, environment::Environment* env, int row, int column):Agent(name,env){
  name_ = name;
  env_ = env;
  row_ = row;
  column_ = column;
  q_table = new int[row*column];
  for(int  i = 0; i < row_ ; i++){
    for(int j = 0; j < column_; j++){
      //  q_table[i][j] is as q_table[i * column + j]
      q_table[i * column_ + j] = RandomInt(0,2);

    }
  }
}

simulator::action::Action* Qlearning::interpreter(int action_taken){
  int crop_id = action_taken < (column_ /2 ) ? 1 : 2;
  if(crop_id == 1){
    if(1<= action_taken  && action_taken <= 9){
      int action_index = 1;
      int row_index = 9/action_taken;
      int col_index = 9;
    }
  }else if(crop_id == 2){

  }
  return NULL;
}

int Qlearning::Execute(int days){
  //To Do
  while(days > 0){
    int current_state = RandomInt(0, row_);
    int action_taken = Max_q_action(current_state);
    auto action = interpreter(action_taken);
    ///env.ReceiveAction(action);
    int next_state =  RandomInt(0, row_);
    Update_agent_q_table(current_state, 0, next_state, action_taken);
    days--;
  }
  return days;
}
  
void Qlearning::Update_agent_q_table(int current_state, int reward, int next_state, int action_taken){
  //q_table[i][j] is as q_table[i * column + j]
  int current_q = q_table[current_state * column_ + action_taken];
  int max_q_future = Max_q(next_state);
  int new_q = (1 - LEARNING_RATE) * current_q + LEARNING_RATE * (reward + DISCOUNT * max_q_future);
  q_table[current_state * column_ + action_taken] = new_q;
  return;
}


int Qlearning::Max_q(int state){
  // get the max value in a state
  //  q_table[i][j] is as q_table[i * column + j]
  int max =  *std::max_element(q_table + state * column_, q_table + state * column_ + column_);
  return max;
}

int Qlearning::Max_q_action(int state){
  // get the max value in a state
  //  q_table[i][j] is as q_table[i * column + j]
  int index = std::distance(q_table + state * column_, std::max_element(q_table + state * column_, q_table + state * column_ + column_));
  return index;
}

}  // namespace agent