//============================================================================
// Name        : CoursePlanner.cpp
// Author      : Aaron Ciminelli
// Version     : 1.0
// Copyright   : Copyright © 2022 SNHU CS-300-T6609 DSA: Analysis and Design 22EW6
// Description : Project 2 in C++, reads CSV File and Prints Data
//============================================================================

#include <iostream>
#include <fstream>
#include <Vector>
#include "CSVparser.hpp"
using namespace std;
using namespace std;
// AC** Creating Global Definitions 
struct Courses {
    string courseNum;
    string courseName;
    vector<string> coursePrereqs;
    Courses() {}
};
struct Node { // AC** internal structor for node 
    Courses course;
    Node* head;
    Node* tail;
    Node() {
        head = nullptr;
        tail = nullptr;
    }
    Node(Courses iCourse) : Node() { //AC** intitalize course 
        this->course = iCourse;
    }
};
class CourseBinarySearchTree { // AC** implementing Binary Serach stree data methods 
private:
    Node* root;
    void addNode(Node* node, Courses course);
    void printCourseList(Node* node);
    void printCourseName(Node* node, string courseNum);
public:
    CourseBinarySearchTree();
    virtual ~CourseBinarySearchTree();
    void rNode(Node* node);
    void Insert(Courses course);
    int nCoursesPrereq(Courses course);
    void PrintCourseList();
    void PrintCourseName(string courseNum);
};
CourseBinarySearchTree::CourseBinarySearchTree() {
    root = nullptr; // AC** setting root equal to nullptr
}
void CourseBinarySearchTree::Insert(Courses course) { // AC* inserting nde in to Couse Binary Search Tree 
    if (root == nullptr)
        root = new Node(course);
    else
        this->addNode(root, course);
}
CourseBinarySearchTree::~CourseBinarySearchTree() {
    rNode(root);//AC** recurse from root deleteing every node
}
void CourseBinarySearchTree::rNode(Node* node) {
    if (node) {
        rNode(node->head);
        rNode(node->tail);
        delete node;
    }
}
void CourseBinarySearchTree::addNode(Node* node, Courses course) {// implementing logic to add course
    if (node->course.courseNum.compare(course.courseNum) > 0) {
        if (node->head == nullptr)
            node->head = new Node(course);
        else
            this->addNode(node->head, course);
    }
    else {
        if (node->tail == nullptr)
            node->tail = new Node(course);
        else
            this->addNode(node->tail, course);
    }
}
int CourseBinarySearchTree::nCoursesPrereq(Courses course) { //AC** Searing for courses with PreReqs and implement logic to print prereq
    int count = 0;
    for (unsigned int i = 0; i < course.coursePrereqs.size(); i++) {
        if (course.coursePrereqs.at(i).length() > 0)
            count++;
    }
    return count;
}

void CourseBinarySearchTree::PrintCourseName(string courseNum) { //AC** implementing logic to print course name 
    this->printCourseName(root, courseNum);
}

void CourseBinarySearchTree::PrintCourseList() {// AC** implementing logic to print course list 
    this->printCourseList(root);
}
void CourseBinarySearchTree::printCourseList(Node* node) {
    if (node != nullptr) {
        printCourseList(node->head);
        cout << node->course.courseNum << ", " << node->course.courseName << endl;
        printCourseList(node->tail);
    }
    return;
}
void CourseBinarySearchTree::printCourseName(Node* curr, string courseNum) { // AC** implementing logic to print course list and prereqs 
    while (curr != nullptr) {
        if (curr->course.courseNum.compare(courseNum) == 0) {
            cout << endl << curr->course.courseNum << ", " << curr -> course.courseName << endl;
            unsigned int size = nCoursesPrereq(curr->course);
            cout << "Prerequisite(s): ";
            unsigned int i = 0;
            for (i = 0; i < size; i++) {
                cout << curr->course.coursePrereqs.at(i);
                if (i != size - 1)
                    cout << ", ";
            }
            if (i == 0)
                cout << "No prerequisites required.";
            cout << endl;
            return;
        }
        else if (courseNum.compare(curr->course.courseNum) < 0)
            curr = curr->head;
        else
            curr = curr->tail;
    }
        cout << "Course " << courseNum << " not found." << endl;
}
bool loadCourses(string csvPath, CourseBinarySearchTree* coursesBST) { // AC** Load a CSV file containing courseses into a container
    
    try {
        ifstream courseFile(csvPath);
        if (courseFile.is_open()) {
            while (!courseFile.eof()) {
                vector<string> courseInfo;
                string courseData;
                getline(courseFile, courseData);
                while (courseData.length() > 0) {
                    unsigned int key = courseData.find(',');
                    if (key < 100) {
                        courseInfo.push_back(courseData.substr(0, key));
                        courseData.erase(0, key + 1);
                    }
                    else {
                        courseInfo.push_back(courseData.substr(0,
                            courseData.length()));
                        courseData = "";
                    }
                }
                Courses course; // AC** Create a data structure and add to the collection of Courses
                course.courseNum = courseInfo[0];
                course.courseName = courseInfo[1];
                for (unsigned int i = 2; i < courseInfo.size(); i++) {
                    course.coursePrereqs.push_back(courseInfo[i]);
                }
                coursesBST->Insert(course); //AC** pushing course to end 
            }
            courseFile.close();
            return true;
        }
    }
    catch (csv::Error& e) {
        cerr << e.what() << endl;
    }
    return false;
}
int main(int argc, char* argv[]) {// AC** creating main method switch case 
    string csvPath, courseId;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        break;
    case 3:
        csvPath = argv[1];
        courseId = argv[2];
        break;
    default:
        csvPath = "ABCU__Input.csv";
        break;
    }
    CourseBinarySearchTree* coursesBST = nullptr; // creating menu
    cout << "\nWelcome to the course planner!\n" << endl;
    string choice = "0";
    int userChoice = choice[0] - '0';
    while (userChoice != 4) {
        cout << " 1. Load Data Structure" << endl;
        cout << " 2. Print Course List" << endl;
        cout << " 3. Print Course" << endl;
        cout << " 4. Exit" << endl;
        cout << "\nWhat would you like to do? ";
        cin >> choice;
        if (choice.length() == 1)
            userChoice = choice[0] - '0';
        else
            userChoice = 0;
        bool success = 1;
        switch (userChoice) {
        case 1:
            if (coursesBST == nullptr)
                coursesBST = new CourseBinarySearchTree();
            if (csvPath.length() == 0) {
                cout << "Enter the file name that contains the course data: ";
                cin >> csvPath;
            }
            success = loadCourses(csvPath, coursesBST);
            if (success)
                cout << "Courses have been loaded.\n" << endl;
            else
                cout << "File not found.\n" << endl;
            csvPath = "";
            break;
        case 2:
            if (coursesBST != nullptr && success) {
                cout << "Here is a sample schedule:\n" << endl;
                coursesBST->PrintCourseList();
                cout << endl;
            }
            else
                cout << "Load courses first - option 1\n" << endl;
            break;
        case 3:
            if (coursesBST != nullptr && success) {
                if (courseId.length() == 0) {
                    cout << "What course do you want to know about? ";
                    cin >> courseId;
                    for (auto& userChoice : courseId) userChoice =toupper(userChoice);
                }
                coursesBST->PrintCourseName(courseId);
                cout << endl;
            }
            else
                cout << "Error! Please Load courses first - press option 1\n" << endl;
            courseId = "";
            break;
        default: 
            if (userChoice != 4)
                cout << choice << " is not a valid option\n" << endl;
            break;
        }
    }
    cout << "\nThank you for using the course planner!" << endl;
    return 0;
}




