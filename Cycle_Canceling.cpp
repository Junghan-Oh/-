#include <vector>

using namespace std;

struct Network
{
	int n;

	struct edge
	{
		int to, rev, cost, cap;
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

struct CycleCanceling
{
	int maxCost;
	int maxFlow;
	int n;
	Network &nw;
	vector <int> dist;
	vector <bool> chk;
	vector < vector <int> > prev;
	int st;

	CycleCanceling(Network &_network) :nw(_network), maxCost(0), maxFlow(0)
	{
		n = nw.n;
		dist.resize(n + 1);
		chk.resize(n + 1);
		prev.resize(n + 1, vector <int>(2));
		start();
	}

	bool dfsSpfa(int u)
	{
		chk[u] = true;
		for (int i = 0; i < nw.adj[u].size(); i++)
		{
			if (nw.adj[u][i].cap)
			{
				int v = nw.adj[u][i].to;
				int nc = dist[u] + nw.adj[u][i].cost;

				if (nc > dist[v])
				{
					dist[v] = nc;
					prev[v][0] = u;
					prev[v][1] = i;

					if (chk[v])
					{
						st = v;
						return true;
					}
					else if (dfsSpfa(v))return true;
				}
			}
		}

		chk[u] = false;
		return false;
	}

	bool findPositiveCycle()
	{
		for (int i = 0; i <= n; i++)
		{
			dist[i] = 0;
			prev[i][0] = prev[i][1] = -1;
			chk[i] = false;
		}

		for (int i = 1; i <= n; i++)if (dfsSpfa(i))return true;

		return false;
	}

	void cancelCycle()
	{
		int at = st;

		while (1)
		{
			int node = prev[at][0];
			int en = prev[at][1];

			nw.adj[node][en].cap--;
			nw.adj[at][nw.adj[node][en].rev].cap++;

			maxCost += nw.adj[node][en].cost;

			at = node;
			if (at == st)break;
		}
	}

	void start()
	{
		while (findPositiveCycle())
		{
			cancelCycle();
			maxFlow++;
		}
	}
};