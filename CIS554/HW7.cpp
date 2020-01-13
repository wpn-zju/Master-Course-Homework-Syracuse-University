#include <iostream>

using namespace std;

int getDis(int level) {
	return level < 4 ? 2 : 3 * (1 << (level - 4 >> 1));
}

int getScale(int level) {
	return level < 3 ? 1 : 3 * (1 << (level - 1 >> 1)) - 1;
}

int getAnchor(int level) {
	return getScale(level) >> 1;
}

void buildTree(int level, int anchorX, int anchorY, char** output) {
	output[anchorX][anchorY] = 'O';
	if (level == 1)
		return;
	if (level & 1) {
		for (int i = 1; i < getDis(level); ++i)
			output[anchorX][anchorY - i] = output[anchorX][anchorY + i] = '-';
		buildTree(level - 1, anchorX, anchorY - getDis(level), output);
		buildTree(level - 1, anchorX, anchorY + getDis(level), output);
	}
	else {
		for (int i = 1; i < getDis(level); ++i)
			output[anchorX - i][anchorY] = output[anchorX + i][anchorY] = '|';
		buildTree(level - 1, anchorX - getDis(level), anchorY, output);
		buildTree(level - 1, anchorX + getDis(level), anchorY, output);
	}
}

void GenerateTree(int level) {
	char** matrix = new char*[getScale(level + 1)];
	for (int i = 0; i < getScale(level + 1); ++i)
		matrix[i] = new char[getScale(level)];
	for (int i = 0; i < getScale(level + 1); ++i)
		for (int j = 0; j < getScale(level); ++j)
			matrix[i][j] = ' ';
	buildTree(level, getAnchor(level + 1), getAnchor(level), matrix);
	for (int i = 0; i < getScale(level + 1); ++i) {
		for (int j = 0; j < getScale(level); ++j)
			cout << matrix[i][j];
		cout << endl;
	}
	cout << endl;
	for (int i = 0; i < getScale(level + 1); ++i)
		delete[](matrix[i]);
	delete[](matrix);
}

int main() {
	for (int i = 1; i <= 9; ++i)
		GenerateTree(i);
	system("pause");
	return 0;
}