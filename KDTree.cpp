//2019aim1006, Mahip Soni.
// IIT Ropar.

#include<bits/stdc++.h>

using namespace std;
int alpha;

struct node{
	bool axis; //0 for x, 1 for y axis
	int splitvalue;
	struct node* left;
	struct node* right;
	int xmin, ymin, xmax, ymax;
	struct node* parent;
	bool position; //0 for left, 1 for right
	vector<vector<int> > result;
	bool isleaf;
};

bool sortcolx( const vector<int>& v1,
               const vector<int>& v2) {
 return v1[1] < v2[1];
}

bool sortcoly( const vector<int>& v1,
               const vector<int>& v2) {
 return v1[2] < v2[2];
}

//All the functions used
node* root;
node* bulkload(node* parentnode, int start, int end, bool pos, vector<vector <int> > xs);
void visualize(node* currnode);
bool search(int pid, int x, int y, node* currnode);
bool TestIntersect(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
bool pointinside(int xmi, int ymi, int xma, int yma, int x, int y);
bool Inside(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
node* searchposition(int x, int y, node* currnode);
node* insert(node* currnode, int pid, int x, int y);
node* deletion(node* currnode, int pid, int x, int y);
void range(int xmi, int ymi, int xma, int yma);
int RegionXmi(node* v);
int RegionYmi(node* v);
int RegionXma(node* v);
int RegionYma(node* v);
bool RegionInside(node* v, int xmi, int ymi, int xma, int yma);
bool RegionIntersect(node* v, int xmi, int ymi, int xma, int yma);
int searchKDtree(node* v, int xmi, int ymi, int xma, int yma);
int PrintBelow(node* v);
void naivesearch(std::vector<std::vector<int> > v, int xmi, int ymi, int xma, int yma);
int kdheight(node* v);

int main(){
  // filestream variable file

  	cout<<"Enter the value of Alpha\n";
  	cin>>alpha;

        fstream data; //Input all data from file into 2D Vector
        string filename;
				cout<<"Enter filename.txt(dataA.txt or dataB.txt or dataC.txt)\n";
        cin>>filename;
        data.open(filename.c_str());
        int p,x,y;

        int i=0;
				std::vector<std::vector<int> > cd;
        while(data>>p>>x>>y){
          cd.push_back(vector<int>());
          cd[i].push_back(p);
          cd[i].push_back(x);
          cd[i].push_back(y);
          i++;
        }

        int n = cd.size(); // Number of rows. Number of columns =2.
				root = bulkload(NULL, 0, n-1, 0, cd);
				int ans=0;

				cout<<"-----------------------\n";
				int flag=0;
				while(flag==0){
					cout<<"Enter choices.\n1.visualize 2.insert 3.deletion 4.search\n5.range 6.naivesearch 7.kdheight 8.end\n";

					int check;
					cin>>check;
					switch (check) {
						case 1:
						visualize(root);
						cout<<"-----------------------\n";
						break;

						case 2:
						cout<<"Insert point as pid, x, y\n";
						int pidin, xin, yin;
						cin>>pidin>>xin>>yin;
						insert(root, pidin, xin, yin);
						cout<<"-----------------------\n";
						break;

						case 3:
						cout<<"Insert point as pid, x, y\n";
						int pidind, xind, yind;
						cin>>pidind>>xind>>yind;
						deletion(root, pidind, xind, yind);
						cout<<"-----------------------\n";
						break;

						case 4:
						cout<<"Insert point as pid, x, y\n";
						int pidins, xins, yins;
						cin>>pidins>>xins>>yins;

						if(search(pidins, xins, yins, root)){
						cout<<"Found\n";
						}

						else{
						cout<<"Not found\n";
						}
						cout<<"-----------------------\n";
						break;

						case 5:
						cout<<"Enter region to find range as, xmin, ymin, xmax, ymax\n";
						int xmini, ymini, xmaxi, ymaxi;
						cin>>xmini>>ymini>>xmaxi>>ymaxi;

						searchKDtree(root, xmini,ymini,xmaxi,ymaxi);
						cout<<"-----------------------\n";
						break;

						case 6:
						cout<<"Enter region to find range as, xmin, ymin, xmax, ymax\n";
						int xminin, yminin, xmaxin, ymaxin;
						cin>>xminin>>yminin>>xmaxin>>ymaxin;

						naivesearch(cd, xminin,yminin,xmaxin,ymaxin);
						cout<<"-----------------------\n";
						break;

						case 7:
						cout<<kdheight(root);
						cout<<"\n-----------------------\n";
						break;

						default:
						flag=1;
						break;
					}
				}
}


node* bulkload(node* parentnode, int start, int end, bool pos, vector<vector <int> > xs){

	node* temp = new node;
	vector<vector <int> > ys = xs;
	sort(xs.begin(), xs.end(), sortcolx);
	sort(ys.begin(), ys.end(), sortcoly);

	if((xs.size())<=alpha){

		temp->parent = parentnode;
		temp->isleaf = 1;
		if(temp->parent->axis){//y axis
			if(xs[0][1]<temp->parent->splitvalue)
				temp->position=0;

			else
				temp->position=1;
		}

		else{
			if(xs[0][2]<temp->parent->splitvalue)
				temp->position=0;

			else
				temp->position=1;
		}

		for(int i=0; i<xs.size(); i++)
			temp->result.push_back(xs[i]);
	}

	else if(parentnode==NULL){


		bool parent_axis;
		int med;
		int medpos;
		if(xs[xs.size()-1][1] - xs[0][1] >= ys[ys.size()-1][2] - ys[0][2]){
			parent_axis = 1;
			medpos = (xs.size())/2;

			while((xs[medpos][1]==xs[medpos+1][1])&&(medpos<xs.size()-2))
			medpos++;

			med = xs[medpos][1];
		}

		else{
			parent_axis = 0;
			medpos = (ys.size())/2;
				while((ys[medpos][2]==ys[medpos+1][2])&&(medpos<xs.size()-2))
			medpos++;

			med = ys[medpos][2];
		}



		temp->isleaf = 0;
		temp->splitvalue = med;
		temp->axis = parent_axis;

		temp->xmin = xs[0][1];
		temp->ymin = ys[0][2];
		temp->xmax = xs[xs.size()-1][1];
		temp->ymax = ys[ys.size()-1][2];

		temp->parent = NULL;
		temp->position = 0;

		temp->result.push_back(vector<int>());

		vector< vector< int> > sol;
		vector< vector< int> > sor;

		if(parent_axis){

			for(int i=0; i<=medpos; i++)
					sol.push_back(xs[i]);

			for(int i=medpos+1; i<xs.size(); i++)
					sor.push_back(xs[i]);

					temp->left = bulkload(temp, start, medpos, 0, sol);
					temp->right = bulkload(temp, medpos+1, end, 1, sor);
			}

			else{

				for(int i=0; i<=medpos; i++)
						sol.push_back(ys[i]);

				for(int i=medpos+1; i<ys.size(); i++)
						sor.push_back(ys[i]);

						temp->left = bulkload(temp, start, medpos, 0, sol);
						temp->right = bulkload(temp, medpos+1, end, 1, sor );
				}


		}




	else{

		bool parent_axis;
		int med;
		int medpos;

		if(xs[xs.size()-1][1] - xs[0][1] >= ys[ys.size()-1][2] - ys[0][2]){
			parent_axis = 1;
			medpos = (xs.size())/2;

			while((xs[medpos][1]==xs[medpos+1][1])&&(medpos<xs.size()-2))
			medpos++;

			med = xs[medpos][1];
		}

		else{
			parent_axis = 0;
			medpos = (ys.size())/2;

			while((ys[medpos][2]==ys[medpos+1][2])&&(medpos<xs.size()-2))
			medpos++;

			med = ys[medpos][2];
		}


		temp->isleaf = 0;

		temp->splitvalue = med;
		temp->axis = parent_axis;

		temp->parent = parentnode;
		temp->position = pos;

		temp->result.push_back(vector<int>());

		vector< vector< int> > sol;
		vector< vector< int> > sor;

		if(parent_axis){

		for(int i=0; i<=medpos; i++)
				sol.push_back(xs[i]);

		for(int i=medpos+1; i<xs.size(); i++)
				sor.push_back(xs[i]);

				temp->left = bulkload(temp, start, medpos, 0, sol);
				temp->right = bulkload(temp, medpos+1, end, 1, sor);
		}

		else{

			for(int i=0; i<=medpos; i++)
					sol.push_back(ys[i]);

			for(int i=medpos+1; i<ys.size(); i++)
					sor.push_back(ys[i]);

					temp->left = bulkload(temp, start, medpos, 0, sol);
					temp->right = bulkload(temp, medpos+1, end, 1, sor );
			}

	}

	return temp;
}

void visualize(node* currnode){

	queue<node*> q;

	q.push(currnode);
	int i=1;

	while(!q.empty()){
		node* temp = q.front();
		q.pop();
		cout<<"sno "<<i<<" splitvalue= "<<temp->splitvalue<<" axis= "<<temp->axis<<"\n";

		if(!temp->left->isleaf)
		q.push(temp->left);

		if(!temp->right->isleaf)
		q.push(temp->right);
		i++;
	}
}



bool search(int pid, int x, int y, node* currnode){

	if(!(x>=root->xmin&&y>=root->ymin&&x<=root->xmax&&y<=root->ymax))
	return 0;

	else{

		if(currnode->isleaf){
			bool ans=0;



			for(int i=0; i<currnode->result.size(); i++){

				if((currnode->result[i][0]==pid)&&(currnode->result[i][1]==x) && (currnode->result[i][2]==y)){
					ans = 1;
					break;
				}
			}

			return ans;
		}




		else{
			if(currnode->axis){ //y axis

				if(x<=currnode->splitvalue)
				return search(pid, x, y, currnode->left);

				else
				return search(pid, x,y, currnode->right);
			}

			else{ // x axis
				if(y<=currnode->splitvalue)
				return search(pid, x, y, currnode->left);

				else
				return search(pid, x, y, currnode->right);
			}

		}
	}

}

node* searchposition(int x, int y, node* currnode){

	if(currnode->isleaf){
		return currnode;
	}




	else{
		if(currnode->axis){ //y axis
			if(x<=currnode->splitvalue)
			return searchposition(x, y, currnode->left);

			else
			return searchposition(x,y, currnode->right);
		}

		else{ // x axis
			if(y<=currnode->splitvalue)
			searchposition(x, y, currnode->left);

			else
			searchposition(x,y, currnode->right);
		}

	}

}

node* insert(node* currnode, int pid, int x, int y){

	node* temp = searchposition(x, y, currnode);
	vector< int > vin;
	vin.push_back(pid);
	vin.push_back(x);
	vin.push_back(y);


	if(temp->result.size()<alpha){

		temp->result.push_back(vin);
	}

	else{
		temp->isleaf=0;

		vector<int> xarr;
		vector<int> yarr;

		for(int i=0; i<temp->result.size(); i++){
			xarr.push_back(temp->result[i][1]);
			yarr.push_back(temp->result[i][2]);
		}

		xarr.push_back(x);
		yarr.push_back(y);

		sort(xarr.begin(), xarr.end());
		sort(yarr.begin(), yarr.end());

		vector<vector<int > > input;
		input = temp->result;

		input.push_back(vin);

		int medpos = (alpha)/2;

		if((xarr[alpha] - xarr[0])>=(yarr[alpha] - yarr[0])){
			temp->axis = 1;
			temp->splitvalue = xarr[medpos];
			sort(input.begin(), input.end(), sortcolx);
			}

		else{
			temp->axis = 0;
			temp->splitvalue = yarr[medpos];
			sort(input.begin(), input.end(), sortcoly);
		}

		node* templ = new node;
		node* tempr = new node;

		temp->left = templ;
		temp->right = tempr;

		templ->position = 0;
		templ->isleaf = 1;
		templ->parent = temp;

		tempr->position = 1;
		tempr->isleaf = 1;
		tempr->parent = temp;


		for(int i=0; i<=medpos; i++)
		templ->result.push_back(input[i]);

		for(int i=medpos+1; i<=alpha; i++)
		tempr->result.push_back(input[i]);

	}

	return temp;
}

node* deletion(node* currnode, int pid, int x, int y){
			node* temp = searchposition(x,y, currnode);
			vector< int > target;
			target.push_back(pid);
			target.push_back(x);
			target.push_back(y);

			if(temp->result.size()>1){
				for(int i=0; i<temp->result.size(); i++){
					if(temp->result[i]==target){
						temp->result.erase(temp->result.begin()+i);
						break;
					}
				}
			}

			else{
				if(temp->position==0){
					if(temp->parent->right->isleaf){
						temp->parent->isleaf = 1;
						temp->parent->result = temp->parent->right->result;
						node* temptemp = temp->right;
						temp->right = NULL;
						temp->left = NULL;
						temp->splitvalue = -1;
						free(temp);
						free(temptemp);
					}
				}

				else{
					if(temp->parent->left->isleaf){
						temp->parent->isleaf = 1;
						temp->parent->result = temp->parent->left->result;
						node* temptemp = temp->left;
						temp->right = NULL;
						temp->left = NULL;
						temp->splitvalue = -1;
						free(temp);
						free(temptemp);
					}
				}
			}


			return temp;
}

bool pointinside(int xmi, int ymi, int xma, int yma, int x, int y){
	if(x>=xmi&&y>=ymi&&x<=xma&&y<=yma)
		return 1;

	else
		return 0;
}

bool TestIntersect(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4){
	if((pointinside(x1,y1,x2,y2, x3,y3)||pointinside(x1,y1,x2,y2, x4,y4))||(pointinside(x3,y3,x4,y4, x1,y1)||pointinside(x3,y3,x4,y4,x2,y2))){
		return 1;
	}

	else{
		int x11, y11,x21, y21, x31,y31, x41, y41;
		x11 = x1;
		y11 = y2;
		x21 = x2;
		y21 = y1;
		x31 = x3;
		y31 = y4;
		x41 = x4;
		y41 = y3;

			if((pointinside(x1,y1,x2,y2,x31,y31)||pointinside(x1,y1,x2,y2, x41,y41))||(pointinside(x3,y3,x4,y4, x11,y11)||pointinside(x3,y3,x4,y4,x21,y21)))
			return 1;

			else
			return 0;
	}

}

bool Inside(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4){//3,4 is inside 1,2
	if((pointinside(x1,y1,x2,y2,x3,y3)&&pointinside(x1,y1,x2,y2,x4,y4))){
		return 1;
	}

	else
	return 0;
}

int searchKDtree(node* v, int xmi, int ymi, int xma, int yma){

		if(v->isleaf){

			for(int i=0; i<v->result.size(); i++){
				if((pointinside(xmi, ymi, xma, yma, v->result[i][1], v->result[i][2])))
				cout<<v->result[i][0]<<" "<<v->result[i][1]<<" "<<v->result[i][2]<<"\n";
			}
				return 0;
		}

		else{

			if(RegionInside(v->left, xmi,ymi,xma,yma)){

					PrintBelow(v->left);

				}

			else if(RegionIntersect(v->left, xmi,ymi,xma,yma)){

					searchKDtree(v->left, xmi, ymi, xma, yma);
			}

			if(RegionInside(v->right, xmi,ymi,xma,yma)){

					PrintBelow(v->right);

				}

			else if(RegionIntersect(v->right, xmi,ymi,xma,yma)){

					searchKDtree(v->right, xmi, ymi, xma, yma);
			}
}

return 0;
}

bool RegionInside(node* v, int xmi, int ymi, int xma, int yma){
			int vxmi, vymi, vxma, vyma;
			vxmi = RegionXmi(v);
			vymi = RegionYmi(v);
			vxma = RegionXma(v);
			vyma = RegionYma(v);

			return Inside(xmi,ymi,xma,yma,vxmi,vymi,vxma,vyma);
}

bool RegionIntersect(node* v, int xmi, int ymi, int xma, int yma){
			int vxmi, vymi, vxma, vyma;
			vxmi = RegionXmi(v);
			vymi = RegionYmi(v);
			vxma = RegionXma(v);
			vyma = RegionYma(v);

			return TestIntersect(xmi,ymi,xma,yma,vxmi,vymi,vxma,vyma);
}

int RegionXmi(node* v){
			if(v->parent==root){
				if((v->position==1)&&(v->parent->axis==0)){
					return v->parent->splitvalue;
				}

				else
				return root->xmin;
			}

			if((v->parent->axis==1)&&(v->position==1))
			return v->parent->splitvalue;

			else
			RegionXmi(v->parent);
}

int RegionYmi(node* v){
			if(v->parent==root){
				if((v->position==1)&&(v->parent->axis==1)){
					return v->parent->splitvalue;
				}

				else
				return root->ymin;
			}

			if((v->parent->axis==0)&&(v->position==1))
			return v->parent->splitvalue;

			else
			RegionYmi(v->parent);
}

int RegionXma(node* v){
			if(v->parent==root){
				if((v->position==0)&&(v->parent->axis==0)){
					return v->parent->splitvalue;
				}

				else
				return root->xmax;
			}


			if((v->parent->axis==1)&&(v->position==0))
			return v->parent->splitvalue;

			else
			RegionXma(v->parent);
}

int RegionYma(node* v){
			if(v->parent==root){
				if((v->position==0)&&(v->parent->axis==1)){
					return v->parent->splitvalue;
				}

				else
				return root->ymax;
			}


			if((v->parent->axis==0)&&(v->position==0))
			return v->parent->splitvalue;

			else
			RegionYma(v->parent);
}

int PrintBelow(node* v){
			if(v==NULL)
			return 0;

			if(v->isleaf){
				for(int i=0; i<v->result.size(); i++){
					cout<<v->result[i][0]<<" "<<v->result[i][1]<<" "<<v->result[i][2]<<"\n";
				}
				return 0;
			}

			else{
				PrintBelow(v->left);

				PrintBelow(v->right);
			}
				return 0;

}

void naivesearch(std::vector<std::vector<int> > v, int xmi, int ymi, int xma, int yma){
			for(int i=0; i<v.size(); i++){
				if(pointinside(xmi,ymi,xma,yma,v[i][1], v[i][2]))
					cout<<v[i][0]<<" "<<v[i][1]<<" "<<v[i][2]<<"\n";
			}
}

int kdheight(node* v){
			if(v->isleaf)
			return 0;

			else{
				int hl = kdheight(v->left);
				int hr = kdheight(v->right);

				if(hl>hr)
				return(hl+1);

				else
				return (hr+1);
			}
}
