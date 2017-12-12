#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <queue>
#include <map>

using namespace std;

class Link;
class Graph
{

public:

	Graph(int nodes, int links, int gateways);

	void SetLink(int index, int node1, int node2);
	void SetGateway(int index, int gateway);

	const Link& GetLink(int index);

	Link SeverLinkToClosestGateWay(int skynetIndex);

private:

	bool IsGateway(int node);

	Link TraverseToClosestGateway(int start);

	std::vector<int> GetNeighbours(int node);

private:

	int nodes;
	std::vector<Link> links;
	std::vector<int> gateways;

};

class Link
{

public:

	Link();
	Link(int node1, int node2);

	void Set(int node1, int node2);
	bool Contains(int node);
	int GetEnd(int otherEnd);

public:

	int node1, node2;

};

class LinkComparer
{

public:

	LinkComparer(int node1, int node2);

	bool operator() (const Link& link) const;

private:

	int node1, node2;

};


void print(std::queue<int> q);
/**
* Auto-generated code below aims at helping you parse
* the standard input according to the problem statement.
**/
int main()
{
	int nodes, links, gateways; // the number of exit gateways
	cin >> nodes >> links >> gateways; cin.ignore();

	Graph* graph = new Graph(nodes, links, gateways);

	for (int i = 0; i < links; i++) {
		int node1; // N1 and N2 defines a link between these nodes
		int node2;
		cin >> node1 >> node2; cin.ignore();
		graph->SetLink(i, node1, node2);
	}
	for (int i = 0; i < gateways; i++) {
		int gateway; // the index of a gateway node
		cin >> gateway; cin.ignore();
		graph->SetGateway(i, gateway);
	}

	// game loop
	while (1) {
		int skynetIndex; // The index of the node on which the Skynet agent is positioned this turn
		cin >> skynetIndex; cin.ignore();

		// Write an action using cout. DON'T FORGET THE "<< endl"
		// To debug: cerr << "Debug messages..." << endl;
		Link severedLink = graph->SeverLinkToClosestGateWay(skynetIndex);

		// Example: 0 1 are the indices of the nodes you wish to sever the link between
		cout << severedLink.node1 << " " << severedLink.node2 << endl;
	}
}


//-------------
// Graph class implementation
Graph::Graph(int nodes, int links, int gateways) : nodes(nodes), links(links), gateways(gateways) { }


void Graph::SetLink(int index, int node1, int node2)
{
	if (index > links.size())
		return;

	links[index].Set(node1, node2);
}

void Graph::SetGateway(int index, int gateway)
{
	if (index > gateways.size())
		return;

	gateways[index] = gateway;
}


const Link& Graph::GetLink(int index)
{
	return links[index];
}


bool Graph::IsGateway(int node)
{
	return (std::find(gateways.begin(), gateways.end(), node) != gateways.end());
}


Link Graph::TraverseToClosestGateway(int start)
{
	//  # a FIFO open_set
	//	open_set = Queue()
	//	open_set.enqueue(start)
	std::queue<int> openSet;
	openSet.push(start);

	//	# an empty set to maintain visited nodes
	//	closed_set = set()
	std::vector<int> closeSet;

	//	# a dictionary to maintain meta information(used for path formation)
	//	meta = dict()  # key -> (parent state, action to reach child)
	//	meta[start] = (None, None)
	std::map<int, int> meta;

	//	# initialize
	//	start = problem.get_start_state()

	int currentNode;
	//	while not open_set.is_empty() :
	while (!openSet.empty())
	{
		currentNode = openSet.front();
		openSet.pop();

		//		if problem.is_goal(parent_state) :
		//			return construct_path(parent_state, meta)
		cerr << "[Debug]: IsGateway(" << currentNode << ") = " << IsGateway(currentNode) << endl;
		if (IsGateway(currentNode))
			return Link(currentNode, meta[currentNode]);

		//		for (child_state, action) in problem.get_successors(parent_state) :
		std::vector<int> neighbors = GetNeighbours(currentNode);
		for (int i = 0; i < neighbors.size(); i++)
		{
			if (IsGateway(neighbors[i]))
				return Link(currentNode, neighbors[i]);

			//			if child_state in closed_set :
			//				continue
			if (std::find(closeSet.begin(), closeSet.end(), neighbors[i]) != closeSet.end())
				continue;

			//			if child_state not in open_set :
			//				meta[child_state] = (parent_state, action)
			//				open_set.enqueue(child_state)
			meta[neighbors[i]] = currentNode;
			cerr << "[Debug]: neighbor = " << neighbors[i] << ", parent: " << currentNode << endl;
			openSet.push(neighbors[i]);
			cerr << "[Debug]: openSet.push " << neighbors[i] << ", content: ";
			print(openSet);
		}

		//		closed_set.add(parent_state)
		closeSet.push_back(currentNode);
	}

	return Link();
}


std::vector<int> Graph::GetNeighbours(int node)
{
	std::vector<int> neighbors;
	for (int i = 0; i < links.size(); i++)
	{
		if (links[i].Contains(node))
			neighbors.push_back(links[i].GetEnd(node));
	}

	return neighbors;
}

Link Graph::SeverLinkToClosestGateWay(int skynetIndex)
{
	Link linkToSever = TraverseToClosestGateway(skynetIndex);

	auto linkIterator = std::find_if(links.begin(), links.end(), LinkComparer(linkToSever.node1, linkToSever.node2));
	links.erase(linkIterator);

	return linkToSever;
}



//-------------
// Link class implementation
Link::Link(int node1, int node2) : node1(node1), node2(node2) { }


Link::Link() : Link(0, 0) { }


void Link::Set(int node1, int node2)
{
	this->node1 = node1;
	this->node2 = node2;
}


bool Link::Contains(int node)
{
	return node1 == node || node2 == node;
}

int Link::GetEnd(int otherEnd)
{
	if (otherEnd == node2)
		return node1;

	if (otherEnd == node1)
		return node2;

	return -1;
}



//-------------
// LinkFinder class implementation
LinkComparer::LinkComparer(int node1, int node2) : node1(node1), node2(node2) { }

bool LinkComparer::operator() (const Link& link) const
{
	return (link.node1 == node1 && link.node2 == node2) || (link.node2 == node1 && link.node1 == node2);
}


//---
// debug
void print(std::queue<int> q)
{
	while (!q.empty())
	{
		cerr << q.front() << " ";
		q.pop();
	}
	cerr << std::endl;
}