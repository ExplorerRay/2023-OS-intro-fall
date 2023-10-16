#include<bits/stdc++.h>

using namespace std;

struct proc{
    int id;
    int arr_tm;
    int bst_tm;
    //wait time = turnaround time - burst time - arrival time
    int ta_tm=0; //finish job timing
    int now_que;
};

bool cmpArr(proc p1, proc p2){
    if(p1.arr_tm != p2.arr_tm)
        return p1.arr_tm > p2.arr_tm;
    return p1.id < p2.id;
}
bool cmpBst(proc p1, proc p2){
    if(p1.bst_tm != p2.bst_tm)
        return p1.bst_tm > p2.bst_tm;
    else if(p1.arr_tm != p2.arr_tm)
        return p1.arr_tm > p2.arr_tm;
    return p1.id < p2.id;
}

int main(){
    int n,m;
    int algo,tq;
    int arr,bst;
    proc new_proc;

    vector<pair<int,int>> algo_tq;
    vector<proc> rcd_prc;

    cin >> n >> m;
    for(int q=0;q<n;q++){
        cin >> algo >> tq;
        algo_tq.emplace_back(make_pair(algo,tq));
    }
    vector<vector<proc>> que(n);

    for(int i=0;i<m;i++){
        cin >> arr >> bst;
        new_proc.id = i;
        new_proc.arr_tm = arr;
        new_proc.bst_tm = bst;
        new_proc.now_que = 0;
        rcd_prc.emplace_back(new_proc);
    }

    if(algo_tq[0].first==1) sort(que[0].begin(), que[0].end(), cmpBst);
    else sort(que[0].begin(), que[0].end(), cmpArr);

    proc *run_proc=new proc, *last_prc=new proc;
    last_prc->id = -1;
    int added_cnt=0, processed_cnt = 0, current_tm = 0, tq_rcd=0;
    bool run, tqr;
    while (processed_cnt < m){
        for(int p=added_cnt;p<m;p++){
            if(rcd_prc[p].arr_tm <= current_tm && rcd_prc[p].bst_tm > 0 ){
                que[0].emplace_back(rcd_prc[p]);
                added_cnt++;
            }
        }

        if(algo_tq[0].first==1) sort(que[0].begin(), que[0].end(), cmpBst);
        else sort(que[0].begin(), que[0].end(), cmpArr);

        current_tm++;
        run=false;tqr=false;
        for(int pcq=0;pcq<n;pcq++){
            while(!que[pcq].empty()){
                run_proc = &que[pcq].back();

                // re-emplace preempted proc
                if(last_prc->id != run_proc->id && last_prc->id != -1 && last_prc->bst_tm > 0){
                    last_prc->arr_tm = current_tm;

                    if(pcq==last_prc->now_que){
                        que[pcq].pop_back();
                        que[pcq].pop_back();
                        que[pcq].emplace_back(*run_proc);
                    }
                    else{
                        que[last_prc->now_que].pop_back();
                    }

                    if(last_prc->now_que!=n-1){
                        last_prc->now_que++;
                    }
                    que[last_prc->now_que].emplace_back(*last_prc);

                    if(algo_tq[last_prc->now_que].first==1) sort(que[last_prc->now_que].begin(), que[last_prc->now_que].end(), cmpBst);
                    else sort(que[last_prc->now_que].begin(), que[last_prc->now_que].end(), cmpArr);
                }

                cout << "running: " << run_proc->id << '\n';
                if(algo_tq[pcq].first == 0){ // FCFS
                    run_proc->bst_tm--;
                    if(run_proc->bst_tm == 0){
                        rcd_prc[run_proc->id].ta_tm = current_tm;
                        processed_cnt++;
                        tq_rcd=0;
                        que[pcq].pop_back();
                    }

                    if(algo_tq[pcq].first==1) sort(que[pcq].begin(), que[pcq].end(), cmpBst);
                    else sort(que[pcq].begin(), que[pcq].end(), cmpArr);
                    run=true;
                    break;
                }
                else if(algo_tq[pcq].first == 1){ // SRFS
                    run_proc->bst_tm--;
                    if(run_proc->bst_tm == 0){
                        rcd_prc[run_proc->id].ta_tm = current_tm;
                        processed_cnt++;
                        tq_rcd=0;
                        que[pcq].pop_back();
                    }
                    run=true;
                    break;
                }
                else{ // round robin
                    run_proc->bst_tm--;
                    tq_rcd++;
                    if(run_proc->bst_tm == 0){
                        rcd_prc[run_proc->id].ta_tm = current_tm;
                        processed_cnt++;
                        tq_rcd=0;
                        que[pcq].pop_back();
                    }
                    if(tq_rcd == algo_tq[pcq].second){ //time quantum reached
                        que[pcq].pop_back();
                        if(run_proc->bst_tm>0){
                            run_proc->arr_tm = current_tm;
                            if(pcq != n-1){ // move process to next queue
                                run_proc->now_que++;
                                que[pcq+1].emplace_back(*run_proc);

                                if(algo_tq[pcq+1].first==1) sort(que[pcq+1].begin(), que[pcq+1].end(), cmpBst);
                                else sort(que[pcq+1].begin(), que[pcq+1].end(), cmpArr);
                            }
                            else{
                                que[pcq].emplace_back(*run_proc);

                                if(algo_tq[pcq].first==1) sort(que[pcq].begin(), que[pcq].end(), cmpBst);
                                else sort(que[pcq].begin(), que[pcq].end(), cmpArr);
                            }
                        }
                        else{
                            rcd_prc[run_proc->id].ta_tm = current_tm;
                            processed_cnt++;
                        }
                        tq_rcd=0;
                        tqr=true;
                    }
                    run=true;
                    break;
                }
                // }
            }
            if(run)break;
        }
        if(tqr) last_prc->id=-1;
        else last_prc->id = run_proc->id;
        last_prc->arr_tm = run_proc->arr_tm;
        last_prc->bst_tm = run_proc->bst_tm;
        last_prc->ta_tm = run_proc->ta_tm;
        last_prc->now_que = run_proc->now_que;
    }

    int tt_wt=0, tt_ta=0;
    proc prc;
    for(int p=0;p<m;p++){
        prc=rcd_prc[p];
        cout << prc.ta_tm-prc.bst_tm-prc.arr_tm << ' ' << prc.ta_tm-prc.arr_tm << '\n';
        tt_wt+=prc.ta_tm-prc.bst_tm-prc.arr_tm;
        tt_ta+=prc.ta_tm-prc.arr_tm;
    }
    cout << tt_wt << '\n' << tt_ta << '\n';
}
