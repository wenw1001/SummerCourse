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

vector<int> searchRange(vector<int>& nums, int target) {
    int n = nums.size();
    if(n==0)
        return{-1,-1};
    int cut,firstPos=-1, lastPos=-1;
    int left=0,right=n-1;
    // find nearest maximum position of first position(最接近first position的position, cut小於target去找右半，大於等於去找左半)
    while(left<=right){
//        cout<<"Range:"<<left<<"~"<<right<<endl;
        cut=(left+right+1)/2;
//        cout<<"cut="<<cut<<", value= "<<nums[cut]<<endl;
        if(nums[cut]>=target){// 切的數字比target大，first position在左邊(要<target,因為non-decreasing)，search左半部
            right=cut-1;
        }
        else{
            left=cut+1;
        }
    }
//    cout<<"first cut pos: "<<cut<<" value="<<nums[cut]<<endl;
    if(nums[cut]==target)
        firstPos=cut;
    else{
        firstPos=cut+1;
        if(firstPos>n-1){
            return {-1,-1};
        }
    }
    
    // find nearest minimum position of last position(最接近last position的position, cut大於target去找左半，小於等於去找右半(因為等於target代表last position在這或在右半邊))
    left=0,right=n-1;
    while(left<=right){
//        cout<<"Range:"<<left<<"~"<<right<<endl;
        cut=(left+right+1)/2;
//        cout<<"cut="<<cut<<", value= "<<nums[cut]<<endl;
        if(nums[cut]>target){// 切的數字比target大，last position在左邊(要>=target)，search左半部
            right=cut-1;
        }
        else{
            left=cut+1;
        }
    }
//    cout<<"last cut pos: "<<cut<<" value="<<nums[cut]<<endl;
    if(nums[cut]==target)
        lastPos=cut;
    else{
        lastPos=cut-1;
        if(lastPos<0){
            return {-1,-1};
        }
    }

    if(nums[firstPos]==target && nums[lastPos]==target){
        return {firstPos,lastPos};
    }
    return {-1,-1};
};


int main(int argc, const char * argv[]) {
    //{}, target=0-->[-1,-1]
    //{1}, target=0-->[-1,-1]
    //{1,2,3}, target=1-->[0,0]
    vector<int> nums1({5,7,7,8,8,10});
    int target = 8;
    
    vector<int> result=searchRange(nums1, target);
    cout<<"["<<result[0]<<","<<result[1]<<"]"<<endl;
}
