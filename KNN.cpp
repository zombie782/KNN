#include<iostream>
#include<iomanip>
#include<fstream>
#include<vector>
#include<algorithm>
#include<utility>
#include<numeric>
#include<cmath>

#define RADIX dec

using namespace std;

const string REF_FILE_NAME="ref.txt";
const string QUERY_FILE_NAME="query.txt";

const unsigned CLASSIFICATIONS=2;
const unsigned DIMS=30;
const unsigned K=20;

const unsigned QUERY_DATA_POINTS=169;

double square(double x){return x*x;}

vector<unsigned> knn(const pair<vector<vector<double>>,vector<unsigned>>& ref,const vector<vector<double>>& query){
	
	vector<vector<double>> distances;
	for(unsigned i=0;i!=query.size();++i){
		vector<double> dist;
		for(unsigned j=0;j!=ref.first.size();++j){
			double dist_sum=0;
			for(unsigned k=0;k!=ref.first[0].size();++k){
				dist_sum+=square(query[i][k]-ref.first[j][k]);
			}
			dist.push_back(sqrt(dist_sum));
		}
		distances.push_back(dist);
	}
	vector<unsigned> classif(QUERY_DATA_POINTS,0);
	vector<vector<unsigned>> indices(distances.size(),vector<unsigned>(ref.first.size(),0));

	for(auto& e:indices){
		iota(e.begin(),e.end(),0);
	}
	
	for(unsigned i=0;i!=indices.size();++i){
		vector<double> dist_i=distances[i];
		sort(indices[i].begin(),indices[i].end(),[&dist_i](const unsigned& a,const unsigned& b){return dist_i[a]<dist_i[b];});
		vector<unsigned> tally(CLASSIFICATIONS,0);
		for(unsigned j=0;j!=K;++j){
			//cout<<indices[i][j]<<" ";
			++tally[ref.second[indices[i][j]]];
		}
		//cout<<endl;
		classif[i]=max_element(tally.begin(),tally.end())-tally.begin();
	}
	
	return classif;
}

pair<vector<vector<double>>,vector<unsigned>> get_ref(){
	ifstream ifs(REF_FILE_NAME);
	vector<unsigned> classif;
	vector<vector<double>> dims;
	unsigned c;
	while(ifs>>RADIX>>c){
		vector<double> d;
		for(unsigned i=0;i!=DIMS;++i){
			double a;
			ifs>>RADIX>>a;
			d.push_back(a);
			//d.push_back(static_cast<double>(a));
		}
		classif.push_back(c);
		dims.push_back(d);
	}
	return make_pair(dims,classif);
}

vector<vector<double>> get_query(){
	ifstream ifs(QUERY_FILE_NAME);
	vector<vector<double>> dims;
	for(unsigned i=0;i!=QUERY_DATA_POINTS;++i){
		vector<double> d;
		for(unsigned j=0;j!=DIMS;++j){
			double a;
			ifs>>RADIX>>a;
			d.push_back(a);
			//d.push_back(static_cast<double>(a));
		}
		dims.push_back(d);
	}
	return dims;
}

int main(){
	auto ref=get_ref();
	auto query=get_query();
	/*
	for(auto e:query){
		for(auto f:e){
			cout<<RADIX<<f<<" ";
			//cout<<RADIX<<static_cast<unsigned>(f)<<" ";
		}
		cout<<endl;
	}
	*/
	
	//for(auto e:ref.second) cout<<e<<" ";
	
	/*
	for(auto e:ref.first){
		for(auto f:e){
			cout<<RADIX<<f<<" ";
			cout<<RADIX<<static_cast<unsigned>(f)<<" ";
		}
		cout<<endl;
	}
	*/
	
	auto classif=knn(ref,query);
	for(auto e:classif){
		cout<<e<<" ";
	}
	cout<<endl;
	
}
