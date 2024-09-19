#include <stdio.h>
#include <iostream>
#include <stack>
#include<cmath>
#include<fstream>
using namespace std;
typedef struct {
	double x;
	double y;
}Point;
typedef enum{
	X=-1, Y=1
}Type;
class KDNode{
	public:
		int x;
		int y;
		Type type;
		KDNode(double x, double y, Type type, KDNode *left=NULL, KDNode *right=NULL):x(x), y(y), type(type), left(left), right(right){}
		KDNode *left;
		KDNode *right;
};
ostream& operator <<(ostream& os, const Point& p){
	return os<<"("<<p.x<<","<<p.y<<")";
}
ostream& operator <<(ostream& os, const KDNode& n){
	return os<<"("<<n.x<<","<<n.y<<","<<n.type<<")";
}
void quick_sort(Point *array, int begin, int end, Type type){
	if(begin == end)return ;
	int i = begin-1, j = end+1;
	if(type == X){
		int mid = array[(begin + end)/2].x;
		while(i<j){
			do ++i; while(array[i].x<mid);
			do --j;while(array[j].x>mid);
			if(i<j){
				Point tmp = array[i];
				array[i] = array[j];
				array[j] = tmp;
			}	
		}
		quick_sort(array, begin, j, type);
		quick_sort(array, j+1, end,type);
	}else if(type==Y){
		int mid = array[(begin + end)/2].y;
		while(i<j){
			do ++i; while(array[i].y<mid);
			do --j;while(array[j].y>mid);
			if(i<j){
				Point tmp = array[i];
				array[i] = array[j];
				array[j] = tmp;
			}	
		}
		quick_sort(array, begin, j, type);
		quick_sort(array, j+1, end,type);
	}
}
//对数组的指定维度进行一次快速排序
Point median_point(Point *array, int begin, int end, Type type){
	quick_sort(array, begin, end, type);
	return array[(begin+end+1)/2];
}

//type为当前需要按x轴分割还是按y轴分割 
KDNode * buildKDTree(Point* points, int start, int end, Type type){
	if(start == end){
		return new KDNode(points[start].x, points[start].y, type);
	}
	
	//中位数下标 
	int mid = (end+start+1)/2;
	//找到指定维度的中位数的点
	Point p = median_point(points, start, end, type);
	KDNode *node = new KDNode(p.x, p.y, type);
	
	//如果当前的中点已经是开头，说明中点左边没有节点，而中点已经在根节点被保存了信息，所以不需要再次生成一个节点 
	if(mid != start){
		node->left = buildKDTree( points, start, mid-1 ,(Type)(-((int)type)));
	}
	//如果当前的中点已经是结尾，说明中点右边没有节点，而中点已经在根节点被保存了信息，所以不需要再次生成一个节点
	if(mid != end){
		node->right = buildKDTree(points, mid+1, end, (Type)(-((int)type)));
	}
	return node;
}
void preOrderTriverse(KDNode *node){
	cout<<*node;
	cout<<"[";
	if(node->left!=NULL)
		preOrderTriverse(node->left);
	cout<<"]";
	cout<<"[";
	if(node->right!=NULL)
		preOrderTriverse(node->right);
	cout<<"]";
} 
void addNodes(stack<KDNode*>* stk, KDNode **curnode, Point point){
	while((*curnode)->left or (*curnode)->right){
		stk->push(*curnode);
		//cout<<"from "<<**curnode;
		if((*curnode)->type==X){
			//说明当前节点将平面分为x左侧和x右侧； 
			if(point.x < (*curnode)->x) {
				//进入当前节点的左子树 
				if((*curnode)->left!=NULL){
					*curnode = (*curnode)->left; 
				}else break;
			}else{
				//进入当前节点的右子树 
				if((*curnode)->right!=NULL){
					*curnode = (*curnode)->right;
				}else break;
			} 
		}else if((*curnode)->type == Y){
			//说明当前节点将平面分为y上侧和y下侧;
			if(point.y < (*curnode)->y) {
				//进入当前节点的左子树 
				if((*curnode)->left!=NULL){
					*curnode = (*curnode)->left; 
				}else break;
			}else{
				//进入当前节点的右子树 
				if((*curnode)->right!=NULL){
					*curnode = (*curnode)->right;
				}else break;
			} 	
		}
		//cout<<" enter:"<<**curnode<<'\n';
	}
	stk->push(*curnode);
	//cout<<"reach leaf:"<<**curnode<<'\n';
}
void updateR(double * r, KDNode * curnode, Point point, KDNode ** targetNode){
	double new_r = sqrt((curnode->x - point.x) * (curnode->x - point.x) + (curnode->y - point.y) * (curnode->y - point.y));
	//cout<<"curnode:"<<*curnode<<"new r:"<<new_r<<'\n';
	if(new_r < *r){
		*r = new_r;
		*targetNode = curnode;
		//cout<<"new point:"<<*curnode;
	} 
}
double findPoint(KDNode * root, Point point,KDNode **targetNode){
	stack<KDNode *> stk;
	double r;
	//第一从上到下，找到回溯路径
	KDNode* curnode=root;
	//cout<<*curnode<<'\n';
	addNodes(&stk, &curnode, point); 
	//跳出回溯路径时curnode在哪一个点，就计算哪一个点的距离并且开始回溯
	r = sqrt((curnode->x - point.x) * (curnode->x - point.x) + (curnode->y - point.y) * (curnode->y - point.y));
	*targetNode = curnode;
	//cout<<" ,r = "<<r<<'\n';
	while(stk.size()>0){
		//回溯到父节点(上一节点)
		curnode = stk.top();
		stk.pop();
		//cout<<"trace back to:"<<*curnode<<",";
		updateR(&r, curnode, point, targetNode);
		//cout<<", r="<<r<<'\n';
		//比较point和当前节点的水平距离差与r的关系，如果与父节点的线相交，则需要考虑父节点的距离是否小于当前距离 
		if(curnode->type == X){
			//cout<<"compare X,";
			if(abs(point.x - curnode->x)<r){
				//cout<<"across with:"<<*curnode<<", ";
				//与父节点相交,考虑父节点是不是最近邻接点 
				updateR(&r,curnode,point, targetNode);
				if(point.x < curnode->x  and curnode->right!=NULL){
					//当前节点是从左子树回溯上来的，遍历右子树
					curnode = curnode->right;
					addNodes(&stk,&curnode, point);
					//curnode到达叶子节点 
					//updateR(&r, curnode, point, targetNode);
				}else if(point.x > curnode->x and curnode->left!=NULL){
					//当前节点是从右子树回溯上来的，遍历左子树 
					curnode = curnode->left;
					addNodes(&stk,&curnode, point);
					//curnode到达叶子节点 
					//updateR(&r,curnode,point, targetNode);
				}
			} 
		}else if(curnode->type ==Y){
			//cout<<"compare Ys,"<<curnode->y<<" and "<<point.y<<",";
			if(abs(point.y - curnode->y)<r){
				//cout<<"across with:"<<*curnode<<", ";
				//与父节点相交,考虑父节点是不是最近邻接点 
				updateR(&r,curnode,point, targetNode);
				if(point.y < curnode->y  and curnode->right!=NULL){
					//当前节点是从左子树回溯上来的，遍历右子树
					curnode = curnode->right;
					addNodes(&stk,&curnode, point);
					//curnode到达叶子节点 
					//updateR(&r,curnode,point, targetNode);
				}else if(point.y > curnode->y and curnode->left!=NULL){
					//当前节点是从右子树回溯上来的，遍历左子树 
					curnode = curnode->left;
					addNodes(&stk,&curnode, point);
					//curnode到达叶子节点 
					//updateR(&r,curnode,point, targetNode);
				}
			} 
		} 
	}
	return r;
}
int main(){
	int n;
	ifstream fin("KDTreeInput.txt");
	fin>>n;
	/*cout<<"please input the number of the points:";
	cin>>n ;
	cout<<"please input "<<n<<"points,each line with the format of x y:\n";
	*/
	Point arr[n];
	for(int i=0;i<n;i++){
		fin>>arr[i].x>>arr[i].y;
	}
	Point point;
	/*
	cout<<"please input the point you want to quiry with the format x y:";
	cin>>point.x>>point.y;
	*/ 
	fin>>point.x>>point.y;
	KDNode* KDroot = buildKDTree(arr, 0, n-1, X);
	cout<<"KDTree: ";
	preOrderTriverse(KDroot);
	cout<<'\n';
	cout<<"pos :"<<point<<'\n';
	KDNode * targetNode;
	cout<<"minimum distance is:"<<findPoint(KDroot, point, &targetNode)<<'\n';
	cout<<"target point is:"<<"("<<targetNode->x <<","<<targetNode->y <<")"<<'\n';
	return 0;
}
