/* PROJECT 4 - project_4.cpp

    Name: Kevin Bui
    Email: kevinbui@temple.edu
    Date: 04/11/2025

    Implements a priority encoder using cpp's built in priority encoder to simulate an 
    emergency room triage program, the more servre the injurt the less wait time they should have.

    Make:
        make
    Run:
        /proj_4 <hospitalData.txt OR hospitalDataShort.txt>
*/

#include<iostream>
#include<bits/stdc++.h>
#include<fstream>

using namespace std;

// creating a patient struct
struct patient {
    // initalize patiend id, severtiy, and arrival time
    string patient_id;
    int severity;
    int arrival_time;

    // constructor 
    patient(string id, int s, int a_t) : patient_id(id), severity(s), arrival_time(a_t) {} 
};

// going off the example in demo1.cpp, i will make a struct to define lower patient priority
struct customLowerPriority {
    // i am customizing the struct to handle a case where the severity is the same
    bool operator()(const patient& a, const patient& b) const {
        // this is checking if the severities are the same, if so return the lower priority for who arrived later
        if (a.severity == b.severity){
            return a.arrival_time > b.arrival_time;
        }
        // returning this as true than a is considered less severe then b
        return a.severity > b.severity;
    }
};

// creating a parseFile function that takes two inputs of a file name and a vector of patients to store
void parseFile(const string& filename, vector<patient>& patients){
    // using amperstands in function calls to use the orignal and not make a copy, stops it getting destroyed when exiting scope
    // to read the file i will be using the method provided in demo2.cpp
    string line, substring, id;
    int severity, arrival_time;
    stringstream ss;  

    // opening input file as a stream
    // this allows us to read the data piece by piece instead of all at oncee
    ifstream fileID(filename);    
    
    if (!fileID.is_open()){
        cerr << "could not open " << filename << endl;
        return;
    }

    while (getline(fileID,line)){
        // to split <line> into substrings (using spaces as the delimiter):

        // convert <line> into a stream
        ss.str( line );

        // take the first substring that ends in ' '
        getline(ss, id ,' ');

        // take the second substring that ends in ' '
        getline(ss,substring,' ');
        severity = stoi(substring);

        // take the third substring that ends in ' '
        getline(ss,substring,' ');
        arrival_time = stoi(substring);

        // push_back everything into patients by creating a patient object w/ id, severity, and arrival time
        patients.push_back(patient(id, severity, arrival_time));

        // clear the stream for the next pass through the loop
        ss.clear();
        // clearing the contents of the stream
        ss.str("");
    }

    // close the input file
    fileID.close();
}

void hospitalSim(const vector<patient>& patients, const string& output_file){
    // there is only 1 doctor and it takes 5 minutes for each patient, new patients are placed in the priority queue
    // open the output file to write to it
    ofstream fileID(output_file);
    if (!fileID.is_open()) {
        cerr << "could not open " << output_file << endl;
        return;
    }
    // first we will create the priority queue of patients, i am using demo1.cpp as a template
    priority_queue<patient, vector<patient>, customLowerPriority> waitList;

    // set varibles needed to run the simulation
    int currTime = 0;
    int nextIdx = 0; 
    // edited my original code for 2 doctor extra credit
    bool doctorBusy[2] = {false, false};
    int doctorDone[2] = {0, 0};

    // begin the simulation loop
    while (!waitList.empty() || nextIdx < patients.size()){
        // add patients who arrive at the currTime
        while (nextIdx < patients.size() && patients[nextIdx].arrival_time == currTime){
            waitList.push(patients[nextIdx]);
            nextIdx++;
        }
        
        // check if doctor is done treating
        for(int i = 0; i < 2; i++){
            if (doctorBusy[i] && currTime == doctorDone[i]){
                // free the doctor
                doctorBusy[i] = false;
            }
        }
        for (int i = 0; i < 2; i++){
            // doctor becomes available and takes patient w/ highest priority
            if (!doctorBusy[i] && !waitList.empty()){
                // use top since it returns the highest priority element
                patient next = waitList.top();
                // after setting a new patient class to the highest priority pop that element
                waitList.pop();

                // set the waiting time of the patient
                int waitTime = currTime - next.arrival_time;
                fileID << next.severity << "," << waitTime << endl;

                // set doctor busy to true
                doctorBusy[i] = true;
                doctorDone[i] = currTime + 5;
            }
        }

        // increment currTime to the next minute
        currTime++;
    }
    
    // close the output file
    fileID.close();
}

int main(int argc, char** argv){
    if (argc != 2){
        // we only need 2 since no matter what we'll be writing the output to results.csv
        cerr << "usage: " << argv[0] << " <hospitalData.txt OR hospitalDataShort.txt>" << endl;
        return 1;
    }

    // setting the input and output files
    string input_file = argv[1];
    string output_file = "results.csv";

    // creating a vector of patients to store parsed arguments and calling parseFile function
    vector<patient> patients;
    parseFile(input_file, patients);

    // run the hospital simulation
    hospitalSim(patients, output_file);

    // end of main
    return 0;
}