#include <list>

using namespace std;

class Errorton {
	list<string> usageErrors;
	list<string> houseErrors;
	list<string> algorithmErrors;
	list<string> generalErrors;
	int reportsCounter = 0;
	
public :

	void reportError(char c, const string& s);
	int reports() const { return reportsCounter; }
	static Errorton& getInstance() { return instance; }
	~Errorton();

private :
	static Errorton instance;
};