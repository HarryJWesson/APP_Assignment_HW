// This is the header file for resource related classes (books, journals, conferences.)
//

#include <string>

class resource {
	private:
		bool _canLend;
		int _ID;
		bool _isBorrowed;
		std::string title;
	public:
		std::string asString() {};
};

class book : resource {
	private:
		std::string _author;
	public:
		;
};

class journal : resource {
	private:
		;
	public:
		;
};

class conference : resource {
	private:
		std::string _acronym;
	public:
		;
};