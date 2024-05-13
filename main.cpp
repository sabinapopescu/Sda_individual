#include <iostream>
#include <fstream>
#include <limits>
#include <string>
#include <cmath>

using namespace std;

enum LoanType {
    BUSINESS,
    PERSONAL,
    MORTGAGE,
    CAR
};

class Loan {
private:
    double loanAmount;
    int loanTermYears;
    int loanTermMonths;
    double interestRate;
    LoanType loanType;
    bool isFixedRate;

public:
    Loan(double amount, int termYears, int termMonths, double rate, LoanType type, bool fixedRate)
            : loanAmount(amount), loanTermYears(termYears), loanTermMonths(termMonths), interestRate(rate),
              loanType(type), isFixedRate(fixedRate) {}

    double getLoanAmount() const {
        return loanAmount;
    }

    int getLoanTermYears() const {
        return loanTermYears;
    }

    int getLoanTermMonths() const {
        return loanTermMonths;
    }

    double getInterestRate() const {
        return interestRate;
    }

    LoanType getLoanType() const {
        return loanType;
    }

    bool isInterestRateFixed() const {
        return isFixedRate;
    }

    double calculateMonthlyPaymentForFixedRate() const {
        int totalMonths = loanTermYears * 12 + loanTermMonths;
        double monthlyInterestRate = interestRate / 12.0 / 100.0;
        double numerator = loanAmount * monthlyInterestRate;
        double denominator = 1.0 - pow(1.0 + monthlyInterestRate, -totalMonths);
        return numerator / denominator;
    }

    double calculateMonthlyPaymentForFlexibleRate(double initialInterestRate) const {
        int totalMonths = loanTermYears * 12 + loanTermMonths;
        double monthlyInterestRate = initialInterestRate / 12.0 / 100.0;
        double numerator = loanAmount * monthlyInterestRate;
        double denominator = 1.0 - pow(1.0 + monthlyInterestRate, -totalMonths);
        return numerator / denominator;
    }

    double calculateTotalPayment() const {
        double monthlyPayment;
        if (isFixedRate) {
            monthlyPayment = calculateMonthlyPaymentForFixedRate();
        } else {
            double initialInterestRate = getInterestRate();
            monthlyPayment = calculateMonthlyPaymentForFlexibleRate(initialInterestRate);
        }

        int totalMonths = loanTermYears * 12 + loanTermMonths;
        return monthlyPayment * totalMonths;
    }
};

// Function to get valid user input for a double value
double getDoubleInput(const string& prompt) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        } else {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number." << endl;
        }
    }
}

// Function to get valid user input for an integer value
int getIntegerInput(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        } else {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter an integer." << endl;
        }
    }
}

// Function to validate loan parameters and create a Loan object
Loan* createLoan() {
    double loanAmount = getDoubleInput("Enter the loan amount: ");
    if (loanAmount <= 0) {
        cout << "Loan amount must be greater than zero." << endl;
        return nullptr;
    }

    int loanTermYears = getIntegerInput("Enter the loan term in years: ");
    int loanTermMonths = getIntegerInput("Enter the loan term in months: ");

    double interestRate = getDoubleInput("Enter the interest rate (%): ");
    if (interestRate < 0) {
        cout << "Interest rate cannot be negative." << endl;
        return nullptr;
    }

    cout << "Select the loan type:" << endl;
    cout << "1. Business" << endl;
    cout << "2. Personal" << endl;
    cout << "3. Mortgage" << endl;
    cout << "4. Car" << endl;
    int loanTypeChoice = getIntegerInput("Enter your choice (1-4): ");

    LoanType loanType;
    switch (loanTypeChoice) {
        case 1:
            loanType = BUSINESS;
            break;
        case 2:
            loanType = PERSONAL;
            break;
        case 3:
            loanType = MORTGAGE;
            break;
        case 4:
            loanType = CAR;
            break;
        default:
            cout << "Invalid loan type choice." << endl;
            return nullptr;
    }

    bool isFixedRate;
    char fixedRateChoice;
    cout << "Is the interest rate fixed? (y/n): ";
    cin >> fixedRateChoice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    isFixedRate = (fixedRateChoice == 'y' || fixedRateChoice == 'Y');

    return new Loan(loanAmount, loanTermYears, loanTermMonths, interestRate, loanType, isFixedRate);
}

int main() {
    Loan* newLoan = createLoan();
    if (newLoan != nullptr) {
        ofstream outputFile("loan_details.txt");
        if (outputFile.is_open()) {
            outputFile << "Loan Amount: $" << newLoan->getLoanAmount() << endl;
            outputFile << "Loan Term: " << newLoan->getLoanTermYears() << " years and " << newLoan->getLoanTermMonths() << " months" << endl;
            outputFile << "Interest Rate: " << newLoan->getInterestRate() << "%" << endl;
            outputFile << "Loan Type: " << (newLoan->getLoanType() == BUSINESS ? "Business" : (newLoan->getLoanType() == PERSONAL ? "Personal" : (newLoan->getLoanType() == MORTGAGE ? "Mortgage" : "Car"))) << endl;
            outputFile << "Interest Rate Type: " << (newLoan->isInterestRateFixed() ? "Fixed" : "Flexible") << endl;
            outputFile << "Monthly Payment: $" << newLoan->calculateMonthlyPaymentForFixedRate() << endl;
            outputFile << "Total Payment: $" << newLoan->calculateTotalPayment() << endl;
            outputFile.close();
            cout << "Loan details have been saved to 'loan_details.txt'" << endl;
        } else {
            cout << "Unable to open the file." << endl;
        }
        delete newLoan;
    }

    return 0;
}