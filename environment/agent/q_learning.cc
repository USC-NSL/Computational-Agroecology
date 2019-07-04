#include "q_learning.h"

namespace agent {

void Qlearning::Update_agent_q_table(int current_state, int reward, int next_state, int action_taken){
  int current_q = q_table[current_state, action_taken];
  int max_q_future = q_table[current_state, action_taken];
  int new_q = (1 - LEARNING_RATE) * current_q + self.LEARNING_RATE * (reward + self.DISCOUNT * max_q_future)
  return;
}


}  // namespace agent