#include<iostream>
#include<fstream>
//#include<vector>

using namespace std;

class graph_and_data_structures{

    int n;
    int adjecency_matrix[50][50];
    char source_vertex;
    int Distance[50];
    char last_modified;
    int Heap[50];
    char Loc1[50];// array which contains (one to one) nodes in heap
    int Loc2[50];// array which contains position of each node in heap
    int Heap_Track[50]; //if entry vertex is in Heap==1, if not==0
    //int Distance_track[50];
    int parent[50];

    public:

        void get_n(FILE *fp){
            fscanf(fp,"%d",&n);
        }

        void get_source_vertex(FILE *fp){
            fscanf(fp," %c",&source_vertex);
        }

        void create_adjecency_matrix(FILE *fp){
            char index_row,index_column;
            int weight,i,j;
            for(i=0;i<n;i++){
                for(j=0;j<n;j++){
                        adjecency_matrix[i][j]=-1;
                }
            }
            while(!feof(fp)){
                fscanf(fp," %c %c %d",&index_row,&index_column,&weight);
                adjecency_matrix[(int)index_row-65][(int)index_column-65]=weight;
                //adjecency_matrix[(int)index_column-65][(int)index_row-65]=weight;
            }
        }

        void display_graph(){
            int i,j;
            for(i=0;i<n;i++){
                for(j=0;j<n;j++){
                    printf("%d   ",adjecency_matrix[i][j]);
                }
                printf("\n");
            }
        }

        //int get_n(){
        //    return n;
        //}
        //void heapify(){

        //}
        void heap_insert(int key,char vertex){  //heap_and_loc12_initializer is the caller
            Heap[++Heap[0]]=key;                //insert the new vertex at last position of heap
            Loc1[Heap[0]]=vertex;               //
            Loc2[(int)vertex-65]=Heap[0];
            int i=Heap[0];
            while(i>0 && Heap[i]<Heap[i/2]){
                swap_num(&Heap[i],&Heap[i/2]);
                swap_num(&Loc2[(int)(Loc1[i])-65],&Loc2[(int)(Loc1[i/2])-65]);
                swap_char(&Loc1[i],&Loc1[i/2]);
                i=i/2;
            }
        }

        void heap_and_loc12_HTrack_initializer(){// for user in main engine
            int i;
//            for(i=0;i<n;i++){
//                if((adjecency_matrix[(int)source_vertex-65][i])==-1){
//                    counter++;
//                }
//            }
           // Heap[0]=counter;// not to be declared actually
           Heap[0]=0;
           for(i=0;i<n;i++)
                Heap_Track[i]=0;
           //Heap_Track[0]=0;
            for(i=0;i<n;i++){
                if(adjecency_matrix[(int)source_vertex-65][i]!=-1){

                    heap_insert(adjecency_matrix[(int)source_vertex-65][i],(char)(i+65));
                    Heap_Track[i]=1;
                    parent[i]=(int)source_vertex-65;
                }
            }
        }

        void distance_initializer(){
            int i;
            Distance[(int)source_vertex-65]=0;
            for(i=0;i<n;i++){
                if(i!=(int)source_vertex-65)
                    Distance[i]=-1;
            }
        }



        int heap_is_empty(){
            if(Heap[0]==0)
                return 1;
            else
                return 0;
        }

        void heap_extract_min(){        // extracts min from root node and inserts in Distance array(used in engine)
            //if(Distance[(int)(Loc1[1])-65]==-1){
                Distance[(int)(Loc1[1])-65]=Heap[1];// puts element in top of min-heap to Distance array
                last_modified=Loc1[1];              // keeps track of last modified element in Distance array
                int last=Heap[0];             //******************* last index of heap** this is bug(actually this is giving last value of heap)
                swap_num(&Heap[1],&Heap[last]);     // swaps root and last element in heap
                swap_num(&Loc2[(int)(Loc1[1])-65],&Loc2[(int)(Loc1[last])-65]);     //swaps corresponding values of positions(index) in Loc2
                swap_char(&Loc1[1],&Loc1[last]);    // swap items in Loc1
                Heap_Track[(int)last_modified-65]=0;
                Heap[0]--;
                min_heapify(Heap,1);
          //  }
        }

        void min_heapify(int Heap[],int key){        // will take care of array Loc1 and Loc2 simultaneously
            int i=key,min_i;
            while(i<Heap[0] && 2*i<=Heap[0]){        //Heap[0] replaced by n
                if(2*i+1<=Heap[0]){
                    min_i=Heap[2*i]<Heap[2*i+1]?2*i:2*i+1;
                    if(Heap[i]>Heap[min_i]){
                        swap_num(&Heap[i],&Heap[min_i]);
                        swap_num(&Loc2[(int)(Loc1[i])-65],&Loc2[(int)(Loc1[min_i])-65]);
                        swap_char(&Loc1[i],&Loc1[min_i]);
                        i=min_i;
                    }
                    else{
                        break;
                    }
                }
                else{
                    if(Heap[i]>Heap[2*i]){
                        swap_num(&Heap[i],&Heap[2*i]);
                        swap_num(&Loc2[(int)(Loc1[i])-65],&Loc2[(int)(Loc1[2*i])-65]);
                        swap_char(&Loc1[i],&Loc1[2*i]);
                        i=2*i;
                    }
                    else{
                        break;
                    }
                }
            }
        }

        void Update_Heap(){
            int i;
            for(i=0;i<n;i++){
                if(adjecency_matrix[(int)last_modified-65][i]!=-1){
                        //check which is already there and update
                        //check which is not there and insert
                    if(Distance[i]==-1){
                        if(Heap_Track[i]==1){
                            if(Heap[Loc2[i]]>(Distance[(int)last_modified-65])+(adjecency_matrix[(int)last_modified-65][i])){
                                Heap[Loc2[i]]=(Distance[(int)last_modified-65])+(adjecency_matrix[(int)last_modified-65][i]);
                                min_heapify(Heap,Loc2[i]);
                                parent[i]=(int)last_modified-65;
                            }

                        }
                        else{
                            heap_insert((Distance[(int)last_modified-65])+(adjecency_matrix[(int)last_modified-65][i]),(char)(i+65));
                            Heap_Track[i]=1;
                            parent[i]=(int)last_modified-65;
                        }
                    }

                }
            }
        }

        void parent_initialiser(){
            int i;
            for(i=0;i<n;i++){
                parent[i]=-1;
            }
        }

        void parent_printer(){
            int i;
            for(i=0;i<n;i++){
                printf("%d ",parent[i]);
            }
        }

        void print_path(){
            int i;
            for(i=0;i<n;i++){
                int j=i;
                printf(" %c ",(char)(j+65));
                do{
                    printf(" %c ",(char)(parent[j]+65));
                    j=parent[j];
                }
                while(parent[j]!=-1);
                //printf(" %c ",source_vertex);
                printf("\n");
            }
        }

        void swap_num(int *n1,int *n2){    //check c++ swap()
            int temp=*n1;
            *n1=*n2;
            *n2=temp;
        }
        void swap_char(char *n1,char *n2){    //check c++ swap()
            char temp=*n1;
            *n1=*n2;
            *n2=temp;
        }
        void Print_Distance(){
            int i;
            for(i=0;i<n;i++)
               // cout<<(char)(i+65)<<Distance[i]<<endl;
               printf("%c %d, ",(char)(i+65),Distance[i]);
        }

        inline void Print_Source_Vertex_and_n(){
            //cout<<source_vertex<<" "<<n<<"\n";
            printf("%c  %d",source_vertex,n);
        }

         void Print_Heap(){
            int i;
            for(i=0;i<n;i++){
                printf("%d %d, ",i,Heap[i]);
            }
            cout<<"\n"<<"Loc1   ";
            for(i=0;i<n;i++){
                printf("%d %c, ",i,Loc1[i]);
            }
            cout<<"\n"<<"Loc2   ";
            for(i=0;i<n;i++){
                printf("%d %d, ",i,Loc2[i]);
            }
            cout<<"\n"<<"Heap_Track   ";
            for(i=0;i<n;i++){
                printf("%c %d, ",(char)(i+65),Heap_Track[i]);
            }
            cout<<"\n"<<last_modified;
        }
};


int main()
{
    FILE *fp=fopen("F:\\Drive G\\C++\\DIJKSTRA\\Dijkstra\\input3.txt","r");
    graph_and_data_structures g1;
    g1.get_n(fp);
    g1.get_source_vertex(fp);
    g1.create_adjecency_matrix(fp);

    g1.display_graph();
    fclose(fp);

    //data_structures d1(g1.get_n());
    //g1.Print_Source_Vertex_and_n();
    g1.distance_initializer();      // to debug,problem with cout
    //cout<<"\n"<<"Distance Array ";
    //g1.Print_Distance();            //giving wrong, problem with cout
    g1.parent_initialiser();
    g1.heap_and_loc12_HTrack_initializer();
    //g1.parent_initialiser();
    //cout<<"\n"<<"After Initializing Heap,loc1 and 2:     ";
    //g1.Print_Heap();
    //cout<<"\n"<<"Distance Array ";
    //g1.Print_Distance();
    //cout<<"\n"<<"Heap   ";
    //g1.Print_Heap();
    //cout<<"\n\n";
    while(!g1.heap_is_empty()){
        g1.heap_extract_min();          // problem : solved
        //g1.Update_Heap();
        //cout<<"\n"<<"Distance Array ";
        //g1.Print_Distance();
        //cout<<"\n"<<"Heap   ";
        //g1.Print_Heap();
        g1.Update_Heap();               //problem
        //cout<<"\n"<<"Heap after update:   ";
        //g1.Print_Heap();
        //cout<<"\n\n";
        //cout<<"\n"<<"Parent Array: \n";
        //g1.parent_printer();
        //cout<<"\n\n";
    }
    cout<<"\n"<<"Distance Array ";
    g1.Print_Distance();
    cout<<"\n";
    g1.print_path();


    return 0;
}
