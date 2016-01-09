#include <iostream>
#include <strstream>
#include <cstdio>
#include <map>
#include <list>
#include <set>
#include <vector>
#include <string>
#include <cstring>
#include <unistd.h>
using namespace std;

int sleep_time = 10000;

class turing_machine {
private:
	struct transition {
		int state;
		char symbol;
		short move;
	};
	list<char> tape;
	list<char>::iterator pointer;
	vector< map<char, transition> > delta;
	vector<bool> is_final;
	int current_state;
	int N;
public:
	void read(istream &is) {
		is >> N;
		delta.resize(N);
		string buff;
		for (int i = 0; i < N; ++i) {
			int S;
			is >> S;
			is_final.push_back(S == -1);
			for (int j = 0; j < S; ++j) {
				char input_symbol;
				transition T;

				is >> buff; input_symbol = buff[0];
				is >> T.state;
				is >> buff; T.symbol = buff[0];
				is >> buff;
				T.move = (buff[0] == 'L' ? -1 : (buff[0] == 'R' ? 1 : 0));
				delta[i][input_symbol] = T;
			}
		}
		is >> buff;
		int len = buff.length();
		for (int i = 0; i < len; ++i) {
			tape.push_back(buff[i]);
		}
		is >> current_state;
		int pointer_pos;
		is >> pointer_pos;
		pointer = tape.begin();
		for (int i = 0; i < pointer_pos; ++i) {
			++pointer;
		}
		tape.push_front('_');
		tape.push_back('_');
	}
	void print_current_state(ostream &os) {
		system("clear");
		for (list<char>::iterator it = tape.begin(); it != tape.end(); ++it) {
			if (*it != '_') {
				os << *it;
			} else {
				os << ' ';
			}
		}
		os << endl;
		for (list<char>::iterator it = tape.begin(); it != tape.end(); ++it) {
			if (it == pointer) {
				os << '^';
				break;
			} else {
				os << ' ';
			}
		}
		os << endl;
		for (list<char>::iterator it = tape.begin(); it != tape.end(); ++it) {
			if (it == pointer) {
				os << "q:" << current_state;
				break;
			} else {
				os << ' ';
			}
		}
		os << endl;
		usleep(sleep_time);
	}
	bool run_one_step() {
		char input_symbol = *pointer;
		if (*pointer == '_') {
			if (pointer == tape.begin()) {
				tape.push_front('_');
			} else {
				tape.push_back('_');
			}
		}
		if (delta[current_state].find(input_symbol) == delta[current_state].end()) {
			return false;
		}
		transition T = delta[current_state][input_symbol];
		current_state = T.state;
		*pointer = T.symbol;
		if (T.move == -1) {
			--pointer;
		} else if (T.move == 1) {
			++pointer;
		}
		return true;
	}
	void run(ostream &os) {
		print_current_state(os);
		bool result = true;
		while ((result = run_one_step())) {
			if (is_final[current_state]) {
				break;
			}
			print_current_state(os);
		}
		print_current_state(os);
		os << (result ? "Accept" : "Reject") << endl;
	}
};

int string_to_int(char *str) {
	strstream ss;
	ss << str;
	int ret = 0;
	ss >> ret;
	return ret;
}

int main(int argc, char *argv[]) {
	if (argc > 1) {
		sleep_time = string_to_int(argv[1]);
	}
	turing_machine M;
	M.read(cin);
	M.run(cout);
	return 0;
}