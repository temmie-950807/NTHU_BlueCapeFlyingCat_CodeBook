struct MCMF {
	struct Edge {
		int u, rc, k, rv;
	};
 
	vector<vector<Edge>> G;
	vector<int> par, par_eid;
	MCMF(int n) : G(n+1), par(n+1), par_eid(n+1) {}
 
 	// v->u, capcity: c, cost: k
	void add(int v, int u, int c, int k){
		G[v].push_back({u, c, k, (int)G[u].size()});
		G[u].push_back({v, 0, -k, (int)G[v].size()-1});
	}
 
	// 6d1140
	int spfa(int s, int t){
		fill(par.begin(), par.end(), -1);
		vector<int> dis(par.size(), INF);
		vector<bool> in_q(par.size(), false);
		queue<int> Q;
		dis[s] = 0;
		in_q[s] = true;
		Q.push(s);
 
		while (!Q.empty()){
			int v = Q.front();
			Q.pop();
			in_q[v] = false;
 
			for (int i=0 ; i<G[v].size() ; i++){
				auto [u, rc, k, rv] = G[v][i];
				if (rc>0 && dis[v]+k<dis[u]){
					dis[u] = dis[v]+k;
					par[u] = v;
					par_eid[u] = i;
					if (!in_q[u]) Q.push(u);
					in_q[u] = true;
				}
			}
		}
 
		return dis[t];
	}
 
 	// return <max flow, min cost>, d7e7ad
	pair<int, int> flow(int s, int t){
		int fl = 0, cost = 0, d;
		while ((d = spfa(s, t))<INF){
			int cur = INF;
			for (int v=t ; v!=s ; v=par[v])
				cur = min(cur, G[par[v]][par_eid[v]].rc);
			fl += cur;
			cost += d*cur;
			for (int v=t ; v!=s ; v=par[v]){
				G[par[v]][par_eid[v]].rc -= cur;
				G[v][G[par[v]][par_eid[v]].rv].rc += cur;
			}
		}
		return {fl, cost};
	}
};
