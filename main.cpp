#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
using namespace std;

struct Pelajar {
    string name;
    int marks[5]; // BM, ENG, MATH, SCI, HIS
    float percentage;
    string overallGrade;
};

extern const int CLASS_COUNT;
extern vector<Pelajar> classes[];

string nakGredStr(int );
int nakMarkah(const string &subject);
void calculateResults(Pelajar &);
void saveToFile(int );
void loadFromFile(int );
int selectClass();
void menu(int );
bool passcheck(int);


int main() {
    bool access;
    for (int i = 0; i < CLASS_COUNT; ++i)
        loadFromFile(i);

    while (true) {
        int classIndex = selectClass();
        access=passcheck(classIndex);
        if(access==true){
            menu(classIndex);  
        }
        else{
            cout<<"ACCESS DENIEDD\n";
            exit(0);
        }
        
    }
    return 0;
}
const int CLASS_COUNT = 4;
vector<Pelajar> classes[CLASS_COUNT];
string filenames[CLASS_COUNT] = {"class_A.csv", "class_B.csv", "class_C.csv", "class_D.csv"};

string nakGredStr(int mark) {
    if (mark >= 90 && mark <= 100) 
        return "A+";
    if (mark >= 80 && mark <= 89) 
        return "A";
    if (mark >= 70 && mark <= 79) 
        return "A-";
    if (mark >= 65 && mark <= 69) 
        return "B";
    if (mark >= 60 && mark <= 64) 
        return "B-";
    if (mark >= 55 && mark <= 59) 
        return "C";
    if (mark >= 50 && mark <= 54)
        return "C-";
    if (mark >= 45 && mark <= 49)
        return "D";
    if (mark >= 40 && mark <= 44) 
        return "E";
    if (mark >= 0 && mark <= 39) 
        return "G";
    return "Invalid Mark";//if the inputted mark not met any criteria
}

int nakMarkah(const string &subject) {
    int mark;
    do {
        cout << subject << ": ";
        cin >> mark;
        if (mark < 0 || mark > 100) {
            cout << "Invalid input! Please enter a mark between 0 and 100.\n";
        }
    } while (mark < 0 || mark > 100);
    return mark;
}

void calculateResults(Pelajar &s) {
    int total = 0;
    for (int i = 0; i < 5; ++i) total += s.marks[i];
    s.percentage = total / 5.0;
    s.overallGrade = nakGredStr(s.percentage);
}

void saveToFile(int classIndex) {
    sort(classes[classIndex].begin(), classes[classIndex].end(), [](const Pelajar &a, const Pelajar &b) {
        return a.name < b.name;
    });

    ofstream file(filenames[classIndex]);
    for (const auto &s : classes[classIndex]) {
        file << s.name;
        for (int i = 0; i < 5; ++i)
            file << "," << s.marks[i];
        file << "\n";
    }
    file.close();
}

void loadFromFile(int classIndex) {
    classes[classIndex].clear();
    ifstream file(filenames[classIndex]);
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        Pelajar s;
        getline(ss, s.name, ',');
        for (int i = 0; i < 5; ++i) {
            string mark;
            if (!getline(ss, mark, ',')) break;
            try {
                s.marks[i] = stoi(mark);
            } catch (...) {
                s.marks[i] = 0; // default if invalid
            }
        }
        calculateResults(s);
        classes[classIndex].push_back(s);
    }
    file.close();
}

int selectClass() {
    int choice;
    cout << "----------------------------------\n";
    cout << "Welcome to Student Marking System\n";
    cout << "----------------------------------\n";
    cout << "Select Class:\n1. A\n2. B\n3. C\n4. D\nChoice: ";
    cin >> choice;
    return choice - 1;
}

void menu(int classIndex) {
    while (true) {
        system("CLS");
        cout << "Please select the action (enter the id)\n";
        cout << "1. Register a New Student\n2. Update Student Marks\n3. View Student Marks\n4. Switch Student's Class\n5. Close the Program\nChoice: ";
        int choice;
        cin >> choice;
        cin.ignore();
        system("CLS");

        switch (choice) {
            case 1: {
                Pelajar s;
                cout << "Enter student name: ";
                getline(cin, s.name);
                s.marks[0] = nakMarkah("BM");
                s.marks[1] = nakMarkah("ENG");
                s.marks[2] = nakMarkah("MATH");
                s.marks[3] = nakMarkah("SCI");
                s.marks[4] = nakMarkah("HIS");
                calculateResults(s);
                classes[classIndex].push_back(s);
                saveToFile(classIndex);
                break;
            }
            case 2: {
                if (classes[classIndex].empty()) {
                    cout << "No students in this class.\n";
                    system("pause");
                    break;
                }

                cout << "\nClass " << char('A' + classIndex) << " - List of students:\n";
                for (size_t i = 0; i < classes[classIndex].size(); ++i) {
                    cout << i + 1 << ". " << classes[classIndex][i].name << "\n";
                }

                string input;
                cout << "Enter the number of the student to modify (or 'c' to cancel): ";
                cin >> input;
                if (input == "c" || input == "C") {
                    cout << "Modification canceled.\n";
                    system("pause");
                    break;
                }

                int choice = stoi(input);
                if (choice < 1 || choice > (int)classes[classIndex].size()) {
                    cout << "Invalid choice.\n";
                    system("pause");
                    break;
                }

                Pelajar &s = classes[classIndex][choice - 1];
                cout << "Enter new marks for " << s.name << ":\n";
                s.marks[0] = nakMarkah("BM");
                s.marks[1] = nakMarkah("ENG");
                s.marks[2] = nakMarkah("MATH");
                s.marks[3] = nakMarkah("SCI");
                s.marks[4] = nakMarkah("HIS");
                calculateResults(s);
                saveToFile(classIndex);
                cout << "Marks updated for " << s.name << "!\n";
                system("pause");
                break;
            }
            case 3: {
                if (classes[classIndex].empty()) {
                    cout << "No students to display.\n";
                    system("pause");
                    break;
                }

                vector<Pelajar> ranked = classes[classIndex];
                sort(ranked.begin(), ranked.end(), [](const Pelajar &a, const Pelajar &b) {
                    return a.percentage > b.percentage;
                });

                cout << "\nClass " << char('A' + classIndex) << "\n";
                cout << left << setw(4) << "No" << setw(15) << "Name"
                     << setw(6) << "BM" << setw(4) << "GR"
                     << setw(6) << "ENG" << setw(4) << "GR"
                     << setw(6) << "MATH" << setw(4) << "GR"
                     << setw(6) << "SCI" << setw(4) << "GR"
                     << setw(6) << "HIS" << setw(4) << "GR"
                     << setw(10) << "%" << setw(6) << "GR"
                     << setw(6) << "Rank" << endl;

                for (size_t i = 0; i < ranked.size(); ++i) {
                    const Pelajar &s = ranked[i];
                    cout << left << setw(4) << i + 1 << setw(15) << s.name;
                    for (int j = 0; j < 5; ++j) {
                        cout << setw(6) << s.marks[j] << setw(4) << nakGredStr(s.marks[j]);
                    }
                    cout << setw(10) << s.percentage << setw(6) << s.overallGrade << setw(6) << i + 1 << endl;
                }
                system("pause");
                break;
            }
            case 4:
                return;
            case 5:
                exit(0);
            default:
                cout << "Invalid choice.\n";
        }
    }
}
bool passcheck(int classes) {
    string password[4] = { "bottle", "tumbler", "table", "chair" };
    string enteredPassword;

    cout << "Enter the password: ";
    cin >> enteredPassword;

if (password[classes] == enteredPassword)
    return true;
else
    return false;
}