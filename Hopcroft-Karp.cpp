#include <vector>
#include <queue>

using namespace std;

const int INF = 0x60000000;

struct Hopcroft_Karp
{
	int Un, Vn;
	vector <int> dist, matchU, matchV;
	vector < vector <int> > adj;

	Hopcroft_Karp(int _Un, int _Vn):Un(_Un),Vn(_Vn)
	{
		dist.resize(Un + 1, 0);
		matchU.resize(Un + 1, 0);
		matchV.resize(Vn + 1, 0);
		adj.resize(Un + 1);
	}

	bool bfs()
	{
		queue <int> qu;
		for (int i = 1; i <= Un; i++)
		{
			if (matchU[i] == 0)
			{
				dist[i] = 0;
				qu.push(i);
			}
			else dist[i] = INF;
		}

		dist[0] = INF;

		while (!qu.empty())
		{
			int u = qu.front();
			qu.pop();

			if (dist[u] < dist[0])
			{
				for (int i = 0; i < adj[u].size(); i++)
				{
					int v = adj[u][i];
					if (dist[matchV[v]] == INF)
					{
						dist[matchV[v]] = dist[u] + 1;
						qu.push(matchV[v]);
					}
				}
			}
		}

		return dist[0] != INF;
	}

	bool dfs(int u)
	{
		if (u == 0)return true;
		
		for (int i = 0; i < adj[u].size(); i++)
		{
			int v = adj[u][i];

			if (dist[matchV[v]] == dist[u] + 1)
			{
				if (dfs(matchV[v]))
				{
					matchV[v] = u;
					matchU[u] = v;
					return true;
				}
			}
		}

		dist[u] = INF;
		return false;
	}

	int matching()
	{
		int ret = 0;

		while (bfs())
		{
			for (int i = 1; i <= Un; i++)
			{
				if (!matchU[i] && dfs(i))
				{
					ret++;
				}
			}
		}

		return ret;
	}
};