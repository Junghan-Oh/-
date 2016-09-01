#include <cstdio>
#include <queue>
#include <vector>
#include <algorithm>

using namespace std;

struct Network
{
	int n;

	struct edge
	{
		int to, rev, res;
	};

	vector < vector <edge> > adj;

	Network(int _n)
	{
		n = _n;
		adj.resize(n + 1);
	}

	void addEdge(int s, int e, int _cap, int _cap_rev = 0)
	{
		edge forward{ e, adj[e].size(), _cap };
		edge reverse{ s, adj[s].size(), _cap_rev };

		adj[s].push_back(forward);
		adj[e].push_back(reverse);
	}
};

struct Dinic
{
	int src, sink;
	int n;
	int maxFlow;
	vector <int> lev;
	vector <int> start;
	Network & N;

	Dinic(Network & _N, int _src, int _sink):N(_N), src(_src), sink(_sink)
	{
		maxFlow = 0;
		int n = N.n;
		lev.resize(n + 1);
		start.resize(n + 1);
		Start();
	}

	bool bfs()
	{
		for (int i = 0; i <= sink; i++)lev[i] = 0;
		queue <int> qu;
		qu.push(src);
		lev[src] = 1;

		while (!qu.empty() && !lev[sink])
		{
			int u = qu.front();
			qu.pop();

			for (int i = 0; i < N.adj[u].size(); i++)
			{
				int v = N.adj[u][i].to;

				if (lev[v])continue;
				if (N.adj[u][i].res > 0)
				{
					lev[v] = lev[u] + 1;
					qu.push(v);
				}
			}
		}

		return lev[sink] != 0;
	}

	int dfs(int u, int flow)
	{
		if (u == sink)return flow;

		for (int & i = start[u]; i < N.adj[u].size(); i++)
		{
			int v = N.adj[u][i].to;

			if (lev[v] != lev[u] + 1)continue;
			if (N.adj[u][i].res <= 0)continue;

			if (int ret = dfs(v, min(flow, N.adj[u][i].res)))
			{
				N.adj[u][i].res -= ret;
				N.adj[v][N.adj[u][i].rev].res += ret;

				return ret;
			}
		}

		return 0;
	}

	void Start()
	{
		while (bfs())
		{
			for (int i = 0; i <= n; i++)start[i] = 0;
			while (int d = dfs(src, INF))
			{
				maxFlow += d;
			}
		}
	}
};