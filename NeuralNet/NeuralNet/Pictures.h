#pragma once
string get_directory() {

	string file_path = __FILE__;
	string dir_path = file_path.substr(0, file_path.rfind("\\"));

	return dir_path;
}
class Pictures {
public:
	int label;
	vector<double>RGBPicture;
};
class Set {
public:
	vector<Pictures>OneTest;
	

};
class Converter {
	int Set_amm;
	int amm_examp;
	vector<Set>AllTests;
public:
	
	vector<Set> GettingRawPict();
	void GeneratingTraining();
	void GenerateTest();
	Converter(int SetAmmaount, int exampleAmm);
};
Converter::Converter(int SetAmmaount, int exampleAmm):Set_amm(SetAmmaount),amm_examp(exampleAmm){}
vector<Set> Converter::GettingRawPict() {
	ifstream GetPict;
	int pixels=784-1;
	string pix;
	double tempPix = 0;
	GetPict.open(get_directory() + "\\mnist_train.csv");
	for (int j = 0; j < Set_amm; j++) {
		Set SingleTest;
		for (int k = 0; k < amm_examp; k++) {
			Pictures Number;
			string temp;
			getline(GetPict, temp, ',');
			Number.label = atoi(temp.c_str());
			temp.clear();
			for (int u = 0; u < pixels; u++) {
				getline(GetPict, pix, ',');
				tempPix = atof(pix.c_str()) / 255;
				Number.RGBPicture.push_back(tempPix);
				pix.clear();
			}
			getline(GetPict, pix, '\n');
			tempPix = atof(pix.c_str()) / 255;
			Number.RGBPicture.push_back(tempPix);
			pix.clear();
			SingleTest.OneTest.push_back(Number);
		}
		AllTests.push_back(SingleTest);

	}
	GetPict.close();
	return AllTests;
}
void Converter::GeneratingTraining() {
	int TrainingSetAmm = 80;
	ofstream TrainingSet;
	
	for (int i = 0; i < TrainingSetAmm; i++) {
		stringstream temp;
		temp << get_directory() << "\\Training_set\\Training_set_" << (i + 1) << ".txt";
		TrainingSet.open(temp.str());
		for (int j = 0; j < AllTests[i].OneTest.size(); j++) {
			TrainingSet << AllTests[i].OneTest[j].label << " ";
			for (int k = 0; k < AllTests[i].OneTest[j].RGBPicture.size()-1; k++) {
				TrainingSet << AllTests[i].OneTest[j].RGBPicture[k] << " ";
			}
			TrainingSet << AllTests[i].OneTest[j].RGBPicture[783] << endl;
		}
		TrainingSet.close();
	}
}
void Converter::GenerateTest() {
	int TrainingSetAmm = 20;
	ofstream TestSet;

	for (int i = 80; i < AllTests.size(); i++) {
		stringstream temp;
		temp << get_directory() << "\\Test_set\\Test_set_" << (i + 1)-80 << ".txt";
		TestSet.open(temp.str());
		for (int j = 0; j < AllTests[i].OneTest.size(); j++) {
			TestSet << AllTests[i].OneTest[j].label << " ";
			for (int k = 0; k < AllTests[i].OneTest[j].RGBPicture.size()-1; k++) {
				TestSet << AllTests[i].OneTest[j].RGBPicture[k] <<" ";
			}
			TestSet <<AllTests[i].OneTest[j].RGBPicture[783]<< endl;
		}
		TestSet.close();
	}


}
Set GetTraintSet(int i) {
	Set New;
	ifstream NewSet;
	stringstream ssTemp;
	ssTemp << get_directory() << "\\Training_set\\Training_set_" << (i+1) << ".csv";
	NewSet.open(ssTemp.str());
	string pix;
	int pixels = 784-1;//IMPORTTANTE
	double tempPix = 0;
	while(NewSet.good()){
		Pictures Number;
		string temp;
		getline(NewSet, temp, ',');
		Number.label = atoi(temp.c_str());
		temp.clear();
		for (int u = 0; u < pixels; u++) {
			getline(NewSet, pix, ',');
			tempPix = atof(pix.c_str()) ;
			Number.RGBPicture.push_back(tempPix);
			pix.clear();
		}
		getline(NewSet, pix, '\n');
		tempPix = atof(pix.c_str()) ;
		Number.RGBPicture.push_back(tempPix);
		pix.clear();
		New.OneTest.push_back(Number);
	}
	NewSet.close();
	return New;
}
Set GetTestSet(int i) {
	Set New;
	ifstream NewSet;
	stringstream ssTemp;
	ssTemp << get_directory() << "\\Test_set\\Test_set_" << (i + 1) << ".csv";
	NewSet.open(ssTemp.str());
	string pix;
	int pixels = 784-1;
	double tempPix = 0;
	for (int k = 0; k < 100; k++) {
		Pictures Number;
		string temp;
		getline(NewSet, temp, ',');
		Number.label = atoi(temp.c_str());
		temp.clear();
		for (int u = 0; u < pixels; u++) {
			getline(NewSet, pix, ',');
			tempPix = atof(pix.c_str()) ;
			Number.RGBPicture.push_back(tempPix);
			pix.clear();
		}
		getline(NewSet, pix, '\n');
		tempPix = atof(pix.c_str()) ;
		Number.RGBPicture.push_back(tempPix);
		pix.clear();
		New.OneTest.push_back(Number);
	}
	NewSet.close();
	return New;
}