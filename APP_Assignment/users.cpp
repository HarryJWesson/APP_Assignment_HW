// This is the header file for the classes related to user activity. (student, staff, libstaff)
//

#include <string>

using namespace std;

class person {
	private:

		int _ID;
		int borrowLimit;
		string _name; 

	public:
		// constructors
		person() = default;
		virtual ~person() = default;
		person(string n, int i, int b) : _name(n), _ID(i), borrowLimit(b) {}

		// getters
		int getID() { return(_ID); }
		string getName() { return(_name); }
		int getBorrowCount() { return(borrowLimit); }

		// setters
		void decBorrow() { borrowLimit--; }
		void incBorrow() { borrowLimit++; }

		// misc

};

class student : public person {
	private:
		;
	public:
		// constructors
		student() = default;
		student(string n, int i) : person(n, i, 1) {};
};

class staff : public person {
	private:
		;
	public:
		// constructors
		staff() = default;
		staff(string n, int i) : person(n, i, 2) {};;
};

class libstaff : public person {
	private:
		;
	public:
		// constructors
		libstaff() = default;
		libstaff(string n, int i) : person(n, i, 0) {};;
};