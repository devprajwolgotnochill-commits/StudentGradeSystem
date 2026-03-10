// ==========================================================
// Student Grade Management System
// Features:
//  - Add Student
//  - Display Students
//  - Search Student
//  - Update Student
//  - Delete Student
//  - Save to File
//  - Load from File
//  - Show Statistics
//
// Concepts used:
//  - Structures
//  - Arrays
//  - Functions
//  - File Handling
//  - Exception Handling (try / catch)
//  - Input Validation
// ==========================================================

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <limits>

using namespace std;

// Maximum students allowed
const int MAX_STUDENTS = 200;

// Number of subjects per student
const int NUM_SUBJECTS = 6;

// ----------------------------------------------------------
// Student structure
// Stores all data related to one student
// ----------------------------------------------------------
struct Student { //default :public
    int id;                     // unique student ID
    string name;                // student name
    int marks[NUM_SUBJECTS];    // marks for each subject
    int total;                  // total marks
    double average;             // average marks
    char grade;                 // final grade
};

// Global student storage
Student students[MAX_STUDENTS];
int studentCount = 0;


// ----------------------------------------------------------
// Function: clearInput
// Clears invalid input from the input buffer
// Prevents infinite input loops
// ----------------------------------------------------------
void clearInput() {
    cin.clear(); // reset error flags
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // remove bad input
}


// ----------------------------------------------------------
// Function: inputInt
// Safely reads integer input within a specific range
// Keeps asking until valid input is entered
// ----------------------------------------------------------
int inputInt(const string &prompt, int minVal, int maxVal) {

    int v;

    while (true) {

        cout << prompt;

        if (cin >> v) {

            // check if number is inside allowed range
            if (v >= minVal && v <= maxVal)
                return v;

            cout << "Value must be between "
                 << minVal << " and " << maxVal << ". Try again.\n";
        }

        else {
            cout << "Invalid integer input. Try again.\n";
            clearInput();
        }
    }
}


// ----------------------------------------------------------
// Function: inputLineNonEmpty
// Reads a non-empty string (used for student name)
// ----------------------------------------------------------
string inputLineNonEmpty(const string &prompt) {

    string s;

    do {

        cout << prompt;
        getline(cin, s);

        if (s.empty())
            cout << "Input cannot be empty. Try again.\n";

    } while (s.empty());

    return s;
}


// ----------------------------------------------------------
// Function: computeGrade
// Determines grade based on average marks
// ----------------------------------------------------------
char computeGrade(double avg) {

    if (avg >= 90) return 'A';
    if (avg >= 80) return 'B';
    if (avg >= 70) return 'C';
    if (avg >= 60) return 'D';

    return 'F';
}


// ----------------------------------------------------------
// Function: calculateStudentStats
// Calculates total, average, and grade
// ----------------------------------------------------------
void calculateStudentStats(Student &s) {

    s.total = 0;

    for (int i = 0; i < NUM_SUBJECTS; ++i)
        s.total += s.marks[i];

    s.average = static_cast<double>(s.total) / NUM_SUBJECTS;

    s.grade = computeGrade(s.average);
}


// ----------------------------------------------------------
// Function: findStudentIndexById
// Searches student array and returns index
// ----------------------------------------------------------
int findStudentIndexById(int id) {

    for (int i = 0; i < studentCount; ++i)

        if (students[i].id == id)
            return i;

    return -1;
}


// ----------------------------------------------------------
// Function: addStudent
// Adds new student record
// ----------------------------------------------------------
void addStudent() {

    if (studentCount >= MAX_STUDENTS) {
        cout << "Student storage full. Cannot add more.\n";
        return;
    }

    Student s;

    cout << "--- Add New Student ---\n";

    clearInput();

    s.id = inputInt("Enter Student ID (positive integer): ", 1, 1000000);

    // prevent duplicate ID
    if (findStudentIndexById(s.id) != -1) {

        cout << "A student with this ID already exists.\n";
        return;
    }

    s.name = inputLineNonEmpty("Enter Student Name: ");

    // input marks
    for (int i = 0; i < NUM_SUBJECTS; ++i) {

        string prm = "Enter mark for subject "
                     + to_string(i+1) + " (0-100): ";

        s.marks[i] = inputInt(prm, 0, 100);
    }

    calculateStudentStats(s);

    students[studentCount++] = s;

    cout << "Student added successfully.\n";
}


// ----------------------------------------------------------
// Function: displayStudent
// Prints a single student record
// ----------------------------------------------------------
void displayStudent(const Student &s) {

    cout << left << setw(6) << s.id
         << setw(20) << s.name;

    for (int i = 0; i < NUM_SUBJECTS; ++i)
        cout << setw(6) << s.marks[i];

    cout << setw(7) << s.total
         << setw(9) << fixed << setprecision(2) << s.average
         << setw(4) << s.grade
         << '\n';
}


// ----------------------------------------------------------
// Function: displayAllStudents
// Displays all students in table format
// ----------------------------------------------------------
void displayAllStudents() {

    if (studentCount == 0) {
        cout << "No students to display.\n";
        return;
    }

    cout << "ID    Name                ";

    for (int i = 0; i < NUM_SUBJECTS; ++i)
        cout << "Sub" << (i+1) << "  ";

    cout << "Total  Avg     G\n";

    cout << string(70, '-') << '\n';

    for (int i = 0; i < studentCount; ++i)
        displayStudent(students[i]);
}


// ----------------------------------------------------------
// Function: searchStudent
// Finds and displays student by ID
// ----------------------------------------------------------
void searchStudent() {

    int id = inputInt("Enter Student ID to search: ", 1, 1000000);

    int idx = findStudentIndexById(id);

    if (idx == -1) {
        cout << "Student not found.\n";
        return;
    }

    cout << "Student record:\n";

    displayStudent(students[idx]);
}


// ----------------------------------------------------------
// Function: updateStudent
// Allows editing student information
// ----------------------------------------------------------
void updateStudent() {

    int id = inputInt("Enter Student ID to update: ", 1, 1000000);

    int idx = findStudentIndexById(id);

    if (idx == -1) {
        cout << "Student not found.\n";
        return;
    }

    Student &s = students[idx];

    clearInput();

    cout << "Updating student '" << s.name
         << "' (ID=" << s.id << ").\n";

    string name;

    cout << "New name: ";
    getline(cin, name);

    if (!name.empty())
        s.name = name;

    for (int i = 0; i < NUM_SUBJECTS; ++i) {

        cout << "Current mark for subject "
             << i+1 << " is "
             << s.marks[i]
             << ". Enter new (or -1 to keep): ";

        int v;

        if (cin >> v) {

            if (v >= 0 && v <= 100)
                s.marks[i] = v;

            else if (v != -1)
                cout << "Invalid mark ignored.\n";
        }

        else {

            clearInput();
            cout << "Invalid input, leaving mark unchanged.\n";
        }
    }

    calculateStudentStats(s);

    cout << "Student updated.\n";
}


// ----------------------------------------------------------
// Function: deleteStudent
// Removes student from array
// ----------------------------------------------------------
void deleteStudent() {

    int id = inputInt("Enter Student ID to delete: ", 1, 1000000);

    int idx = findStudentIndexById(id);

    if (idx == -1) {
        cout << "Student not found.\n";
        return;
    }

    // shift array left
    for (int i = idx; i < studentCount - 1; ++i)
        students[i] = students[i+1];

    --studentCount;

    cout << "Student deleted.\n";
}


// ----------------------------------------------------------
// Function: saveToFile
// Saves student records to CSV file
// ----------------------------------------------------------
void saveToFile() {

    ofstream ofs("students.csv");

    if (!ofs) {
        cout << "Failed to open file for writing.\n";
        return;
    }

    ofs << "id,name";

    for (int i = 0; i < NUM_SUBJECTS; ++i)
        ofs << ",m" << (i+1);

    ofs << ",total,average,grade\n";

    for (int i = 0; i < studentCount; ++i) {

        ofs << students[i].id << "," << students[i].name;

        for (int j = 0; j < NUM_SUBJECTS; ++j)
            ofs << "," << students[i].marks[j];

        ofs << "," << students[i].total
            << "," << students[i].average
            << "," << students[i].grade
            << "\n";
    }

    ofs.close();

    cout << "Saved " << studentCount
         << " records to students.csv\n";
    
    
}


// ----------------------------------------------------------
// Function: loadFromFile
// Loads student records from CSV
// Includes try/catch for safe conversion
// ----------------------------------------------------------
void loadFromFile() {

    ifstream ifs("students.csv");

    if (!ifs) {
        cout << "No students.csv file found; skipping load.\n";
        return;
    }

    string line;

    getline(ifs, line); // skip header

    studentCount = 0;

    while (getline(ifs, line) && studentCount < MAX_STUDENTS) {

        Student s;

        size_t pos = 0, prev = 0;
        int col = 0;

        string parts[20];

        // simple CSV split
        while ((pos = line.find(',', prev)) != string::npos) {

            parts[col++] = line.substr(prev, pos - prev);
            prev = pos + 1;
        }

        parts[col++] = line.substr(prev);

        try {

            s.id = stoi(parts[0]);
            s.name = parts[1];

            for (int i = 0; i < NUM_SUBJECTS; ++i)
                s.marks[i] = stoi(parts[2+i]);

        }

        catch (...) {

            cout << "Error reading file line. Skipping record.\n";
            continue;
        }

        calculateStudentStats(s);

        students[studentCount++] = s;
    }

    ifs.close();

    cout << "Loaded "
         << studentCount
         << " records from students.csv\n";
}


// ----------------------------------------------------------
// Function: showStatistics
// Shows overall class statistics
// ----------------------------------------------------------
void showStatistics() {

    if (studentCount == 0) {
        cout << "No data for statistics.\n";
        return;
    }

    double sumAvg = 0;

    int gradeCount[5] = {0};

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

    cout << "Students: "
         << studentCount
         << "   Average of averages: "
         << (sumAvg / studentCount)
         << "\n";

    cout << "Grade distribution: "
         << "A=" << gradeCount[0]
         << " B=" << gradeCount[1]
         << " C=" << gradeCount[2]
         << " D=" << gradeCount[3]
         << " F=" << gradeCount[4]
         << "\n";
}


// ----------------------------------------------------------
// Displays program menu
// ----------------------------------------------------------
void showMenu() {

    cout << "\n=== Student Grade Management System ===\n";

    cout << "1. Add Student\n";
    cout << "2. Display All Students\n";
    cout << "3. Search Student by ID\n";
    cout << "4. Update Student\n";
    cout << "5. Delete Student\n";
    cout << "6. Save to File and Exit\n";
    cout << "7. Load from File\n";
    cout << "8. Statistics\n";
    cout << "9. Exit\n";
}


// ----------------------------------------------------------
// MAIN PROGRAM
// ----------------------------------------------------------
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

            case 6: saveToFile();
                    cout << "Data saved. Exiting program...\n";
                    return 0;


            case 7: loadFromFile(); break;

            case 8: showStatistics(); break;

            case 9: saveToFile(); break;
                cout << "Exiting. Goodbye!\n";
                return 0;

            default:
                cout << "Invalid choice.\n";
        }
    }

    return 0;
}
