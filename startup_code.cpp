#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <set>
#include<bits/stdc++.h>

// Format checker just assumes you have Alarm.bif and Solved_Alarm.bif (your file) in current directory
using namespace std;

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
int get_CPTindex(vector<string> &row_record, Graph_Node var, int valIndex)
{
	vector<string> parents = (var).get_Parents();
	int numVal = var.get_nvalues();
	vector<int> value_indices;
	vector<int> totalValues;
	for(auto par: parents)
	{
		int par_pos = Alarm.get_index(par); //this is the position in Alarm.bif
		Graph_Node par_node = *Alarm.search_node(par);
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

int main()
{
	network Alarm;
	Alarm=read_network();
    
// Example: to do something
	int numVar = Alarm.netSize();
	ifstream myfile("records.dat");
	string line;
	string temp;
	vector<vector<string> > records; // records is a matrix of data
	vector<vector<bool> > unknown; // unknowns is a matrix, if ele==true then corresponsinf data is unknown (?)
	if (myfile.is_open()) {
		while (! myfile.eof() ){
			stringstream ss;
			vector<string> l1;
			vector<bool> l2;
      		getline (myfile,line);
      		ss.str(line);
      		while (ss>>temp) {
				// cout << temp << " " ;
				l1.push_back(temp);
				if (temp == "?") l2.push_back(true);
				else l2.push_back(false);
			}
			records.push_back(l1);
			unknown.push_back(l2);
			// cout << "\n\n\n";
		}
		
	}

	// Lets first initialize the probabilities by replacing -1 by "1/n"
	for (list<Graph_Node>::iterator it = Alarm.Pres_Graph.begin(); it != Alarm.Pres_Graph.end(); ++it) {
        int s = (*it).get_CPT().size();
		vector<float> new_CPT;
		for (int i=0; i++; i<s) {
			new_CPT.push_back(1/s);
		}
		(*it).set_CPT(new_CPT);
    }

	// Running expectation minimization (EM)
	for (int i=0; i<1; i++){
		// find (?) by evaluating expectation of each possible decrete value according to the 'latest' BN trained

		map<pair<int,int>, vector<double>> record_p; //maps j,kth entry (for Var X) to {P(X = x1), P(X=x2), ..} vector of probabilities

		// evaluate through all the data in the records
		for (int j=0; j++; j<records.size()){
			for (int k=0; k++; k<numVar){
				// if found a (?) we need to allot it some discrete value
				if (unknown[j][k]) {
					// find the markov blanket
					Graph_Node cur_var = Alarm.get_nth_node(k);
					vector<string> parents = (cur_var).get_Parents();
					vector<int> children = (*it).get_children();
					set<int> mb_par, mb_chil;
					for (int x=0; x++; x<parents.size()){
						mb_par.insert(Alarm.get_index(parents[x]));
					}
					for (int x=0; x++; x<children.size()){
						list<Graph_Node>::iterator child = Alarm.get_nth_node(children[x]);
						mb_chil.insert(children[i]);
					}
					// we have found the markov blanket (mb) of (?) data, now calc prob.
					int numVal = cur_var.get_nvalues();
					vector<double> cur_prob;
					vector<float> cur_cpt = cur_var.get_CPT();
					double sum_prob = 0;
					for(int valPos=0; valPos < numVal; valPos++)
					{
						//P(x = valPos)
						int cpt_index = get_CPTindex(records[j], cur_var, valPos);
						//now we know which index to check inside cpt of cur_var for given row.
						double prob = cur_cpt[cpt_index];
						//now multiply with each child's P(child | parents(child))
						for(int child: children)
						{
							Graph_Node child_node = Alarm.get_nth_node(child);
							string childVal = records[j][child];
							int childValIndex = valueIndex(child_node, childVal);
							int cpt_child_index = get_CPTindex(records[j], child_node, childValIndex);
							prob*=cur_cpt[cpt_child_index];
						}
						cur_prob.push_back(prob);
						sum_prob+=prob;
					}
					//Now normalise cur_prob 
					for(int valPos = 0; valPos < numVal; valPos++)
					{
						cur_prob[valPos] /= sum_prob;
					}
					//update these expected values of kth variable in whichever data structure we will be using in M of EM
				}
			}
		}



		// given all data, update the CPT
	}


	cout<<"Perfect! Hurrah! \n";
	
}
