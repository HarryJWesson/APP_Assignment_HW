// This is the header file for util related classes and functionality.
//

#include <vector>

using namespace std;

class loan {
	private:

		int _personID;
		int _resourceID;

	public:
		// constructors
		loan(int p, int r) : _personID(p), _resourceID(r) {}

		// getters
		int getPersonID() { return _personID; };
		int getResourceID() { return _resourceID; }

		// setters
};

class resourcelist {
	private:

		vector<int> _r_list;

	public:
		// constructor
		resourcelist() : _r_list({}) {};

		// misc
		void printResourceList() {};
};

class userlist {
	private:

		vector<int> _u_list;

	public:
		// constructor
		userlist() : _u_list({}) {};

		// misc
		void printUserList() {};
};