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
//�������ָ��ά�Ƚ���һ�ο�������
Point median_point(Point *array, int begin, int end, Type type){
	quick_sort(array, begin, end, type);
	return array[(begin+end+1)/2];
}

//typeΪ��ǰ��Ҫ��x��ָ�ǰ�y��ָ� 
KDNode * buildKDTree(Point* points, int start, int end, Type type){
	if(start == end){
		return new KDNode(points[start].x, points[start].y, type);
	}
	
	//��λ���±� 
	int mid = (end+start+1)/2;
	//�ҵ�ָ��ά�ȵ���λ���ĵ�
	Point p = median_point(points, start, end, type);
	KDNode *node = new KDNode(p.x, p.y, type);
	
	//�����ǰ���е��Ѿ��ǿ�ͷ��˵���е����û�нڵ㣬���е��Ѿ��ڸ��ڵ㱻��������Ϣ�����Բ���Ҫ�ٴ�����һ���ڵ� 
	if(mid != start){
		node->left = buildKDTree( points, start, mid-1 ,(Type)(-((int)type)));
	}
	//�����ǰ���е��Ѿ��ǽ�β��˵���е��ұ�û�нڵ㣬���е��Ѿ��ڸ��ڵ㱻��������Ϣ�����Բ���Ҫ�ٴ�����һ���ڵ�
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
			//˵����ǰ�ڵ㽫ƽ���Ϊx����x�Ҳࣻ 
			if(point.x < (*curnode)->x) {
				//���뵱ǰ�ڵ�������� 
				if((*curnode)->left!=NULL){
					*curnode = (*curnode)->left; 
				}else break;
			}else{
				//���뵱ǰ�ڵ�������� 
				if((*curnode)->right!=NULL){
					*curnode = (*curnode)->right;
				}else break;
			} 
		}else if((*curnode)->type == Y){
			//˵����ǰ�ڵ㽫ƽ���Ϊy�ϲ��y�²�;
			if(point.y < (*curnode)->y) {
				//���뵱ǰ�ڵ�������� 
				if((*curnode)->left!=NULL){
					*curnode = (*curnode)->left; 
				}else break;
			}else{
				//���뵱ǰ�ڵ�������� 
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
	//��һ���ϵ��£��ҵ�����·��
	KDNode* curnode=root;
	//cout<<*curnode<<'\n';
	addNodes(&stk, &curnode, point); 
	//��������·��ʱcurnode����һ���㣬�ͼ�����һ����ľ��벢�ҿ�ʼ����
	r = sqrt((curnode->x - point.x) * (curnode->x - point.x) + (curnode->y - point.y) * (curnode->y - point.y));
	*targetNode = curnode;
	//cout<<" ,r = "<<r<<'\n';
	while(stk.size()>0){
		//���ݵ����ڵ�(��һ�ڵ�)
		curnode = stk.top();
		stk.pop();
		//cout<<"trace back to:"<<*curnode<<",";
		updateR(&r, curnode, point, targetNode);
		//cout<<", r="<<r<<'\n';
		//�Ƚ�point�͵�ǰ�ڵ��ˮƽ�������r�Ĺ�ϵ������븸�ڵ�����ཻ������Ҫ���Ǹ��ڵ�ľ����Ƿ�С�ڵ�ǰ���� 
		if(curnode->type == X){
			//cout<<"compare X,";
			if(abs(point.x - curnode->x)<r){
				//cout<<"across with:"<<*curnode<<", ";
				//�븸�ڵ��ཻ,���Ǹ��ڵ��ǲ�������ڽӵ� 
				updateR(&r,curnode,point, targetNode);
				if(point.x < curnode->x  and curnode->right!=NULL){
					//��ǰ�ڵ��Ǵ����������������ģ�����������
					curnode = curnode->right;
					addNodes(&stk,&curnode, point);
					//curnode����Ҷ�ӽڵ� 
					//updateR(&r, curnode, point, targetNode);
				}else if(point.x > curnode->x and curnode->left!=NULL){
					//��ǰ�ڵ��Ǵ����������������ģ����������� 
					curnode = curnode->left;
					addNodes(&stk,&curnode, point);
					//curnode����Ҷ�ӽڵ� 
					//updateR(&r,curnode,point, targetNode);
				}
			} 
		}else if(curnode->type ==Y){
			//cout<<"compare Ys,"<<curnode->y<<" and "<<point.y<<",";
			if(abs(point.y - curnode->y)<r){
				//cout<<"across with:"<<*curnode<<", ";
				//�븸�ڵ��ཻ,���Ǹ��ڵ��ǲ�������ڽӵ� 
				updateR(&r,curnode,point, targetNode);
				if(point.y < curnode->y  and curnode->right!=NULL){
					//��ǰ�ڵ��Ǵ����������������ģ�����������
					curnode = curnode->right;
					addNodes(&stk,&curnode, point);
					//curnode����Ҷ�ӽڵ� 
					//updateR(&r,curnode,point, targetNode);
				}else if(point.y > curnode->y and curnode->left!=NULL){
					//��ǰ�ڵ��Ǵ����������������ģ����������� 
					curnode = curnode->left;
					addNodes(&stk,&curnode, point);
					//curnode����Ҷ�ӽڵ� 
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
