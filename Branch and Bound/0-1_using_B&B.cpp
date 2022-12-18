//Branch and bound is very useful technique for searching a solution but in worst case, we need to fully calculate the entire tree. At best, we only need to fully calculate one path through the tree and prune the rest of it. 

#include <bits/stdc++.h>
using namespace std;

//Structure to store weight and value of an item
struct Item{
    float weight;
    int value;
};

//Structure to store information of decision tree
struct Node{
    //level --> level of nodes in decision tree

    //profit --> Profit of nodes in decision tree

    //bound --> Upper bound of maximum profit in subtree of that node
    int level, profit, bound;
    float weight;
};

//Comparison function to sort Item according to val/weight ratio

bool cmp(Item a, Item b){
    double r1 = (double)a.value/a.weight;
    double r2 = (double)b.value/b.weight;
    return r1 > r2;
}

int bound(Node u, int n, int W, Item arr[]){
    if(u.weight >= W)
        return 0;

    int profit_bound = u.profit;

    int j = u.level + 1;
    int totweight = u.weight;

    //Checking index condition and knapsack capacity condition
    while ((j < n) && (totweight + arr[j].weight <= W))
    {
        totweight    += arr[j].weight;
        profit_bound += arr[j].value;
        j++;
    }

    //If k is not n, include last item partially for upper bound on profit
    if (j < n)
        profit_bound += (W - totweight) * arr[j].value / arr[j].weight;

    return profit_bound;
}

//Returns maximum profit we can get with capacity W
int knapsack(int W, Item arr[], int n)
{
    //Sorting Item on basis of value per unit weight.
    sort(arr, arr + n, cmp);

    //Make a queue for traversing the node
    queue<Node> Q;
    Node u, v;
    //Dummy node at starting
    u.level = -1;
    u.profit = u.weight = 0;
    Q.push(u);
    //One by one extract an item from decision tree compute profit of all children of extracted item and keep saving maxProfit
    int maxProfit = 0;
    while (!Q.empty())
    {
        //Dequeue a node
        u = Q.front();
        Q.pop();
        //If it is starting node, assign level 0
        if (u.level == -1)
            v.level = 0;
        //If there is nothing on next level
        if (u.level == n-1)
            continue;
        //Else if not last node, then increment level, and compute profit of children nodes.
        v.level = u.level + 1;
        //Taking current level's item add current level's weight and value to node u's weight and value
        v.weight = u.weight + arr[v.level].weight;
        v.profit = u.profit + arr[v.level].value;
        //If cumulated weight is less than W and profit is greater than previous profit, update maxprofit
        if (v.weight <= W && v.profit > maxProfit)
            maxProfit = v.profit;
        //Get the upper bound on profit to decide whether to add v to Q or not.
        v.bound = bound(v, n, W, arr);

        //If bound value is greater than profit, then only push into queue for further consideration
        if (v.bound > maxProfit)
            Q.push(v);
        //Do the same thing, but without taking the item in knapsack
        v.weight = u.weight;
        v.profit = u.profit;
        v.bound = bound(v, n, W, arr);
        if (v.bound > maxProfit)
            Q.push(v);
    }

    return maxProfit;
}

int main()
{
    int W = 10;   // Weight of knapsack
    Item arr[] = {{2, 40}, {3.14, 50}, {1.98, 100}, {5, 95}, {3, 30}};
    int n = sizeof(arr) / sizeof(arr[0]);
    cout << "Maximum possible profit = " << knapsack(W, arr, n);
    return 0;
}

