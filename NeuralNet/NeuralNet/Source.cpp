#include"std_lib_facilities.h"
#include"Neural.h"
#include"Pictures.h"
#include<ctime>
#include<algorithm>
Set GetTraintSet(int i);
Set GetTestSet(int i);

void save_percents(int correct, int all, int which);
int Uniform_Mersenne(int Low, int Up) {
	random_device seed;
	mt19937 gen(seed());
	uniform_int_distribution<int> dist(Low, Up);
	return dist(gen);
}

int main() {
	try {
		Converter x(100,100);
		cout << "START CONVERTER" << endl;
	

		vector<Set>all;
		all= x.GettingRawPict();//bez plikow jade prosto z mnist

		
	
		vector<Set>Training;
		vector<Set>Test;
		for (int i = 0; i < 80; i++) {
			Training.push_back(all[i]);
		}
		for (int j = 80; j < all.size(); j++) {
			Test.push_back(all[j]);
		}

		Network nn({ 784,16,16,10 });
		nn.SetTraining(Training);
		nn.SetTest(Test);
		cout << "SETS LOADED TO RAM" << endl;
		for (int i = 0; i < 20; i++) {//20 cykli
			clock_t start = clock();
				nn.Learning();
				
				cout << "Learning cycle: " << i + 1 << endl;
				clock_t stop = clock();
				cout << "CZAS: " << (stop - start) / CLK_TCK << endl;


			cout << "Time for test!" << endl;
			
				
				nn.Testing();
				cout << "Test cycle: " << i + 1 << endl;
				cout << "Correct: " << (double(nn.Correct) / double(nn.AllExamples)) * 100 << "%" << endl;

			
			save_percents(nn.Correct, nn.AllExamples, i + 1);
			nn.Correct = 0;
			nn.AllExamples = 0;
			random_shuffle(Training.begin(), Training.end());
		}
		
		keep_window_open();
		return 0;
	}
	catch (exception &e) {
		cerr << "ERROR: " << e.what() << endl;
		keep_window_open();
		return 1;
	}
}
void save_percents(int correct, int all,int which) {
	double ratio = (double(correct) / double(all))*100;
	ofstream results;
	results.open(get_directory() + "\\save.csv", ios::app);

	results << which << "," << ratio << endl;
	results.close();
}