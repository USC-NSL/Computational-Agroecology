#ifndef COMPUTATIONAL_AGROECOLOGY_AGENT_Q_LEARNING_H_
#define COMPUTATIONAL_AGROECOLOGY_AGENT_Q_LEARNING_H_

#include "agent.h"

#include <algorithm>

namespace agent {

class Qlearning : public Agent {
public:
  Qlearning(const std::string &name, environment::Environment *env, int row_,
            int column_);

  ~Qlearning();

  // update q_table value
  void UpdateQtable(int current_state, int reward, int next_state,
                    int action_taken);
  // select max Q value in a state
  int MaxQ(int state);
  // return max q action index
  int MaxQAction(int state);

  //  TODO:  add function from action index to action object
  simulator::action::Action *interpreter(int action_taken_);
  // TODO: to add execute q leanring cuntion.
  int Execute(int days);

private:
  int row_;
  int column_;
  int *qtable_;
  const static int kLearningRate = 0.1;
  const static int kDiscount = 0.95;
};

} // namespace agent

#endif // COMPUTATIONAL_AGROECOLOGY_AGENT_AGENT_H_
