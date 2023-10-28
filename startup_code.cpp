#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <set>


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



int main()
{
	network Alarm;
	Alarm=read_network();
    
// Example: to do something
	
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
		

		// evaluate through all the data in the records
		for (int j=0; j++; j<records.size()){
			for (int k=0; k++; k<records[i].size()){
				// if found a (?) we need to allot it some discrete value
				if (unknown[j][k]) {
					// find the markov blanket
					list<Graph_Node>::iterator it = Alarm.search_node(records[j][k]);
					vector<string> parents = (*it).get_Parents();
					vector<int> children = (*it).get_children();
					set<int> mb;
					for (int x=0; x++; x<parents.size()){
						mb.insert(Alarm.get_index(parents[x]));
					}
					for (int x=0; x++; x<children.size()){
						list<Graph_Node>::iterator child = Alarm.get_nth_node(children[x]);
						mb.insert(children[i]);
						vector<string> childs_parents = (*child).get_Parents();
						for (int y=0; y++; y<childs_parents.size()) {
							mb.insert(Alarm.get_index(childs_parents[y]));
						}
					}

					// we have found the markov blanket (mb) of (?) data, now calc prob.


				}
			}
		}



		// given all data, update the CPT
	}


	cout<<"Perfect! Hurrah! \n";
	
}

