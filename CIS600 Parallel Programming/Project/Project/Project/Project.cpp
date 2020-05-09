#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <mutex>
#include <set>
#include <vector>
#include <queue>
#include <algorithm>

#include "json.h"

using namespace std;

bool running = true;
mutex mtxOut;
mutex mtxCout;
ofstream out("data.txt", ios::out | ios::trunc);

chrono::time_point<chrono::system_clock> start;

// Do not uncomment this if you want to use GUI tool.
// #define OUTPUT_COMMANDS
#define OUTPUT_SAMPLE_INTERNAL 500ms
#define CTRL_INTERNAL 500ms
#define IDLE_COST 1000ms
#define MOVE_COST 3000ms
#define STOP_COST 5000ms

int cart_number = 3;
int max_level = 9;

enum CartState {
	idle = 0,
	idle_open = 1,
	up = 2,
	up_open = 3,
	down = 4,
	down_open = 5,
};

class Cart {
public:
	int pos = 1;
	CartState state = CartState::idle;
	set<int> upLevels;
	set<int> downLevels;
	set<int> stopLevels;

	void addLevel(int level) {
		if ((state == CartState::up || state == CartState::up_open) && pos < level || 
			(state == CartState::down || state == CartState::down_open) && pos > level)
			stopLevels.insert(level);
	}

	void addLevel(int level, bool up) {
		up ? upLevels.insert(level) : downLevels.insert(level);
	}

	int getResponseTime(int level, bool up) {
		switch (state)
		{
		case CartState::idle:
		case CartState::idle_open:
			return abs(pos - level);
		case CartState::up:
		case CartState::up_open: {
			if (up) {
				if (pos < level) {
					return abs(pos - level);
				}
				else {
					int highest = max(upLevels.empty() ? pos : *(--upLevels.end()), downLevels.empty() ? pos : *(--downLevels.end()));
					highest = max(highest, stopLevels.empty() ? pos : *(--stopLevels.end()));
					int lowest = min(upLevels.empty() ? pos : *(upLevels.begin()), downLevels.empty() ? pos : *(downLevels.begin()));
					return abs(pos - highest) + abs(highest - lowest) + abs(lowest - level);
				}
			}
			else {
				int highest = max(upLevels.empty() ? pos : *(--upLevels.end()), downLevels.empty() ? pos : *(--downLevels.end()));
				highest = max(highest, stopLevels.empty() ? pos : *(--stopLevels.end()));
				return abs(pos - highest) + abs(highest - level);
			}
		}
		case CartState::down:
		case CartState::down_open: {
			if (!up) {
				if (pos > level) {
					return abs(pos - level);
				}
				else {
					int highest = max(upLevels.empty() ? pos : *(--upLevels.end()), downLevels.empty() ? pos : *(--downLevels.end()));
					int lowest = min(upLevels.empty() ? pos : *(upLevels.begin()), downLevels.empty() ? pos : *(downLevels.begin()));
					lowest = min(lowest, stopLevels.empty() ? pos : *(stopLevels.begin()));
					return abs(pos - lowest) + abs(lowest - highest) + abs(highest - level);
				}
			}
			else {
				int lowest = min(upLevels.empty() ? pos : *(upLevels.begin()), downLevels.empty() ? pos : *(downLevels.begin()));
				lowest = min(lowest, stopLevels.empty() ? pos : *(stopLevels.begin()));
				return abs(pos - lowest) + abs(lowest - level);
			}
		}
		}
	}
};

class Elevator {
public:
	mutex mtx;
	vector<Cart> carts;
	queue<int> upQueue, downQueue;

	Elevator() : carts(vector<Cart>(cart_number)) {}
};

void input(Elevator& sys) {
	char mode;
	int para1, para2;

	while (running) {
		cin >> mode;
		if (mode == 'c') {
			cin >> para1;
			cin >> para2;

			if (para1 <= 0 || para1 > cart_number)
				return;
			if (para2 <= 0 || para2 > max_level)
				return;

			--para1;

			sys.carts[para1].addLevel(para2);

#ifdef OUTPUT_COMMANDS
			unique_lock<mutex> lg(mtxOut);
			out << "{\"type\":\"goto\",\"cart\":" << para1 << ",\"des\":" << para2 << "}" << endl;
#endif
		}
		else if (mode == 'd') {
			cin >> para1;

			if (para1 <= 0 || para1 > max_level)
				return;

			sys.downQueue.push(para1);

#ifdef OUTPUT_COMMANDS
			unique_lock<mutex> lg(mtxOut);
			out << "{\"type\":\"new\",\"dir\":\"down\",\"des\":" << para1 << "}" << endl;
#endif
		}
		else if (mode == 'u') {
			cin >> para1;
			sys.upQueue.push(para1);

			if (para1 <= 0 || para1 > max_level)
				return;

#ifdef OUTPUT_COMMANDS
			unique_lock<mutex> lg(mtxOut);
			out << "{\"type\":\"new\",\"dir\":\"up\",\"des\":" << para1 << "}" << endl;
#endif
		}
		else if (mode == 'x') {
			running = false;
		}
	}
}

void output(Elevator& sys) {
	while (running) {
		{
			{
				unique_lock<mutex> lg(mtxOut);

				json* js = new json(4);
				js->convert_object()->insert({ "timestamp", new json(0, chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now() - start).count()) });
				js->convert_object()->insert({ "upQueue" , new json(3) });
				js->convert_object()->insert({ "downQueue" , new json(3) });
				js->convert_object()->insert({ "carts", new json(3) });
				for (Cart& cart : sys.carts) {
					json* cartJson = new json(4);
					cartJson->convert_object()->insert({ "pos", new json(0, cart.pos) });
					cartJson->convert_object()->insert({ "state", new json(0, cart.state) });
					cartJson->convert_object()->insert({ "stopLevels",new json(3) });
					vector<json*>* vec = cartJson->convert_object()->at("stopLevels")->convert_vector();

					for (int num : cart.upLevels)
						js->convert_object()->at("upQueue")->convert_vector()->push_back(new json(0, num));
					
					for (int num : cart.downLevels)
						js->convert_object()->at("downQueue")->convert_vector()->push_back(new json(0, num));
					
					for (int num : cart.stopLevels)
						vec->push_back(new json(0, num));

					js->convert_object()->at("carts")->convert_vector()->push_back(cartJson);
				}
				out << js->serialize() << endl;
				delete (js);
			}

			for (size_t i = 0; i < sys.carts.size(); ++i) {
				cout << sys.carts[i].pos;

				switch (sys.carts[i].state)
				{
				case CartState::idle:
					cout << " ×";
					break;
				case CartState::up:
					cout << " ↑×";
					break;
				case CartState::down:
					cout << " ↓×";
					break;
				case CartState::up_open:
					cout << " ↑√";
					break;
				case CartState::down_open:
					cout << " ↓√";
					break;
				case CartState::idle_open:
					cout << " √";
					break;
				}

				cout << '\t';
			}
			cout << endl;
		}
		this_thread::sleep_for(OUTPUT_SAMPLE_INTERNAL);
	}
}

void upControl(Elevator& sys) {
	while (running) {
		if (sys.upQueue.empty()) {
			this_thread::sleep_for(CTRL_INTERNAL);
		}
		else {
			unique_lock<mutex> lg(sys.mtx);

			vector<pair<int, int>> prior;
			for (size_t i = 0; i < sys.carts.size(); ++i) {
				prior.push_back({ i, sys.carts[i].getResponseTime(sys.upQueue.front(), true) });
			}
			sort(prior.begin(), prior.end(), [](const pair<int, int>& a, const pair<int, int>& b) {
				return a.second < b.second;
			});

			sys.carts[prior.front().first].addLevel(sys.upQueue.front(), true);

#ifdef OUTPUT_COMMANDS
			unique_lock<mutex> lgIO(mtxOut);
			out << "{\"type\":\"alloc\",\"cart\":" << prior.front().first << ",\"dir\":\"up\",\"des\":" << sys.upQueue.front() << "}" << endl;
#endif
			sys.upQueue.pop();
		}
	}
}

void downControl(Elevator& sys) {
	while (running) {
		if (sys.downQueue.empty()) {
			this_thread::sleep_for(CTRL_INTERNAL);
		}
		else {
			unique_lock<mutex> lg(sys.mtx);

			vector<pair<int, int>> prior;
			for (size_t i = 0; i < sys.carts.size(); ++i) {
				prior.push_back({ i, sys.carts[i].getResponseTime(sys.downQueue.front(), false) });
			}
			sort(prior.begin(), prior.end(), [](const pair<int, int>& a, const pair<int, int>& b) {
				return a.second < b.second;
			});

			sys.carts[prior.front().first].addLevel(sys.downQueue.front(), false);

#ifdef OUTPUT_COMMANDS
			unique_lock<mutex> lgIO(mtxOut);
			out << "{\"type\":\"alloc\",\"cart\":" << prior.front().first << ",\"dir\":\"down\",\"des\":" << sys.downQueue.front() << "}" << endl;
#endif
			sys.downQueue.pop();
		}
	}
}

void cart_handle(Elevator& sys, int index) {
	Cart& cart = sys.carts[index];

	while (running) {
		switch (cart.state)
		{
		case CartState::idle:
			this_thread::sleep_for(IDLE_COST);
			break;
		case CartState::up:
		case CartState::down:
			this_thread::sleep_for(MOVE_COST);
			break;
		case CartState::up_open:
		case CartState::down_open:
		case CartState::idle_open:
			this_thread::sleep_for(STOP_COST);
			break;
		}

		switch (cart.state)
		{
		case CartState::idle: {
			if(!cart.upLevels.empty()) {
				int begin = *(cart.upLevels.begin());
				if (begin < cart.pos) {
					cart.state = CartState::down;
				}
				else if (begin == cart.pos) {
					cart.upLevels.erase(cart.pos);
					cart.state = CartState::up_open;
				}
				else {
					cart.state = CartState::up;
				}
			}
			else if (!cart.downLevels.empty()) {
				int end = *(--cart.downLevels.end());
				if (end > cart.pos) {
					cart.state = CartState::up;
				}
				else if (end == cart.pos) {
					cart.downLevels.erase(cart.pos);
					cart.state = CartState::down_open;
				}
				else {
					cart.state = CartState::down;
				}
			}
			else {
				cart.state = CartState::idle;
			}
			break;
		}
		case CartState::up: {
			++cart.pos;

			if (cart.upLevels.count(cart.pos)) {
				cart.upLevels.erase(cart.pos);
				cart.state = CartState::up_open;

				if (cart.stopLevels.count(cart.pos))
					cart.stopLevels.erase(cart.pos);
			}
			else if (cart.stopLevels.count(cart.pos)) {
				cart.stopLevels.erase(cart.pos);

				if (cart.stopLevels.empty() && cart.upLevels.empty() && cart.downLevels.empty()) {
					cart.state = CartState::idle_open;
				}
				else if (cart.stopLevels.empty()) {
					int highest = max(cart.upLevels.empty() ? cart.pos : *(--cart.upLevels.end()), cart.downLevels.empty() ? cart.pos : *(--cart.downLevels.end()));
					if (highest > cart.pos) {
						cart.state = CartState::up_open;
					}
					else if (highest == cart.pos) {
						if (cart.downLevels.count(cart.pos)) {
							cart.downLevels.erase(cart.pos);
							cart.state = CartState::down_open;
						}
						else {
							cart.state = CartState::down_open;
						}
					}
				}
				else {
					cart.state = CartState::up_open;
				}
			}
			else if (cart.downLevels.count(cart.pos)
				&& cart.pos == *(--cart.downLevels.end())
				&& cart.stopLevels.empty()
				&& (cart.upLevels.empty() || *(--cart.upLevels.end()) < cart.pos)) {
				cart.downLevels.erase(cart.pos);
				cart.state = CartState::down_open;
			}
			else {
				cart.state = CartState::up;
			}
			break;
		}
		case CartState::down: {
			--cart.pos;

			if (cart.downLevels.count(cart.pos)) {
				cart.downLevels.erase(cart.pos);
				cart.state = CartState::down_open;

				if (cart.stopLevels.count(cart.pos))
					cart.stopLevels.erase(cart.pos);
			}
			else if (cart.stopLevels.count(cart.pos)) {
				cart.stopLevels.erase(cart.pos);

				if (cart.stopLevels.empty() && cart.upLevels.empty() && cart.downLevels.empty()) {
					cart.state = CartState::idle_open;
				}
				else if (cart.stopLevels.empty()) {
					int lowest = max(cart.upLevels.empty() ? cart.pos : *(cart.upLevels.begin()), cart.downLevels.empty() ? cart.pos : *(cart.downLevels.begin()));
					if (lowest < cart.pos) {
						cart.state = CartState::down_open;
					}
					else if (lowest >= cart.pos) {
						if (cart.upLevels.count(cart.pos)) {
							cart.upLevels.erase(cart.pos);
							cart.state = CartState::up_open;
						}
						else {
							cart.state = CartState::up_open;
						}
					}
				}
				else {
					cart.state = CartState::down_open;
				}
			}
			else if (cart.upLevels.count(cart.pos)
				&& cart.pos == *(cart.upLevels.begin())
				&& cart.stopLevels.empty()
				&& (cart.downLevels.empty() || *(cart.downLevels.begin()) > cart.pos)) {
				cart.upLevels.erase(cart.pos);
				cart.state = CartState::up_open;
			}
			else {
				cart.state = CartState::down;
			}
			break;
		}
		case CartState::up_open: {
			if (cart.stopLevels.empty() && cart.upLevels.empty() && cart.downLevels.empty()) {
				cart.state = CartState::idle;
			}
			else if (cart.stopLevels.empty()) {
				int highest = max(cart.upLevels.empty() ? cart.pos : *(--cart.upLevels.end()), cart.downLevels.empty() ? cart.pos : *(--cart.downLevels.end()));
				if (highest > cart.pos) {
					cart.state = CartState::up;
				}
				else if (highest == cart.pos) {
					if (cart.downLevels.count(cart.pos)) {
						cart.downLevels.erase(cart.pos);
						cart.state = CartState::down;
					}
					else {
						cart.state = CartState::down;
					}
				}
			}
			else {
				cart.state = CartState::up;
			}
			break;
		}
		case CartState::down_open: {
			if (cart.stopLevels.empty() && cart.upLevels.empty() && cart.downLevels.empty()) {
				cart.state = CartState::idle;
			}
			else if (cart.stopLevels.empty()) {
				int lowest = min(cart.upLevels.empty() ? cart.pos : *(cart.upLevels.begin()), cart.downLevels.empty() ? cart.pos : *(cart.downLevels.begin()));
				if (lowest < cart.pos) {
					cart.state = CartState::down;
				}
				else if (lowest >= cart.pos) {
					if (cart.upLevels.count(cart.pos)) {
						cart.upLevels.erase(cart.pos);
						cart.state = CartState::up;
					}
					else {
						cart.state = CartState::up;
					}
				}
			}
			else {
				cart.state = CartState::down;
			}
			break;
		}
		case CartState::idle_open: {
			cart.state = CartState::idle;
			break;
		}
		}
	}
}

int main() {
	cout << "Number of carts (if you want to use GUI tool, a number no larger than 5 is recommended): ";
	cin >> cart_number;
	cout << "Number of levels (if you want to use GUI tool, a number no larger than 20 is recommended): ";
	cin >> max_level;

	{
		unique_lock<mutex> lg(mtxOut);
		
		json* js = new json(4);
		js->convert_object()->insert({ "levels", new json(0, max_level) });
		js->convert_object()->insert({ "carts", new json(0, cart_number) });
		out << js->serialize() << endl;
		delete (js);
	}
	
	start = chrono::system_clock::now();

	Elevator sys;

	thread thread_up_control(upControl, ref(sys));
	thread thread_down_control(downControl, ref(sys));
	thread thread_input(input, ref(sys));
	thread thread_output(output, ref(sys));
	vector<thread> thread_carts;
	for (int i = 0; i < cart_number; ++i)
		thread_carts.emplace_back(thread(cart_handle, ref(sys), i));

	thread_up_control.join();
	thread_down_control.join();
	thread_input.join();
	thread_output.join();
	for (thread& th : thread_carts) th.join();

	out.close();

	return 0;
}
