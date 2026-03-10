// Student Grade Management System
// Features: add/view/search/update/delete students, compute grades, save/load file
// Uses: control structures, functions, arrays, file handling, input validation

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <limits>

using namespace std;

const int MAX_STUDENTS = 200;
const int NUM_SUBJECTS = 6;

struct Student { //public by default
    int id;
    string name;
    int marks[NUM_SUBJECTS];
    int total;
    double average;
    char grade;
};

Student students[MAX_STUDENTS];
int studentCount = 0;

// Utility: clear input when bad
void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int inputInt(const string &prompt, int minVal, int maxVal) {
    int v;
    while (true) {
        cout << prompt;
        if (cin >> v) {
            if (v >= minVal && v <= maxVal) return v;
            cout << "Value must be between " << minVal << " and " << maxVal << ". Try again.\n";
        } else {
            cout << "Invalid integer input. Try again.\n";
            clearInput();
        }
    }
}

string inputLineNonEmpty(const string &prompt) {
    string s;
    do {
        cout << prompt;
        getline(cin, s);
        if (s.empty()) cout << "Input cannot be empty. Try again.\n";
    } while (s.empty());
    return s;
}

char computeGrade(double avg) {
    if (avg >= 90) return 'A';
    if (avg >= 80) return 'B';
    if (avg >= 70) return 'C';
    if (avg >= 60) return 'D';
    return 'F';
}

void calculateStudentStats(Student &s) {
    s.total = 0;
    for (int i = 0; i < NUM_SUBJECTS; ++i) s.total += s.marks[i];
    s.average = static_cast<double>(s.total) / NUM_SUBJECTS;
    s.grade = computeGrade(s.average);
}

int findStudentIndexById(int id) {
    for (int i = 0; i < studentCount; ++i) if (students[i].id == id) return i;
    return -1;
}

void addStudent() {
    if (studentCount >= MAX_STUDENTS) {
        cout << "Student storage full. Cannot add more.\n";
        return;
    }
    Student s;
    cout << "--- Add New Student ---\n";
    // Ensure any trailing newline removed
    clearInput();
    s.id = inputInt("Enter Student ID (positive integer): ", 1, 100);
    if (findStudentIndexById(s.id) != -1) {
        cout << "A student with this ID already exists. Aborting add.\n";
        return;
    }
    s.name = inputLineNonEmpty("Enter Student Name: ");
    for (int i = 0; i < NUM_SUBJECTS; ++i) {
        string prm = "Enter mark for subject " + to_string(i+1) + " (0-100): ";
        s.marks[i] = inputInt(prm, 0, 100);
    }
    calculateStudentStats(s);
    students[studentCount++] = s;
    cout << "Student added successfully.\n";
}

void displayStudent(const Student &s) {
    cout << left << setw(6) << s.id << setw(20) << s.name;
    for (int i = 0; i < NUM_SUBJECTS; ++i) cout << setw(6) << s.marks[i];
    cout << setw(7) << s.total << setw(9) << fixed << setprecision(2) << s.average << setw(4) << s.grade << '\n';
}

void displayAllStudents() {
    if (studentCount == 0) { cout << "No students to display.\n"; return; }
    cout << "ID    Name                ";
    for (int i = 0; i < NUM_SUBJECTS; ++i) cout << "Sub" << (i+1) << "  ";
    cout << "Total  Avg     G\n";
    cout << string(70, '-') << '\n';
    for (int i = 0; i < studentCount; ++i) displayStudent(students[i]);
}

void searchStudent() {
    int id = inputInt("Enter Student ID to search: ", 1, 1000000);
    int idx = findStudentIndexById(id);
    if (idx == -1) { cout << "Student not found.\n"; return; }
    cout << "Student record:\n";
    displayStudent(students[idx]);
}

void updateStudent() {
    int id = inputInt("Enter Student ID to update: ", 1, 1000000);
    int idx = findStudentIndexById(id);
    if (idx == -1) { cout << "Student not found.\n"; return; }
    Student &s = students[idx];
    clearInput();
    cout << "Updating student '" << s.name << "' (ID=" << s.id << "). Leave blank to keep current.\n";
    string name;
    cout << "New name: ";
    getline(cin, name);
    if (!name.empty()) s.name = name;
    for (int i = 0; i < NUM_SUBJECTS; ++i) {
        cout << "Current mark for subject " << i+1 << " is " << s.marks[i] << ". Enter new (or -1 to keep): ";
        int v; if (cin >> v) {
            if (v >= 0 && v <= 100) s.marks[i] = v;
            else if (v != -1) cout << "Invalid mark ignored.\n";
        } else { clearInput(); cout << "Invalid input, leaving mark unchanged.\n"; }
    }
    calculateStudentStats(s);
    cout << "Student updated.\n";
}

void deleteStudent() {
    int id = inputInt("Enter Student ID to delete: ", 1, 1000000);
    int idx = findStudentIndexById(id);
    if (idx == -1) { cout << "Student not found.\n"; return; }
    for (int i = idx; i < studentCount - 1; ++i) students[i] = students[i+1];
    --studentCount;
    cout << "Student deleted.\n";
}

void saveToFile() {
    ofstream ofs("students.csv");
    if (!ofs) { cout << "Failed to open file for writing.\n"; return; }
    ofs << "id,name";
    for (int i = 0; i < NUM_SUBJECTS; ++i) ofs << ",m" << (i+1);
    ofs << ",total,average,grade\n";
    for (int i = 0; i < studentCount; ++i) {
        ofs << students[i].id << "," << students[i].name;
        for (int j = 0; j < NUM_SUBJECTS; ++j) ofs << "," << students[i].marks[j];
        ofs << "," << students[i].total << "," << students[i].average << "," << students[i].grade << "\n";
    }
    ofs.close();
    cout << "Saved " << studentCount << " records to students.csv\n";
}

void loadFromFile() {
    ifstream ifs("students.csv");
    if (!ifs) { cout << "No students.csv file found; skipping load.\n"; return; }
    string line;
    getline(ifs, line); // header
    studentCount = 0;
    while (getline(ifs, line) && studentCount < MAX_STUDENTS) {
        // simple CSV parse (commas not allowed in names)
        Student s;
        size_t pos = 0, prev = 0; int col = 0;
        int values[NUM_SUBJECTS+3]; // id + marks + total + avg*100 + grade not stored here
        string parts[20];
        while ((pos = line.find(',', prev)) != string::npos) {
            parts[col++] = line.substr(prev, pos - prev);
            prev = pos + 1;
        }
        parts[col++] = line.substr(prev);
        if (col < 2 + NUM_SUBJECTS) continue; // malformed
        s.id = stoi(parts[0]);
        s.name = parts[1];
        for (int i = 0; i < NUM_SUBJECTS; ++i) s.marks[i] = stoi(parts[2+i]);
        calculateStudentStats(s);
        students[studentCount++] = s;
    }
    ifs.close();
    cout << "Loaded " << studentCount << " records from students.csv\n";
}

void showStatistics() {
    if (studentCount == 0) { cout << "No data for statistics.\n"; return; }
    double sumAvg = 0;
    int gradeCount[5] = {0}; // A,B,C,D,F
    for (int i = 0; i < studentCount; ++i) {
        sumAvg += students[i].average;
        switch (students[i].grade) {
            case 'A': gradeCount[0]++; break;
            case 'B': gradeCount[1]++; break;
            case 'C': gradeCount[2]++; break;
            case 'D': gradeCount[3]++; break;
            default: gradeCount[4]++; break;
        }
    }
    cout << "Students: " << studentCount << "   Average of averages: " << (sumAvg / studentCount) << "\n";
    cout << "Grade distribution: A=" << gradeCount[0] << " B=" << gradeCount[1] << " C=" << gradeCount[2] << " D=" << gradeCount[3] << " F=" << gradeCount[4] << "\n";
}

void showMenu() {
    cout << "\n=== Student Grade Management System ===\n";
    cout << "1. Add Student\n";
    cout << "2. Display All Students\n";
    cout << "3. Search Student by ID\n";
    cout << "4. Update Student\n";
    cout << "5. Delete Student\n";
    cout << "6. Save to File\n";
    cout << "7. Load from File\n";
    cout << "8. Statistics\n";
    cout << "9. Exit\n";
}

int main() {
    cout << "Student Grade Management System\n";
    loadFromFile();
    while (true) {
        showMenu();
        int choice = inputInt("Choose an option (1-9): ", 1, 9);
        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayAllStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: saveToFile(); break;
            case 7: loadFromFile(); break;
            case 8: showStatistics(); break;
            case 9: cout << "Exiting. Goodbye!\n"; return 0;
            default: cout << "Invalid choice.\n"; break;
        }
    }
    return 0;
}
