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
};

class resourceList {
	private:

		vector<int> _r_list;

	public:
		// constructor
		resourceList() : _r_list({}) {};

		// getter
		vector<int> getList() { return _r_list; }

		// setter
		void addItem(int item) { _r_list.push_back(item); }
		void removeItem(int item) {
			int pos0{ 0 };
			int pos1{ 0 };

			for (int i : _r_list) {
				if (i == item) { pos1 = pos0; }
				pos0++;
			}

			_r_list.erase(_r_list.begin() + pos1);
		}
};

class userList {
	private:

		vector<int> _u_list;

	public:
		// constructor
		userList() : _u_list({}) {};

		// getter
		vector<int> getList() { return _u_list; }

		// setter
		void addItem(int item) { _u_list.push_back(item); }
		void removeItem(int item) {
			int pos0{ 0 };
			int pos1{ 0 };

			for (int i : _u_list) {
				if (i == item) { pos1 = pos0; }
				pos0++;
			}

			_u_list.erase(_u_list.begin() + pos1);
		}
};