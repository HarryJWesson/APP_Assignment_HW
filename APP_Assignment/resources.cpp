// This is the header file for resource related classes (books, journals, conferences.)
//

#include <string>
#include <vector>

using namespace std;

class resource {
	private:

		int _ID;
		bool _canLend{ true };
		bool _isBorrowed{false};

		string title;

	public:

		// constructors
		resource() = default;
		virtual ~resource() = default;
		resource(string t, int i) : title(t), _ID(i) {}

		// getters
		int getID() { return _ID; }
		string getTitle() { return title; }
		bool checkIfBorrowed() { return _isBorrowed; }
		virtual bool checkIfCanLend() { return _canLend; }

		// setters
		void setTitle(string t) { title = t; }
		void setBorrowed(bool b) { _isBorrowed = b; }
		void setLended(bool l) { _canLend = l; }

		// misc
		virtual string asString() { return title; };
};

class book : public resource {
	private:

		string _author;
		int _year;

	public:

		// constructors
		book() = default;
		book(string author, string title, int year, int id) : resource(title, id), _author(author), _year(year) {}

		// getters
		string getAuthor() { return _author; }

		// setters
		void setAuthor(string a) { _author = a; }

		// misc
		string asString() override { return _author; }
};

class journal : public resource {
	private:

		vector<string> editions;

	public:

		// constructors
		journal() = default;
		journal(string title, int id, vector<string> ed) : resource(title, id), editions(ed) {}

		// getters
		vector<string>& getEditions() { return editions; }

		// setters
		void addEdition(string e) { editions.push_back(e); }
};

class conference : public resource {
	private:

		string _acronym;
		vector<string> years;

	public:

		// constructors
		conference() = default;
		conference(string title, string acronym, int id, vector<string> y) : resource(title, id), _acronym(acronym), years(y) {};

		// getters
		vector<string>& getYears() { return years; }
		string getAcronym() { return _acronym; }
		bool checkIfCanLend() { return false; }

		// setters
		void addYear(string y) { years.push_back(y); }
};