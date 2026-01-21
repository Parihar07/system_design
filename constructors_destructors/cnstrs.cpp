#include<iostream>
#include<string>
#include<cstring>
using namespace std;

class Student{
    int age;
    char *ptrName;
    public:
        // Default Constructor - OPTION 1: Initialize with nullptr
        Student() {
            age = 0;
            ptrName = nullptr;  // Safe - means "no string yet"
            cout << "✓ Default Constructor called ONLY" << endl;
        }
        
        // Default Constructor - OPTION 2: Allocate memory with default string
        // Uncomment this and comment above if you want pre-allocated memory
        /*
        Student() {
            age = 0;
            ptrName = new char[8];  // Allocate memory
            strcpy(ptrName, "Unknown");  // Copy default string
            cout << "Default Constructor called (with memory)" << endl;
        }
        */
        
        // Parameterized Constructor
        Student(int a, string str)
        {
            age = a;
            ptrName = new char[str.length() + 1];  // +1 for null terminator
            strcpy(ptrName, str.c_str());
            cout << "✓ Parameterized Constructor called ONLY (NOT default first!)" << endl;
        }
        
        // Copy Constructor (DEEP COPY) - MUST HAVE!
        Student(const Student &s) {
            age = s.age;
            if(s.ptrName != nullptr) {
                ptrName = new char[strlen(s.ptrName) + 1];  // New memory
                strcpy(ptrName, s.ptrName);
                cout << "Copy Constructor called (Deep Copy)" << endl;
            } else {
                ptrName = nullptr;
            }
        }
        
        // Copy Assignment Operator - MUST HAVE!
        Student& operator=(const Student &s) {
            cout << "Copy Assignment Operator called" << endl;
            
            if(this == &s) {  // Self-assignment check
                return *this;
            }
            
            // Delete old memory
            if(ptrName != nullptr) {
                delete[] ptrName;
            }
            
            // Copy values
            age = s.age;
            if(s.ptrName != nullptr) {
                ptrName = new char[strlen(s.ptrName) + 1];
                strcpy(ptrName, s.ptrName);
            } else {
                ptrName = nullptr;
            }
            
            return *this;
        }
        
        // Destructor - MUST HAVE!
        ~Student() {
            if(ptrName != nullptr) {
                cout << "Destructor called for " << ptrName << endl;
                delete[] ptrName;
            }
        }
        
        // Method to set name after object creation
        void setName(string str) {
            // Delete old memory if exists
            if(ptrName != nullptr) {
                delete[] ptrName;
            }
            
            // Allocate new memory and copy string
            ptrName = new char[str.length() + 1];
            strcpy(ptrName, str.c_str());
            cout << "Name set to: " << ptrName << endl;
        }
        
    void display(){
        cout << "age : " << age << endl;
        if(ptrName != nullptr) {
            cout << "ptrName : " << ptrName << " (at address: " << (void*)ptrName << ")" << endl;
        } else {
            cout << "ptrName : nullptr (no name set)" << endl;
        }
    }
};

int main()
{
    cout << "=== TEST: Which constructor is called? ===" << endl;
    
    cout << "\n1. Creating s1 with NO parameters:" << endl;
    Student s1;  // Watch output - ONLY default constructor
    
    cout << "\n2. Creating s2 WITH parameters:" << endl;
    Student s2(23, "munna");  // Watch output - ONLY parameterized constructor
    
    cout << "\n3. Creating s3 with copy:" << endl;
    Student s3 = s2;  // Watch output - ONLY copy constructor
    
    cout << "\n=== PROOF: Only ONE constructor called each time! ===" << endl;
    
    cout << "\n=== Setting name for s1 using setName() ===" << endl;
    s1.setName("Ravi");  // This is NOT a constructor, just a method
    
    cout << "\n=== Displaying all objects ===" << endl;
    cout << "s1: "; s1.display();
    cout << "s2: "; s2.display();
    cout << "s3: "; s3.display();
    
    cout << "\n=== Destructors will be called now ===" << endl;
    return 0;
}
