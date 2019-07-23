#include "q_learning.h"

namespace agent {

Qlearning::Qlearning(const std::string &name, environment::Environment *env,
                     int row, int column)
    : Agent(name, env), row_(row), column_(column) {
  qtable_ = new int[row * column];
  for (int i = 0; i < row_; i++) {
    for (int j = 0; j < column_; j++) {
      //  qtable_[i][j] is as qtable_[i * column + j]
      qtable_[i * column_ + j] = RandomInt(0, 2);
    }
  }
}

Qlearning::~Qlearning() {
  delete[] qtable_;
}

simulator::action::Action *Qlearning::interpreter(int action_taken_) {  // To do
  return nullptr;
}

int Qlearning::Execute(int days) {
  while (days > 0) {
    int current_state = RandomInt(0, row_);
    int action_taken = MaxQAction(current_state);
    // TODO: auto action = interpreter(action_taken) and
    // env.ReceiveAction(action)
    int next_state = RandomInt(0, row_);
    UpdateQtable(current_state, 0, next_state, action_taken);
    days--;
  }
  return days;
}

void Qlearning::UpdateQtable(int current_state, int reward, int next_state,
                             int action_taken) {
  // qtable_[i][j] is as qtable_[i * column + j]
  int current_q = qtable_[current_state * column_ + action_taken];
  int max_q_future = MaxQ(next_state);
  int new_q = (1 - kLearningRate) * current_q +
              kLearningRate * (reward + kDiscount * max_q_future);
  qtable_[current_state * column_ + action_taken] = new_q;
  return;
}

int Qlearning::MaxQ(int state) {
  // get the max value in a state
  //  qtable_[i][j] is as qtable_[i * column + j]
  int max = *std::max_element(qtable_ + state * column_,
                              qtable_ + state * column_ + column_);
  return max;
}

int Qlearning::MaxQAction(int state) {
  // get the max value in a state
  //  qtable_[i][j] is as qtable_[i * column + j]
  int index =
      std::distance(qtable_ + state * column_,
                    std::max_element(qtable_ + state * column_,
                                     qtable_ + state * column_ + column_));
  return index;
}

}  // namespace agent
