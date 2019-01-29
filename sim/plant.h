using namespace std;

class Plant {

    int id, curr_state, yield, gdd, live_state, dead_state;
    bool living, flowering;
    int l_states[][]; // see if there is better data structure
    int d_states[][];

    public:
        Plant(int id);
        void Transition(int env[]);
        bool CheckNeeds(int env[]);

    };
