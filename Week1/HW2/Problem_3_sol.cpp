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

double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
    int n1 = nums1.size();
    int n2 = nums2.size();
//    cout<<"size:"<<n1<<" "<<n2<<endl;

    if (n1 > n2)
        return findMedianSortedArrays(nums2, nums1);
    else if (n1==0){
        if(n2 % 2 == 1)
            return nums2[n2/2];
        else
            return ((nums2[n2/2]+nums2[(n2/2)-1])/2.0);
    }
    int n = n1 + n2;
    int partition = (n+1) / 2; // the position of meidan(if even, take right as median)
    int left = 0, right = n1;

    int l1, r1, l2, r2;

    // recursive
    while (left<=right) {
//        cout<<left<<right<<endl;
        int mid1 = (left + right) / 2;
        int mid2 = partition - mid1;
//        cout<<"mid1:"<<mid1<<" mid2:"<<mid2<<endl;
        if (mid1 - 1 >= 0)
            l1 = nums1[mid1 - 1];
        else
            l1 = INT_MIN;
        if(mid1 < n1)
            r1 = nums1[mid1];
        else
            r1 = INT_MAX;
        if (mid2 - 1 >= 0)
            l2 = nums2[mid2 - 1];
        else
            l2 = INT_MIN;
        if(mid2 < n2)
            r2 = nums2[mid2];
        else
            r2 = INT_MAX;
        
//        cout<<l1<<" "<<r1<<endl;
//        cout<<l2<<" "<<r2<<endl;
        if (l1 <= r2 &&
            l2 <= r1 ) { // the partition is correct, we found the midian
            cout<<"found median:"<<endl;
            if (n % 2 == 1)
                return max(l1, l2);
            else
                return (double)(max(l1,l2)+min(r1,r2)) / 2.0;
        } else if (l1 > r2) { // mid1太大
            right = mid1 - 1; // median在左半部, search範圍設定在左半部left~mid1
        } else if (l2 > r1) { // mid1太小
            left = mid1 + 1; // median在右半部, search範圍設定在右半部mid1~right
        }
    }
    return 0;
};


int main(int argc, const char * argv[]) {
    //設定測資nums1, nums2
    vector<int> nums1({1,3});
    vector<int> nums2({2});
    cout<<findMedianSortedArrays(nums1, nums2)<<endl;
}
