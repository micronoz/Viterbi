#import <iostream>
#import <fstream>
#import <vector>

using namespace std;

class HMM {
    private:
        vector<vector<double> > transit;
        int hidStatesNum;
        vector<int> obs;
        vector<vector<double> > emission;
        vector<double> initialState;
    public:
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
                    k.push_back(l);
                }
                emission.push_back(k);
            }

            while(true) {
                if (initF.eof())
                    break;
                double k;
                initF >> k;
                initialState.push_back(k);
            }

            hidStatesNum = initialState.size();

            while(true) {
                if (transitF.eof())
                    break;
                vector<double> l;
                for (int i = 0; i < hidStatesNum; i++) {
                    double k;
                    transitF >> k;
                      
                    l.push_back(k);

                }
                transit.push_back(l);
            }

            initF.close();
            transitF.close();
            emitF.close();

            while (true) {
                if (obsF.eof())
                    break;
                int k;
                obsF >> k;

                obs.push_back(k);
            }



            obsF.close();


            cout << emission.size() << endl;
            cout << emission[0].size() << endl;
            cout << initialState.size() << endl;
            cout << transit.size() << endl;
            cout << transit[0].size() << endl;
        }
};

int main(int argc, char* argv[]) {
    if (argc != 5)
        cout << "Usage: ./a.out emission transit init observations" << endl;
    HMM speechRecog(2, argv[2], argv[3], argv[4], argv[5]);
}
