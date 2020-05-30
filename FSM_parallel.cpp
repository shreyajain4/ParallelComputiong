#include <iostream>
#include <sstream>

#include <omp.h>
#include <string.h>
#include <sys/time.h>

#include <stdlib.h>
#include <stdio.h>
#include <bits/stdc++.h>

using namespace std;

int string_size = 1000000;
int no_of_states = 2;
int alphabet_size = 1;
int thread_count = 100;
vector<int> action_string;
vector<vector<int> > state_action_mapping;

vector<vector<int> > thread_action_mapping;


int is_state(int state) {
	if (0 <= state && state < no_of_states)
	return 1;

	cout << "Error: Invalid state (" << state << ")\n";
	return -1;
}

int is_action(int act) {
	if(act<alphabet_size)
	return 1;

	cout << "Error: Invalid action (" << act << ")\n";
	return -1;
}

void mapping_states()
{
	vector<int> p(alphabet_size, 0);
	vector<vector<int> > dp(no_of_states, p);
	for(int i=0;i<dp.size();i++)
	{
		for(int j=0;j<dp[0].size();j++)
		{
			dp[i][j] = rand()%no_of_states;
		}
	}
	state_action_mapping = dp;
}

int get_next_state(int state, int action) {

	if (!is_action(action) || !is_state(state))
	return -1;
	
	return state_action_mapping[state][action];
}


int get_final_state(int state,  int start,  int end) {

	int tmp = state;

	for (int i = start; i < end; i++) {
	tmp = get_next_state(tmp, action_string[i]);
	}

	return tmp;
}

int par_get_final_state() {

	//cout<<"parallel states"<<endl;
	omp_set_num_threads(thread_count);


	int per_thread = string_size / thread_count;

	int next_state = 0;
	int thread_id;
	int start_index, end_index;
#pragma omp parallel private(thread_id, start_index, end_index)
	{
		thread_id = omp_get_thread_num();

		start_index = per_thread * thread_id;
		end_index = per_thread * (thread_id + 1);

		if(thread_id == thread_count - 1)
		    end_index = string_size;


		if (thread_id == 0) {
		    next_state = get_final_state(0, start_index, end_index);
		} else {
		    for(int i=0;i<no_of_states;i++)
		    thread_action_mapping[thread_id][i]=get_final_state(i, start_index, end_index);
		}
	}
	
	for (int i = 1; i < thread_count; i++) {
	//cout<<next_state<<endl;
	next_state = thread_action_mapping[i][next_state];
	}
	//cout<<next_state<<endl;
	return next_state;
}

int main(int argc, char* argv[]) {
	
	cout<<"variables for this program is int string_size = 1000000;int no_of_states = 2;int alphabet_size = 1;int thread_count = 100;    These can be changed in the code " <<endl;
	mapping_states();
	
	vector<int> action(string_size,0);
	for(int i=0;i<action.size();i++)
	action[i]=rand()%alphabet_size;
	
	
	action_string = action;
	vector<int> p(no_of_states, -1);
	vector<vector<int> > dp(thread_count,p);
	thread_action_mapping = dp;
	
	struct timeval par_start, par_end, seq_start, seq_end;

	gettimeofday(&par_start, NULL);
	int par_state = par_get_final_state();
	gettimeofday(&par_end, NULL);


	gettimeofday(&seq_start, NULL);
	int seq_state = get_final_state(0, 0, string_size);
	gettimeofday(&seq_end, NULL);

	

	long int serialTime = ((seq_end.tv_sec * 1000000 + seq_end.tv_usec)
		- (seq_start.tv_sec * 1000000 + seq_start.tv_usec));
	long int parallelTime = ((par_end.tv_sec * 1000000 + par_end.tv_usec)
		- (par_start.tv_sec * 1000000 + par_start.tv_usec));

	
	cout << thread_count << "," << serialTime << "," << parallelTime << endl;
	
cout << seq_state<<"is the final serial state"<<endl;
cout << par_state<<"is the final parallel state"<<endl;
}