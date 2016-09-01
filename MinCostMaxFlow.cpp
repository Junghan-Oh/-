#include <vector>
#include <queue>

using namespace std;

struct Network
{
	int n;

	struct edge
	{
		int to, rev, cost, res;
	};

	vector < vector <edge> > adj;

	Network(int _n)
	{
		n = _n;
		adj.resize(n + 1);
	}

	void addEdge(int s, int e, int _cost, int _cap, int _cap_rev = 0)
	{
		edge forward{ e, adj[e].size(), _cost, _cap };
		edge reverse{ s, adj[s].size(), -_cost, _cap_rev };

		adj[s].push_back(forward);
		adj[e].push_back(reverse);
	}
};

struct MinCostMaxFlow
{
	int nVer_Max;
	int src;
	int sink;
	int minCost;
	int maxFlow;
	Network & N;

	MinCostMaxFlow(Network & _N, int _src, int _sink):N(_N),src(_src),sink(_sink)
	{
		nVer_Max = _N.adj.size();
		minCost = 0;
		maxFlow = 0;
		start();
	}

	void start()
	{
		while (1)
		{
			vector < vector <int> > prev(nVer_Max, vector <int> (2, -1) );
			vector <int> dist(nVer_Max, 0x30000000);
			vector <bool> inQueue(nVer_Max, false);
			queue <int> qu;
			qu.push(src);
			dist[src] = 0;

			while (!qu.empty())
			{
				int u = qu.front();
				qu.pop();
				inQueue[u] = false;

				for (int i = 0; i < N.adj[u].size(); i++)
				{
					int v = N.adj[u][i].to;

					if (!N.adj[u][i].res)continue;

					if (dist[v] > dist[u] + N.adj[u][i].cost)
					{
						dist[v] = dist[u] + N.adj[u][i].cost;
						prev[v][0] = u;
						prev[v][1] = i;

						if (!inQueue[v])
						{
							inQueue[v] = true;
							qu.push(v);
						}
					}
				}
			}

			if (prev[sink][0] == -1)break;

			for (int u = sink; u != src; u = prev[u][0])
			{
				int prevNode = prev[u][0];
				int prevEdge = prev[u][1];
				int rev = N.adj[prevNode][prevEdge].rev;

				int t = -1;
				if (N.adj[prevNode][prevEdge].res > 0)t = 1;

				N.adj[prevNode][prevEdge].res -= t;
				N.adj[u][rev].res -= t;
			}

			minCost += dist[sink];
			maxFlow++;
		}
	}
};