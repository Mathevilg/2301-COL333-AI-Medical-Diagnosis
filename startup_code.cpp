#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <set>
#include <map>
#include <cstring>
#include <unordered_set>
#include <unordered_map>
#include <iomanip>
#include <chrono>
#include <bits/stdc++.h>

using namespace std;

#define bug(...)       __f (#__VA_ARGS__, __VA_ARGS__)
#define print(a)       for(auto x : a) cout << x << " "; cout << endl

template <typename Arg1>
void __f (const char* name, Arg1&& arg1) { cout << name << " : " << arg1 << endl; }
template <typename Arg1, typename... Args>
void __f (const char* names, Arg1&& arg1, Args&&... args)
{
    const char* comma = strchr (names + 1, ',');
    cout.write (names, comma - names) << " : " << arg1 << " | "; __f (comma + 1, args...);
}
// Format checker just assumes you have Alarm.bif and Solved_Alarm.bif (your file) in current directory

// Our graph consists of a list of nodes where each node is represented as follows:
class Graph_Node{

private:
	string Node_Name;  // Variable name 
	vector<int> Children; // Children of a particular node - these are index of nodes in graph.
	vector<string> Parents; // Parents of a particular node- note these are names of parents
	int nvalues;  // Number of categories a variable represented by this node can take
	vector<string> values; // Categories of possible values
	vector<float> CPT; // conditional probability table as a 1-d array . Look for BIF format to understand its meaning

public:
	// Constructor- a node is initialised with its name and its categories
    Graph_Node(string name,int n,vector<string> vals)
	{
		Node_Name=name;
	
		nvalues=n;
		values=vals;
		

	}
	string get_name()
	{
		return Node_Name;
	}
	vector<int> get_children()
	{
		return Children;
	}
	vector<string> get_Parents()
	{
		return Parents;
	}
	vector<float> get_CPT()
	{
		return CPT;
	}
	int get_nvalues()
	{
		return nvalues;
	}
	vector<string> get_values()
	{
		return values;
	}
	void set_CPT(vector<float> new_CPT)
	{
		CPT.clear();
		CPT=new_CPT;
	}
    void set_Parents(vector<string> Parent_Nodes)
    {
        Parents.clear();
        Parents=Parent_Nodes;
    }
    // add another node in a graph as a child of this node
    int add_child(int new_child_index )
    {
        for(int i=0;i<Children.size();i++)
        {
            if(Children[i]==new_child_index)
                return 0;
        }
        Children.push_back(new_child_index);
        return 1;
    }



};


 // The whole network represted as a list of nodes
class network{

public:
	list <Graph_Node> Pres_Graph;
	int addNode(Graph_Node node)
	{
		Pres_Graph.push_back(node);
		return 0;
	}
    
    
	int netSize()
	{
		return Pres_Graph.size();
	}
    // get the index of node with a given name
    int get_index(string val_name)
    {
        list<Graph_Node>::iterator listIt;
        int count=0;
        for(listIt=Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++)
        {
            if(listIt->get_name().compare(val_name)==0)
                return count;
            count++;
        }
        return -1;
    }
// get the node at nth index
    list<Graph_Node>::iterator get_nth_node(int n)
    {
        list<Graph_Node>::iterator listIt;
        int count=0;
        for(listIt=Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++)
        {
            if(count==n)
                return listIt;
            count++;
        }
        return listIt; 
    }
    //get the iterator of a node with a given name
    list<Graph_Node>::iterator search_node(string val_name)
    {
        list<Graph_Node>::iterator listIt;
        for(listIt=Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++)
        {
            if(listIt->get_name().compare(val_name)==0)
                return listIt;
        }
    
            cout<<"node not found\n";
        return listIt;
    }
	

};

network read_network()
{
	network Alarm;
	string line;
	int find=0;
  	ifstream myfile("alarm.bif"); 
  	string temp;
  	string name;
  	vector<string> values;
  	
    if (myfile.is_open())
    {
    	while (! myfile.eof() )
    	{
    		stringstream ss;
      		getline (myfile,line);
      		
      		
      		ss.str(line);
     		ss>>temp;
     		
     		
     		if(temp.compare("variable")==0)
     		{
                    
     				ss>>name;
     				getline (myfile,line);
                   
     				stringstream ss2;
     				ss2.str(line);
     				for(int i=0;i<4;i++)
     				{
     					
     					ss2>>temp;
     					
     					
     				}
     				values.clear();
     				while(temp.compare("};")!=0)
     				{
     					values.push_back(temp);
     					
     					ss2>>temp;
    				}
     				Graph_Node new_node(name,values.size(),values);
     				int pos=Alarm.addNode(new_node);

     				
     		}
     		else if(temp.compare("probability")==0)
     		{
                    
     				ss>>temp;
     				ss>>temp;
     				
                    list<Graph_Node>::iterator listIt;
                    list<Graph_Node>::iterator listIt1;
     				listIt=Alarm.search_node(temp);
                    int index=Alarm.get_index(temp);
                    ss>>temp;
                    values.clear();
     				while(temp.compare(")")!=0)
     				{
                        listIt1=Alarm.search_node(temp);
                        listIt1->add_child(index);
     					values.push_back(temp);
     					
     					ss>>temp;

    				}
                    listIt->set_Parents(values);
    				getline (myfile,line);
     				stringstream ss2;
                    
     				ss2.str(line);
     				ss2>> temp;
                    
     				ss2>> temp;
                    
     				vector<float> curr_CPT;
                    string::size_type sz;
     				while(temp.compare(";")!=0)
     				{
                        
     					curr_CPT.push_back(atof(temp.c_str()));
     					
     					ss2>>temp;
                       
                        

    				}
                    
                    listIt->set_CPT(curr_CPT);


     		}
            else
            {
                
            }
     		
     		

    		
    		
    	}
    	
    	if(find==1)
    	myfile.close();
  	}
  	
  	return Alarm;
}

//returns the index of the value of a particular Variable (eg - X = True is 0th value, X = False is 1st value)
int valueIndex(Graph_Node g, string value)
{
	int numVal = g.get_nvalues();
	vector<string> poss_values = g.get_values();
	for(int i=0; i<numVal; i++)
	{
		if(poss_values[i] == value) return i;
	}
	return -1;
}
//returns which index to choose inside CPT of var, if its current Value has valIndex and its parents are set as row_record
int get_CPTindex(vector<string> &row_record, Graph_Node var, int valIndex, vector<Graph_Node> par_nodes, vector<int> parPos)
{
	
	vector<string> parents = (var).get_Parents();
	int numVal = var.get_nvalues();
	vector<int> value_indices;
	vector<int> totalValues;
	for(int i=0; i<parents.size(); i++)
	{
		string par = parents[i];
		int par_pos = parPos[i]; //this is the position in Alarm.bif
		Graph_Node par_node = par_nodes[i];
		
		int ind = valueIndex(par_node, row_record[par_pos]);
		value_indices.push_back(ind);
		totalValues.push_back(par_node.get_nvalues());
	}
	int mult = 1, cpt_index = 0;
	
	for(int par_index = parents.size()-1; par_index>=0; par_index--)
	{
		cpt_index += (mult*value_indices[par_index]);
		mult*=totalValues[par_index];
	}
	cpt_index+= (mult*valIndex);

	return cpt_index;
}

int get_CPT_childindex(vector<string> &row_record, Graph_Node var, int valIndex, vector<Graph_Node> par_nodes, vector<int> parPos, int XPos, int parValueInd)
{
	
	vector<string> parents = (var).get_Parents();
	int numVal = var.get_nvalues();
	vector<int> value_indices;
	vector<int> totalValues;
	for(int i=0; i<parents.size(); i++)
	{
		string par = parents[i];
		int par_pos = parPos[i]; //this is the position in Alarm.bif
		Graph_Node par_node = par_nodes[i];
		int ind;
		if(par_pos == XPos)
		{
			ind = parValueInd;
		}
		else ind = valueIndex(par_node, row_record[par_pos]);
		value_indices.push_back(ind);
		totalValues.push_back(par_node.get_nvalues());
	}
	int mult = 1, cpt_index = 0;
	
	for(int par_index = parents.size()-1; par_index>=0; par_index--)
	{
		cpt_index += (mult*value_indices[par_index]);
		mult*=totalValues[par_index];
	}
	cpt_index+= (mult*valIndex);

	return cpt_index;
}

int write_ans(network &Alarm)
{
	std::ifstream inputFile("alarm.bif");
    std::ofstream outputFile("solved_alarm.bif");
	int numvar = Alarm.netSize();
	if (!inputFile) {
        std::cerr << "Error: Could not open input file 'alarm.bif'\n";
        return 1;
    }

    if (!outputFile) {
        std::cerr << "Error: Could not create output file 'solved_alarm.bif'\n";
        return 1;
    }

    std::string line;
	int j = 0;
    while (std::getline(inputFile, line)) {
        // Check if the line starts with the word "table"

        size_t pos = line.find_first_not_of(" \t");
        if (pos != std::string::npos && line.substr(pos).find("table") == 0) {
            // Replace the line with the custom line
            outputFile << "table ";
			vector<float> cpt = (*Alarm.get_nth_node(j)).get_CPT();
			for(int k=0; k<cpt.size(); k++)
			{
				outputFile << std::fixed << std::setprecision(4) << cpt[k] << " ";
			}
			outputFile<<";"<<'\n';
			j++;
        } else {
            // Write the original line to the output file
            outputFile << line << std::endl;
        }
    }

    std::cout << "Customized file 'solved_alarm.bif' has been created successfully.\n";

    // Close the file streams
    inputFile.close();
    outputFile.close();
	return 0;
}

int main(int argc, char * argv[])
{
	time_t start;
	time(&start);
	network Alarm;
	Alarm=read_network();
	auto start_time = std::chrono::high_resolution_clock::now();

// Example: to do something
	int numVar = Alarm.netSize();
	ifstream myfile(argv[1]);
	string line;
	string temp; 
	vector<vector<string> > records; // records is a matrix of data
	vector<vector<bool> > unknown; // unknowns is a matrix, if ele==true then corresponsinf data is unknown (?)
	map<int,int> question_mark_index;
	int rowc = 0, colc = 0;
	if (myfile.is_open()) {
		
		while (! myfile.eof() ){
			stringstream ss;
			vector<string> l1;
			vector<bool> l2;
      		getline (myfile,line);
      		ss.str(line);
			colc = 0;
      		while (ss>>temp) {
				// cout << temp << " " ;
				l1.push_back(temp);
				if (temp.size() == 3) {l2.push_back(true); question_mark_index[rowc] = colc;}
				else {l2.push_back(false); }
				colc++;
			}
			records.push_back(l1);
			unknown.push_back(l2);
			rowc++;
			// cout << "\n\n\n";
		}
		
	}
	else{
		cout<<"file could not be opened.\n";
	}

	// Lets first initialize the probabilities by replacing -1 by "1/n"
	for (list<Graph_Node>::iterator it = Alarm.Pres_Graph.begin(); it != Alarm.Pres_Graph.end(); ++it) {
        int s = (*it).get_CPT().size();
		vector<float> new_CPT;
		for (int i=0; i<s; i++) {
			new_CPT.push_back(1.0/(float)s);
		}
		(*it).set_CPT(new_CPT);
    }

	// Running expectation minimization (EM)
	for (int i=0; i<3; i++){
		time_t curr_time;
		time(&curr_time);
		// find (?) by evaluating expectation of each possible decrete value according to the 'latest' BN trained

		map<int, vector<float> > record_p; //maps j,kth entry (for Var X) to {P(X = x1), P(X=x2), ..} vector of probabilities
		
		// evaluate through all the data in the records
		
		for (int j=0; j<records.size(); j++){
			
			for (int k=0; k<numVar; k++){
				// if found a (?) we need to allot it some discrete value
				if (unknown[j][k]) {
					Graph_Node cur_var = *Alarm.get_nth_node(k);
					vector<string> parents = (cur_var).get_Parents();
					vector<int> children = (cur_var).get_children();
					vector <Graph_Node> par_nodes;
					vector<int> par_pos;
					for(auto par: parents)
					{
						Graph_Node par_node = *(Alarm.search_node(par));
						par_nodes.push_back(par_node);
						par_pos.push_back(Alarm.get_index(par));
					}
					// we have found the markov blanket (mb) of (?) data, now calc prob.
					int numVal = cur_var.get_nvalues();
					vector<float> cur_prob;
					vector<float> cur_cpt = cur_var.get_CPT();
					
					float sum_prob = 0;
					for(int valPos=0; valPos < numVal; valPos++)
					{
						//P(x = valPos)
						//now we know which index to check inside cpt of cur_var for given row.
						
						int cpt_index = get_CPTindex(records[j], cur_var, valPos, par_nodes, par_pos);
						
						float prob = cur_cpt[cpt_index];
						
						//now multiply with each child's P(child | parents(child))
						for(int child: children)
						{
							Graph_Node child_node = *Alarm.get_nth_node(child);
							string childVal = records[j][child];
							int childValIndex = valueIndex(child_node, childVal);
							vector<string> child_pars = (child_node).get_Parents();
							vector<Graph_Node> child_par_nodes;
							vector<int> child_par_pos;
							vector<float> cur_child_cpt = child_node.get_CPT();
							for(auto childPar: child_pars)
							{
								Graph_Node par_node = *(Alarm.search_node(childPar));
								child_par_nodes.push_back(par_node);
								child_par_pos.push_back(Alarm.get_index(childPar));
							}
							int cpt_child_index = get_CPT_childindex(records[j], child_node, childValIndex, child_par_nodes, child_par_pos, k, valPos);
							
							
							prob*=cur_child_cpt[cpt_child_index];
							
						}
						cur_prob.push_back(prob);
						sum_prob+=prob;
						
						
					}
					//Now normalise cur_prob 
					
					for(int valPos = 0; valPos < numVal; valPos++)
					{
						if(sum_prob) cur_prob[valPos] /= sum_prob;
						else cur_prob[valPos] = ((float)1.0/(float)numVal);
						
					}
					record_p[j] = cur_prob;
					
				}
			}
		}
		// given all data, update the CPT
		
		vector<vector<string> > new_records;
		vector<float> weights;
		for(int row=0; row<records.size(); row++)
		{
			vector<string> temp;
			
			if(question_mark_index.count(row))
			{	
				int qmi = question_mark_index[row];
				
				Graph_Node missing = *Alarm.get_nth_node(qmi);
				int numval = missing.get_nvalues();
				vector<string> pos_vals = missing.get_values();
				for(int jj = 0; jj<numval; jj++)
				{
					temp.clear();
					for(int j = 0; j<numVar; j++)
					{
						if(j!=qmi) temp.push_back(records[row][j]);
						else temp.push_back(pos_vals[jj]);
					}
					float w = record_p[row][jj];
					new_records.push_back(temp);
					weights.push_back(w);
					
				}
			}
			else{
				for(int j=0; j<numVar; j++)
				{
					temp.push_back(records[row][j]);
				}
				new_records.push_back(temp);
				weights.push_back(1.0);
			}
			
		}
		

		for(int k=0; k<numVar; k++){
			Graph_Node cur = *(Alarm.get_nth_node(k));
			int numVal = cur.get_nvalues();
			vector<float> new_CPT = cur.get_CPT();
			vector<string> cur_parents = cur.get_Parents();
			vector<int> cur_parents_index;
			cur_parents_index.push_back(k);
			for (auto par : cur_parents) cur_parents_index.push_back(Alarm.get_index(par));

			unordered_map<int, vector<string> > par_values_map;
			par_values_map[k] = cur.get_values();
			for (auto par : cur_parents) {
				par_values_map[Alarm.get_index(par)] = (*Alarm.search_node(par)).get_values();
			}

			vector<float> numerator(new_CPT.size(),0.0);
			vector<float> denom(new_CPT.size(), 0.0);
			
			
			
			for (int row = 0; row< new_records.size(); row++){
				vector<string> data = new_records[row];
				int pos = 0;
				int multiplier = 1;
				int iter = 0;
				float weight = weights[row];
				
				
				reverse(cur_parents_index.begin(), cur_parents_index.end());
				for (auto index : cur_parents_index) {
					string value = data[index];
					vector<string> poss_val = par_values_map[index];
					int num = 0;
					for (int jj = 0; jj<poss_val.size(); jj++) {
						if (value==poss_val[jj]) {
							num = jj; break;
						}
					}
					if (iter==cur_parents_index.size()-1) {
						for (int jj = 0; jj<poss_val.size(); jj++) {
							denom[pos+jj*multiplier] += weight;
						}
					}
					pos += multiplier*num;
					multiplier *= poss_val.size();
					iter++;
					
				}
				reverse(cur_parents_index.begin(), cur_parents_index.end());
				numerator[pos] += weight;
				
			}
			for (int kk=0; kk<denom.size(); kk++){
				new_CPT[kk] = ((float)numerator[kk] + (0.01/(float)numVal))/((float)denom[kk]+0.01);
				if (new_CPT[kk]>=0.999) new_CPT[kk] = 0.99;
				if (new_CPT[kk]<=0.001) new_CPT[kk] = 0.01;

			}
			Alarm.get_nth_node(k)->set_CPT(new_CPT);
			
			// cout << "\n";
			auto current_time = std::chrono::high_resolution_clock::now();
			auto elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time).count();
			if(elapsed_seconds > 100){
				break;
			}

		}
	}

	int ans = write_ans(Alarm);
	cout<<"Perfect! Hurrah! \n";
	
}