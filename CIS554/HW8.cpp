#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>

using namespace std;

vector<string> pointDic{ "A","2","3","4","5","6","7","8","9","10","J","Q","K" };
vector<char> suitDic{ 'C','D','H','S' };

class Card {
public:
	int point;
	int suit;
	int owner;
	bool handortable;

	Card(int point, int suit) {
		this->point = point;
		this->suit = suit;
	}

	string toString() {
		return pointDic[point] + suitDic[suit];
	}
};

class CardGame {
public:
	ofstream fout;
	int totalPlayerNum;
	int survivedPlayerNum;
	vector<Card*> cardList;
	vector<Card*> tableList;
	vector<int> cardNumber;
	vector<bool> survived;

	CardGame(int n, int dealer) {
		srand(time(0));

		fout.open(".\\data1.txt", ios::trunc);
		fout.close();
		fout.open(".\\data1.txt", ios::app);

		totalPlayerNum = survivedPlayerNum = n;
		cardNumber = vector<int>(totalPlayerNum, 0);
		survived = vector<bool>(totalPlayerNum, true);
		init(dealer);

		fout << "**** initial deck of cards before shuffle ****" << endl << endl;
		fout << "KC QC JC 10C 9C 8C 7C 6C 5C 4C 3C 2C AC KD QD JD 10D 9D 8D 7D 6D 5D 4D 3D 2D AD KH QH JH 10H 9H 8H 7H 6H 5H 4H 3H 2H AH KS QS JS 10S 9S 8S 7S 6S 5S 4S 3S 2S AS " << endl << endl;
		fout << "**** after shuffle ****" << endl << endl;
		for (Card*& c : cardList)
			fout << c->toString() << ' ';
		fout << endl << endl;
		fout << "**** after cards are dealt ****" << endl << endl;
		vector<string> strs(totalPlayerNum, "");
		for (int i = cardList.size() - 1; i >= 0; --i)
			strs[cardList[i]->owner] += cardList[i]->toString() + ' ';
		for (int i = 0; i < totalPlayerNum; ++i) {
			fout << "cards for player " << to_string(i + 1) << endl;
			fout << strs[i] << endl << endl;
		}

		while (survivedPlayerNum > 1)
			nextRound();

		fout << "Game Over!!  The winner is player ";

		for (int i = 0; i < totalPlayerNum; ++i)
			if (survived[i])
				fout << to_string(i + 1) << endl << endl;

		strs = vector<string>(totalPlayerNum, "");
		for (int i = cardList.size() - 1; i >= 0; --i)
			strs[cardList[i]->owner] += cardList[i]->toString() + ' ';
		for (int i = 0; i < totalPlayerNum; ++i) {
			fout << "Final hand " << to_string(i + 1) << endl;
			fout << strs[i] << endl << endl;
		}

		fout.close();
	}

	~CardGame() {
		for (Card*& c : cardList)
			delete c;
	}

	void init(int dealer) {
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 13; ++j)
				cardList.push_back(new Card(j, i));

		shuffle(-1);

		for (int i = 0; i < 52; ++i) {
			cardNumber[(i + dealer + 1) % totalPlayerNum]++;
			cardList[i]->owner = (i + dealer + 1) % totalPlayerNum;
			cardList[i]->handortable = true;
		}
	}

	void nextRound() {
		print(false);

		for (int i = 0; i < totalPlayerNum; ++i) {
			for (int j = cardList.size() - 1; j >= 0; --j) {
				if (cardList[j]->owner == i) {
					cardList[j]->handortable = false;
					tableList.push_back(cardList[j]);
					break;
				}
			}
		}

		print(true);

		sort(tableList.begin(), tableList.end(), [](Card*& a, Card*& b) { return a->point < b->point; });
		while (tableList.back()->point > tableList.front()->point)
			tableList.pop_back();

		while (tableList.size() > 1) {
			print(false);

			vector<Card*> tmpList;
			for (Card*& c : tableList)
				push2More(c->owner, tmpList);

			print(true);

			sort(tmpList.begin(), tmpList.end(), [](Card*& a, Card*& b) { return a->point < b->point; });
			while (tmpList.back()->point > tmpList.front()->point)
				tmpList.pop_back();

			tableList.swap(tmpList);
		}

		count(tableList.front()->owner);

		shuffle(tableList.front()->owner);

		tableList.pop_back();
	}

	void shuffle(int winner) {
		if (winner == -1) {
			for (int i = cardList.size() - 1; i >= 0; --i)
				swap(cardList[i], cardList[rand() % (i + 1)]);
		}
		else {
			vector<int> index;
			for (int i = 0; i < cardList.size(); ++i)
				if (cardList[i]->owner == winner)
					index.push_back(i);
			for (int i = index.size() - 1; i >= 0; --i) {
				int r = rand() % (i + 1);
				swap(cardList[index[i]], cardList[index[r]]);
				swap(index[i], index[r]);
			}
		}
	}

	void push2More(int player, vector<Card*>& target) {
		bool flag = false;
		for (int i = cardList.size() - 1; i >= 0; --i) {
			if (cardList[i]->owner == player && cardList[i]->handortable) {
				if (flag) {
					cardList[i]->handortable = false;
					target.push_back(cardList[i]);
					return;
				}
				else {
					flag = true;
					cardList[i]->handortable = false;
				}
			}
		}
	}

	void count(int winner) {
		for (Card*& c : cardList) {
			if (c->handortable == false) {
				cardNumber[c->owner]--;
				cardNumber[winner]++;
				c->handortable = true;
				c->owner = winner;
			}
		}

		for (int i = 0; i < totalPlayerNum; ++i)
			if (survived[i] && cardNumber[i] < 5)
				getout(i, winner);
	}

	void getout(int loser, int winner) {
		for (Card*& c : cardList) {
			if (c->owner == loser) {
				c->owner = winner;
				cardNumber[winner]++;
				cardNumber[loser]--;
			}
		}
		--survivedPlayerNum;
		survived[loser] = false;
	}

	void print(bool table) {
		vector<string> strs(totalPlayerNum, "");
		if (table) {
			for (int i = 0; i < cardList.size(); ++i)
				if (!cardList[i]->handortable)
					strs[cardList[i]->owner] += cardList[i]->toString() + ' ';
		}
		else {
			for (int i = cardList.size() - 1; i >= 0; --i)
				if (cardList[i]->handortable)
					strs[cardList[i]->owner] += cardList[i]->toString() + ' ';
		}
		for (int i = 0; i < totalPlayerNum; ++i) {
			if (table)
				fout << "table " << to_string(i + 1) << endl;
			else
				fout << "Hand " << to_string(i + 1) << endl;
			fout << strs[i] << endl << endl;
		}
	}
};

int main() {
	int n, dealer;
	cout << "Input n: ";
	cin >> n;
	cout << "Input dealer (from 1 to n): ";
	cin >> dealer;
	if (dealer < 1 || dealer > n)
		cout << "Invalid Input" << endl;
	else {
		CardGame cg(n, dealer - 1);
		cout << "File Saved" << endl;
	}
	system("pause");
	return 0;
}