#include <bits/stdc++.h>

using namespace std;

int main(){
    int n, way, sz, inp;
    int idx, mn;
    int ans = 0;

    bool chk;
    cin >> n >> way >> sz;    
    int num_sets = n/way;
    vector<pair<int,int> > v(n,make_pair(-1,0));
    for (int i = 0; i < sz; i++){
        cin >> inp;
        chk = false;
        for(int j = 0; j < n; j++){
            if(inp == v[j].first){
                chk = true;
                v[j].second++;
                break;
            }
        }
        if(!chk){ //replace
            int start = (i%num_sets)*way;
            idx=start; mn=INT_MAX;
            for (int j = 0; j < way; j++) {
                if(v[start+j].second < mn){
                    idx = start+j;
                    mn = v[start+j].second;
                }
            }
            v[idx].first = inp;
            v[idx].second = 1;
            ans++;
        }
    }
    cout << "Total Cache Misses:" << ans;
}