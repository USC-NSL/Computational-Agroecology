using namespace std;

class Plant {
    private:
        int id, curState, yield, gdd, liveState, deadState;
        bool living, flowering, perennial;
        int l_states[][]; // see if there is better data structure
        int d_states[][];

    public:
        Plant(int id);
        void transition(int env[]);
        bool checkNeeds(int env[]);
        int get_state();

    };
