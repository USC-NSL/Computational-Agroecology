#ifndef COMPUTATIONAL_AGROECOLOGY_AGENT_Q_LEARNING_H_
#define COMPUTATIONAL_AGROECOLOGY_AGENT_Q_LEARNING_H_

#include "agent.h"
#include <algorithm>


namespace agent {
    const int ROW = 10;
    const int COLUMN = 54;

class Qlearning : public Agent {
  public:
    Qlearning(const std::string& name, environment::Environment* env);
    void Update_agent_q_table(int current_state, int reward, int next_state, int action_taken);
    int max_q();
    int Execute(int days);

  private:
    std::string name_;
    // The corresponding environment that this agent is interacting with
    environment::Environment* env_;
    // std::unordered_map<simulator::ResourceType, size_t> owned_resource_;
    int q_table[ROW][COLUMN];
    const int LEARNING_RATE = 0.1;
    const int DISCOUNT = 0.95;

};

}  // namespace agent

#endif  // COMPUTATIONAL_AGROECOLOGY_AGENT_AGENT_H_