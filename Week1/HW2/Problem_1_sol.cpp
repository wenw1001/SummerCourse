//
//  main.cpp
//  LeetCode
//
//  Created by 文文林 on 2022/8/14.
//

#include <iostream>
#include <string>
#include <vector>
using namespace std;


int gets(string s, int len, int t){
    if (t<len && t>=0)
        return s[t];
    else
        return INT_MAX?t>len:INT_MIN;
};
int compare(string s,int len, int prePos, int nextPos, string *ans){
    char pre = gets(s,len,prePos);
    char next = gets(s, len,nextPos);
    // cout<<"comparing... "<<pre<<" && "<<next<<endl;
    if(pre==next && pre!=NULL){// 左右字相同
    // cout<<"compared! "<<pre<<" == "<<next<<" (ans update!)"<<endl;
        *ans = pre + *ans + next;
        // cout<<"compared! "<<pre<<" == "<<next<<", ans="<<*ans<<" (ans update!)"<<endl;
        return 2 + compare(s, len, prePos-1, nextPos+1, ans);
    }
    else // 左右字不同，結束
        return 0;
};

string longestPalindrome(string s) {
    string ans;
    int len = s.size();
    int count, max_count=0;
    for (int c=0; c<len; c++){//c = current index
    // cout<<"c= "<<c<<" ("<<s[c]<<")"<<endl;
        count=0;
        string tmp_ans="";
        if(c-1>0 && s[c]==s[c-1]){// 和前一個相同(ans length為偶數個)，繼續左右往外比較
            // cout<<"和前一個相同"<<endl;
            tmp_ans = string(1, s[c-1])+s[c];
            // cout<<"tmp_ans= "<<tmp_ans<<endl;
            // cout<<"type:"<<s[c]<<", "<<s[c+1]<<", (string)"<<string(s[c])<<endl;
            count = 2 + compare(s, len, c-2, c+1, &tmp_ans);
            // cout<<"最長substring: "<<tmp_ans<<endl;
            if(count>max_count){
                max_count=count;
                ans = tmp_ans;
            }
        }
        if (c+1<len && s[c]==s[c+1]){// 和下一個相同(ans length為偶數個)，繼續左右往外比較
            // cout<<"和後一個相同"<<endl;
            tmp_ans = string(1, s[c])+s[c+1];
            count = 2 + compare(s, len, c-1, c+2, &tmp_ans);
            // cout<<"最長substring: "<<tmp_ans<<endl;
            if(count>max_count){
                max_count=count;
                ans = tmp_ans;
            }
        }
        if(gets(s, len, c-1) == gets(s, len, c+1)){// 左右一樣
            // cout<<" 左右相同"<<endl;
            tmp_ans = s[c];
            count = 1 + compare(s, len, c-1, c+1, &tmp_ans);
            // cout<<"最長substring: "<<tmp_ans<<endl;
            if(count>max_count){
                max_count=count;
                ans = tmp_ans;
            }
        }
        else{// 單獨一個字元（和左右皆不同）
            // cout<<" 左右皆不同"<<endl;
            count=1;
            if(count>max_count){
                max_count=count;
                ans = s[c];
            }
        }
    }
    return ans;
};

int main(int argc, const char * argv[]) {
    // "assdssd"-->"ssdss"
    // "bababba"-->"babab"
    // "poiei"-->"iei"
    // "acca"-->"acca"
    string s="babad";
    cout<<longestPalindrome(s)<<endl;
}
