#ifndef COMPUTATIONAL_AGROECOLOGY_AGENT_Q_LEARNING_H_
#define COMPUTATIONAL_AGROECOLOGY_AGENT_Q_LEARNING_H_


#include "agent.h"


namespace agent {


class Qlearning public : Agent {
  public:
    void Update_agent_q_table(int current_state, int reward, int next_state, int action_taken));
   

  private:
  int q_table[10][54];
  const int LEARNING_RATE = 0.1
  const int DISCOUNT = 0.95



}  // namespace agent

#endif  // COMPUTATIONAL_AGROECOLOGY_AGENT_AGENT_H_