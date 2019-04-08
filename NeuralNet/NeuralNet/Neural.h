#pragma once
#include"Pictures.h"
double sigmoid(double x) {
	
	return (1 / (1 + exp(-x)));
}
double DerSigmoid(double x) {
	
	return (sigmoid(x) * (1 - sigmoid(x)));
}
double Normal_Mersenne(double start, double end) {
	random_device seed;
	mt19937 gen(seed());
	uniform_real_distribution<double> dist(start, end);
	return dist(gen);
}
class Axon {

public:
	double weight;
	double PartialDerivative;
};
class Neuron {//jeden neuron ktory trzyma wartosc Aktywacji oraz Bajasy
public:
	double Activation;
	double Bias;
	double DerivativeBias;
	double ThirdDerivative;
	double ActiveDerSigm;
	vector<Axon> Connections;

};
class Layer {//pojedyncza warstwa
public:
	vector<Neuron>OneLayer;
};

class Network {//ogolna struktura klasy NN
	vector<int>Info;
	vector<Layer>Hidden;
	vector<double>Answers;
	int LayersAmmount;
	void SetNewActiv();
	void CountDerivatives(int Prev, int Current);
	void MakingStep(int exam);
	vector<Set> train;
	vector<Set> test;
	void LastLayer();
	void backpropagation();
	void MakingChain(int Next,int Current);
	void SeedFirstLay(Pictures example);
	int Biggest();
public:
	int AnswerOne;
	void SetTraining(vector<Set> trenuj);
	void show();
	Network(vector<int>Informations);
	void Learning();
	int AllExamples;
	int Correct;
	void Testing();
	void SetTest(vector<Set> Testuj);
};
Network::Network(vector<int>Informations):Info(Informations), LayersAmmount(Informations.size()){
	
	//size to ilosc wartw, a dane w srodku mowia o ilosciach neuronow
	Layer First;
	for (int i = 0; i < Info[0]; i++) {
		Neuron x;
		x.Activation = 0;
		x.Bias = 0;
		x.ThirdDerivative = 0;
		x.ActiveDerSigm = 0;
		x.DerivativeBias = 0;
		First.OneLayer.push_back(x);
	}
	Hidden.push_back(First);//wkladam czysta 1 warstwe do vectora warstw
	for (int j = 1; j < LayersAmmount; j++) {
		Layer temp;
		for (int i = 0; i < Info[j]; i++) {
			Neuron TempNeuron;
			TempNeuron.Activation = 0;
			TempNeuron.Bias = (Normal_Mersenne(-2, 2));
			TempNeuron.DerivativeBias = 0;
			TempNeuron.ThirdDerivative = 0;
			TempNeuron.ActiveDerSigm = 0;
			for (int k = 0; k < Info[j - 1]; k++) {
				Axon Connect;
				Connect.weight= (Normal_Mersenne(-1, 1));
				Connect.PartialDerivative = 0;
				TempNeuron.Connections.push_back(Connect);
			}
			temp.OneLayer.push_back(TempNeuron);
		}
		Hidden.push_back(temp);
	}
	int lastlayer = Hidden.size() - 1;
	for (int j = 0; j < Hidden[lastlayer].OneLayer.size(); j++) {
		Answers.push_back(0);
	}
	AllExamples = 0;
	Correct = 0;
	AnswerOne = 0;
}
void Network::SetTraining(vector<Set> trenuj) {
	train = trenuj;
}
void Network::SeedFirstLay( Pictures example) {
	for (int i = 0; i < Hidden[0].OneLayer.size(); i++) {//seduje aktywacje
		Hidden[0].OneLayer[i].Activation = example.RGBPicture[i];
	}
	//clear answer
	AnswerOne = example.label;
	for (int i = 0; i < Answers.size(); i++) {
		Answers[i] = 0;
	}
	Answers[example.label] = 1;
}
void Network::Learning() {
	for (int i = 0; i < train.size(); i++) {
		int examples = train[i].OneTest.size();

		for (int o = 0; o < train[i].OneTest.size(); o++) {
			SeedFirstLay(train[i].OneTest[o]);
			SetNewActiv();
			backpropagation();
		}
		cout << "SET<<" << i+1 << endl;
		MakingStep(examples);//dodanie usrednionego gradientu do wag i biasow
	}
	
}
void Network::SetTest(vector<Set> Testuj) {
	test = Testuj;
}
void Network::Testing() {
	for (int j = 0; j < test.size(); j++) {
		for (int o = 0; o < test[j].OneTest.size(); o++) {
			SeedFirstLay(test[j].OneTest[o]);
			SetNewActiv();//aktywacje
			show();//sprawdza wyniki
		}
	}
}
void Network::SetNewActiv() {
	double tempAct = 0;
	for (int i = 1; i < Hidden.size(); i++) {//layer
		for (int k = 0; k < Hidden[i].OneLayer.size(); k++) {//neuron
			for (int f = 0; f < Hidden[i].OneLayer[k].Connections.size(); f++) {//connect
				tempAct += Hidden[i].OneLayer[k].Connections[f].weight*Hidden[i - 1].OneLayer[f].Activation;
			}
			tempAct+=Hidden[i].OneLayer[k].Bias;
			Hidden[i].OneLayer[k].ActiveDerSigm = DerSigmoid(tempAct);//zapisuje z
			Hidden[i].OneLayer[k].Activation = sigmoid(tempAct);
			tempAct = 0;

		}
	}

}
void Network::backpropagation() {
	int last = Hidden.size() - 2;
	int ll = Hidden.size() - 1;
	int size = Hidden.size();
	LastLayer();
	for (int i = last; i > 0; i--) {
		MakingChain(i + 1, i);
	}
	for (int i = 1; i < size; i++) {
		CountDerivatives(i-1,i);
	}
	

}
void Network::MakingChain(int Next, int Current) {
	double get = 0;
	for (int p = 0; p < Hidden[Current].OneLayer.size(); p++) {
		for (int i = 0; i < Hidden[Next].OneLayer.size(); i++) {
			get += Hidden[Next].OneLayer[i].Connections[p].weight * Hidden[Next].OneLayer[i].ActiveDerSigm *  Hidden[Next].OneLayer[i].ThirdDerivative;
		}
		Hidden[Current].OneLayer[p].ThirdDerivative = get;
		get = 0;

	}


}
void Network::CountDerivatives(int Prev, int Current) {
	double GradWeigh = 0;
	double BiasStep = 0;
	for (int i = 0; i < Hidden[Current].OneLayer.size(); i++) {
		for (int h = 0; h < Hidden[Current].OneLayer[i].Connections.size(); h++) {
				GradWeigh = Hidden[Prev].OneLayer[h].Activation * Hidden[Current].OneLayer[i].ActiveDerSigm * Hidden[Current].OneLayer[i].ThirdDerivative;
				Hidden[Current].OneLayer[i].Connections[h].PartialDerivative += GradWeigh;
				GradWeigh = 0;
			
		}
		BiasStep = Hidden[Current].OneLayer[i].ActiveDerSigm * Hidden[Current].OneLayer[i].ThirdDerivative;
		Hidden[Current].OneLayer[i].DerivativeBias += BiasStep;
		BiasStep = 0;

	}

	
}
void Network::MakingStep(int exam) {
	double GStep = 0;
	double biasStep = 0;
	for (int i = 1; i < Hidden.size(); i++) {
		for (int p = 0; p < Hidden[i].OneLayer.size(); p++) {
			for (int k = 0; k < Hidden[i].OneLayer[p].Connections.size(); k++) {
				GStep = (Hidden[i].OneLayer[p].Connections[k].PartialDerivative) / (double( exam));
				Hidden[i].OneLayer[p].Connections[k].weight -= GStep;
				Hidden[i].OneLayer[p].Connections[k].PartialDerivative = 0;
				GStep = 0;
			}
			biasStep = (Hidden[i].OneLayer[p].DerivativeBias) / double(exam);
			Hidden[i].OneLayer[p].Bias -= biasStep;//robie krok biasu
			Hidden[i].OneLayer[p].DerivativeBias = 0;
			biasStep = 0;
		}
	}

}
void Network::LastLayer() {//gg=dobrze
	int Last = Hidden.size() - 1;
	double third;
	for (int i = 0; i < Hidden[Last].OneLayer.size(); i++) {
		third = 2 * (Hidden[Last].OneLayer[i].Activation - Answers[i]);
		Hidden[Last].OneLayer[i].ThirdDerivative = third;
		third = 0;
	}
}
int Network::Biggest() {
	int answer = 0;
	int last = Hidden.size() - 1;
	double temp = Hidden[last].OneLayer[0].Activation;
	for (int i = 1; i < Hidden[last].OneLayer.size(); i++) {
		if (temp < Hidden[last].OneLayer[i].Activation) {
			answer = i;
			temp = Hidden[last].OneLayer[i].Activation;
		}
	}
	return answer;
}
void Network::show() {
	int ans = Biggest();
	if (AnswerOne == ans) {
		Correct += 1;
		AllExamples += 1;
	}
	else {
		AllExamples += 1;
	}
	
}