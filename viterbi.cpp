#include <iostream>
#include <algorithm>
#include <math.h>
#include <fstream>
#include <vector>

using namespace std;

class HMM {
    private:
        vector<vector<double> > transit;
        int hidStatesNum;
        vector<int> obs;
        vector<vector<double> > emission;
        vector<double> initialState;
    public:
        //Constructor for the Hidden Markov Model.
        //Scans the input files and creates the required matrices
        HMM (int obStatesNum, string emissionFile, string transitFile, string initFile, string obsFile) {
            ifstream emitF, transitF, initF, obsF;

            emitF.open(emissionFile);
            transitF.open(transitFile);
            initF.open(initFile);
            obsF.open(obsFile);

            while(true) {
                vector<double> k;
                for (int i = 0; i < obStatesNum; i++) {
                    double l;
                    emitF >> l;
                    k.push_back(log(l));
                }
                if(emitF.eof())
                    break;
                emission.push_back(k);
            }

            while(true) {
                double k;
                initF >> k;
                if (initF.eof())
                    break;
                initialState.push_back(log(k));
            }

            hidStatesNum = initialState.size();

            while(true) {
                vector<double> l;
                for (int i = 0; i < hidStatesNum; i++) {
                    double k;
                    transitF >> k;

                    l.push_back(log(k));

                }
                if (transitF.eof())
                    break;
                transit.push_back(l);
            }

            initF.close();
            transitF.close();
            emitF.close();

            while (true) {
                int k;
                obsF >> k;

                if (obsF.eof())
                    break;
                obs.push_back(k);
            }



            obsF.close();


        }

        //Uses a log-space approach to avoid rounding to zero
        vector<int> viterbi() {
            vector<vector<double> > probs(this->hidStatesNum);
            vector<vector<int> > pathTrace(this->hidStatesNum);

            //Initialize the state probs and state path matrices
            for (int i = 0; i < probs.size(); i++) {
                for(int j = 0; j < this->obs.size(); j++) {
                    probs[i].push_back(0.0);
                    pathTrace[i].push_back(0);
                }
            }

            //Initialize the first column of the state probs matrix
            for (int i = 0; i < this->hidStatesNum; i++)
                probs[i][0] = emission[i][obs[0]] + initialState[i];

            double x1;
            int x2;
            //Calculate the state probability for each state at each time step
            for (int i = 1; i < probs[0].size(); i++) {
                for (int j = 0; j < this->hidStatesNum; j++) {
                    double emissionProb = emission[j][obs[i]];
                    x1 = probs[0][i-1] + transit[0][j] + emissionProb;
                    x2 = 0;

                    double current;
                    //Choose the highest probability incoming path
                    for (int k = 1; k < this->hidStatesNum; k++) {
                        current = probs[k][i-1] + transit[k][j] + emissionProb;

                        if (current > x1) {
                            x1 = current;
                            x2 = k;
                        }
                    }

                    probs[j][i] = x1;
                    pathTrace[j][i] = x2;

                }
            }

            double finalP;
            int finalS;

            finalP = probs[0][this->obs.size() - 1];
            finalS = 0;

            double current;
            //Get the state at the final time step
            for (int i = 1; i < this->hidStatesNum; i++) {
                current = probs[i][this->obs.size() - 1];

                if (current > finalP) {
                    finalP = current;
                    finalS = i;
                }
            }

            vector<int> stateTrace;
            stateTrace.push_back(finalS);
            int prev = finalS;

            //Trace back the time steps from the final step
            //And record states
            for (int i = this->obs.size() - 1; i > 0; i--) {
                prev = pathTrace[prev][i];
                stateTrace.push_back(prev);
            }

            //Print results
            char c, s;
            bool first = true;
            std::reverse(stateTrace.begin(), stateTrace.end());
            ofstream results("results.txt");
            int i = 1;
            for (auto it = stateTrace.begin(); it != stateTrace.end(); it++) {
                results << i << '\t' << (*it) + 1 << endl;
                i++;
                c = char((*it) +65);
                if (*it == 26)
                    c = ' ';
                if (first) {
                    s = c;
                    cout << c;
                    first = false;
                }
                if (c != s) {
                    cout << c;
                    s = c;
                }
            }
        }
};

int main(int argc, char* argv[]) {
    if (argc != 5) {
        cout << "Usage: ./a.out emission transit init observations" << endl;
        exit(0);
    }
    HMM speechRecog(2, argv[1], argv[2], argv[3], argv[4]);
    speechRecog.viterbi();
}
