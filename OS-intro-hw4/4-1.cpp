#include <bits/stdc++.h>

using namespace std;

int main(){
    int n, sz, inp;
    int ans = 0;

    bool chk;
    cin >> n >> sz;
    vector<int> v(n,-1);
    for (int i = 0; i < sz; i++){
        cin >> inp;
        chk = false;
        for(int j = 0; j < n; j++){
            if(inp == v[j]){
                chk = true;
                break;
            }
        }
        if(!chk){
            v[i%n] = inp;
            ans++;
        } 
    }
    cout << "Total Cache Misses:" << ans;
}